<?php

namespace Drupal\openideal_user\Plugin\RulesAction;

use Drupal\user\Entity\User;
use Drupal\rules\Context\ContextConfig;
use Drupal\group\GroupMembershipLoader;
use Drupal\rules\Core\RulesActionBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\rules\Engine\ExpressionManager;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;

/**
 * Provides the transaction create action.
 *
 * @RulesAction(
 *   id = "openideal_user_potins_transaction_bulk_execute",
 *   deriver = "Drupal\openideal_user\Plugin\RulesAction\TransactionBulkExecuteDeriver",
 * )
 */
class TransactionBulkExecute extends RulesActionBase implements ContainerFactoryPluginInterface {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Expression manager.
   *
   * @var \Drupal\rules\Engine\ExpressionManager
   */
  protected $expressionManager;

  /**
   * Group member ship loader.
   *
   * @var \Drupal\group\GroupMembershipLoader
   */
  protected $groupMembershipLoader;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EntityTypeManagerInterface $entity_type_manager,
    ExpressionManager $expressionManager,
    GroupMembershipLoader $groupMembershipLoader
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entity_type_manager;
    $this->expressionManager = $expressionManager;
    $this->groupMembershipLoader = $groupMembershipLoader;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('entity_type.manager'),
      $container->get('plugin.manager.rules_expression'),
      $container->get('group.membership_loader')
    );
  }

  /**
   * {@inheritDoc}
   */
  public function execute() {
    $users = $this->getUsers();
    // Create a actions for every user and grant them points.
    foreach ($users as $group_member) {
      // Loop the transaction create and execute actions per user in group.
      $rules_rule = $this->expressionManager->createRule();
      $rules_rule->addAction('transaction_create:userpoints', $this->prepareContextForTransactionCreate($group_member));
      $context_config = ContextConfig::create()
        ->setValue('immediate', TRUE)
        ->map('transaction', 'transaction');
      $rules_rule->addAction('transaction_execute', $context_config);
      $rules_rule->execute();
    }
  }

  /**
   * Get users in group from node.
   */
  private function getUsers() {
    $users = [];
    $storage = $this->entityTypeManager->getStorage('group_content');
    /** @var \Drupal\group\Entity\GroupContent $group_content */
    $group_contents = $storage->loadByEntity($this->getContextValue('idea'));
    // In case if node were deleted.
    // Because STATE_CHANGED event invokes even if node deleted.
    if (empty($group_contents)) {
      return [];
    }
    $group_content = reset($group_contents);
    $group_members_ships = $this->groupMembershipLoader->loadByGroup($group_content->getGroup());
    foreach ($group_members_ships as $content_group) {
      $member = $content_group->getUser();
      $users += [$member->id() => $member];
    }
    return $users;
  }

  /**
   * Prepare context for transaction create.
   *
   * @param \Drupal\user\Entity\User $user
   *   User.
   *
   * @return \Drupal\rules\Context\ContextConfig
   *   Config.
   */
  private function prepareContextForTransactionCreate(User $user) {
    $context_config = ContextConfig::create();
    foreach ($this->getContexts() as $context_name => $context) {
      $value = $this->getContextValue($context_name);
      if ($context_name == 'idea') {
        continue;
      }
      if ($context_name == 'target_entity') {
        $context_config->setValue($context_name, $user);
        continue;
      }
      $context_config->setValue($context_name, $value);
    }
    return $context_config;
  }

}
