<?php

namespace Drupal\openideal_user\Plugin\RulesAction;

use Drupal\comment\CommentInterface;
use Drupal\content_moderation\ModerationInformation;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\message\Entity\Message;
use Drupal\message\MessageInterface;
use Drupal\rules\Core\RulesActionBase;
use Drupal\user\UserInterface;
use Drupal\votingapi\VoteInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a Message action.
 *
 * @RulesAction(
 *   id = "openideal_message_action",
 *   label = @Translation("Create a message with a reference field"),
 *   category = @Translation("Message"),
 *   context_definitions = {
 *     "template" = @ContextDefinition("string",
 *       label = @Translation("The message template"),
 *       assignment_restriction = "input",
 *       required = TRUE
 *     ),
 *     "referenced_entity" = @ContextDefinition("entity",
 *       label = @Translation("Message's referenced entity field."),
 *       assignment_restriction = "selector",
 *       required = TRUE
 *     ),
 *   }
 * )
 */
class MessageAction extends RulesActionBase implements ContainerFactoryPluginInterface {

  use StringTranslationTrait;

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManager
   */
  protected $entityTypeManager;

  /**
   * Moderation information service.
   *
   * @var \Drupal\content_moderation\ModerationInformation
   */
  protected $moderationInformation;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EntityTypeManager $entityTypeManager,
    ModerationInformation $moderation_information
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
    $this->moderationInformation = $moderation_information;
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
      $container->get('content_moderation.moderation_information')
    );
  }

  /**
   * Create a message with related field.
   *
   * @param string $template
   *   Template ID.
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   The referenced entity.
   */
  protected function doExecute($template, EntityInterface $entity) {
    // If user already voted nothing to do here.
    if ($entity instanceof VoteInterface && $this->isUserVoted($entity, $template)) {
      return;
    }

    $owner_id = $entity instanceof UserInterface ? $entity->id() : $entity->getOwnerId();
    $message = Message::create(['template' => $template, 'uid' => $owner_id]);
    $entity_type = $entity->getEntityTypeId();

    if ($entity instanceof VoteInterface) {
      $entity = $this->entityTypeManager->getStorage($entity->getVotedEntityType())->load($entity->getVotedEntityId());
      $entity_type = $entity->getEntityTypeId();
    }

    // Set arguments in cases if we can't just take data from current entity,
    // e.g. workflow state is changed,
    // and need to save previous one as msg argument.
    $this->setMessageArguments($message, $entity);

    // Set additional node reference to simplify the queries.
    if ($entity instanceof CommentInterface && $template != 'created_reply_on_comment') {
      $message->set('field_node_reference', $entity->getCommentedEntity());
    }

    $message->set('field_' . $entity_type . '_reference', $entity);
    $message->save();
  }

  /**
   * Check if user voted today.
   *
   * @param \Drupal\votingapi\VoteInterface $entity
   *   Entity to check.
   * @param string $template
   *   Template.
   *
   * @return bool
   *   TRUE if user already voted, false otherwise.
   */
  private function isUserVoted(VoteInterface $entity, string $template) {
    $query = $this->entityTypeManager->getStorage('message')->getQuery();
    $result = $query
      ->condition('template', $template)
      ->condition('field_' . $entity->getVotedEntityType() . '_reference', $entity->getVotedEntityId())
      ->condition('uid', $entity->getOwnerId())
      ->execute();
    return !empty($result);
  }

  /**
   * Set arguments to message if need.
   *
   * @param \Drupal\message\MessageInterface $message
   *   Message to set arguments.
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   Entity to get additional info from.
   */
  private function setMessageArguments(MessageInterface $message, EntityInterface $entity) {
    switch ($message->getTemplate()->id()) {
      case 'idea_life_cycle_change':
        $state = $this->moderationInformation->getOriginalState($entity);
        $message->setArguments([
          '@idea_life_cycle' => $state->label(),
        ]);
        break;

      case 'challenge_schedule':
        $message->setArguments([
          '@challenge_status' => $entity->field_is_open->value ? 'Open' : 'Closed',
        ]);
        break;
    }
  }

}
