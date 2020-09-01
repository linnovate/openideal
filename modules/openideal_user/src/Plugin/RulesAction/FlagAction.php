<?php

namespace Drupal\openideal_user\Plugin\RulesAction;

use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Logger\LoggerChannelInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\flag\FlagServiceInterface;
use Drupal\rules\Core\RulesActionBase;
use Drupal\user\UserInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a Flag action.
 *
 * @RulesAction(
 *   id = "openideal_user_flag_action",
 *   label = @Translation("Follow/unfollow the entity"),
 *   category = @Translation("Follow"),
 *   context_definitions = {
 *     "entity" = @ContextDefinition("entity",
 *       label = @Translation("Entity to follow"),
 *       assignment_restriction = "selector",
 *       required = TRUE
 *     ),
 *     "operation" = @ContextDefinition("string",
 *       label = @Translation("The flag operation."),
 *       description = @Translation("The operation can be one of next: flag or unflag"),
 *       assignment_restriction = "input",
 *       required = TRUE
 *     ),
 *     "flag_id" = @ContextDefinition("string",
 *       label = @Translation("The flag id."),
 *       description = @Translation("The identifier of the flag to load."),
 *       assignment_restriction = "input",
 *       required = TRUE
 *     ),
 *     "user" = @ContextDefinition("entity:user",
 *       label = @Translation("User that will follow the entity"),
 *       assignment_restriction = "selector",
 *       required = FALSE
 *     ),
 *   }
 * )
 */
class FlagAction extends RulesActionBase implements ContainerFactoryPluginInterface {

  use StringTranslationTrait;

  /**
   * The flag service.
   *
   * @var \Drupal\flag\FlagServiceInterface
   */
  protected $flagService;

  /**
   * A logger instance.
   *
   * @var \Psr\Log\LoggerInterface
   */
  protected $logger;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    FlagServiceInterface $flag_service,
    LoggerChannelInterface $logger
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->flagService = $flag_service;
    $this->logger = $logger;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('flag'),
      $container->get('logger.factory')->get('rules')
    );
  }

  /**
   * Flag the Entity.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   The entity to be flagged.
   * @param string $operation
   *   Operation (flag or unflag)
   * @param string $flag_id
   *   The identifier of the flag to load.
   * @param \Drupal\user\UserInterface $user
   *   User to flag.
   */
  protected function doExecute(EntityInterface $entity, string $operation, string $flag_id, UserInterface $user = NULL) {
    if ($this->validateOperation($operation)) {
      $flag = $this->flagService->getFlagById($flag_id);
      if ($flag) {
        try {
          $this->flagService->{$operation}($flag, $entity, $user);
        }
        catch (\LogicException $exception) {
          $this->logger->warning($exception->getMessage());
        }
      }
      else {
        $this->logger->warning($this->t("Provided flag id doesn't exists"));
      }
    }
  }

  /**
   * Validate operation.
   *
   * @param string $operation
   *   Operation.
   *
   * @return bool
   *   TRUE if validate, FALSE otherwise.
   */
  private function validateOperation(string $operation) {
    return $operation == 'flag' || $operation == 'unflag';
  }

}
