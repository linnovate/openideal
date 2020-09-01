<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\Component\Plugin\Derivative\DeriverBase;
use Drupal\content_moderation\ModerationInformationInterface;
use Drupal\Core\Plugin\Discovery\ContainerDeriverInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\rules\Context\ContextDefinition;
use Drupal\rules\Context\ContextDefinitionInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Derives condition plugin plugin definitions based on workflow types.
 *
 * @see \Drupal\openideal_user\Plugin\Condition\IdeaIsInState
 */
class IdeaInStateDeriver extends DeriverBase implements ContainerDeriverInterface {

  use StringTranslationTrait;

  /**
   * The moderation information service.
   *
   * @var \Drupal\content_moderation\ModerationInformationInterface
   */
  protected $moderationInformation;

  /**
   * Creates a new TransactionCreateDeriver object.
   *
   * @param \Drupal\content_moderation\ModerationInformationInterface $moderationInformation
   *   The moderation information service.
   */
  public function __construct(ModerationInformationInterface $moderationInformation) {
    $this->moderationInformation = $moderationInformation;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, $base_plugin_id) {
    return new static(
      $container->get('content_moderation.moderation_information')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    /** @var \Drupal\workflows\Entity\Workflow $workflow */
    if ($workflow = $this->moderationInformation->getWorkflowForEntityTypeAndBundle('node', 'idea')) {
      $states = $workflow->get('type_settings')['states'];
      foreach ($states as $state_id => $state_data) {
        // Add the derivative.
        $this->derivatives[$state_id] = [
          'label' => $this->t('Idea is in @workflow_state state', ['@workflow_state' => $state_data['label']]),
          'category' => $this->t('Idea workflows'),
          'provides' => [],
          'context_definitions' => [
            'idea_id' => ContextDefinition::create('integer')
              ->setLabel($this->t('Idea node id.'))
              ->setAssignmentRestriction(ContextDefinitionInterface::ASSIGNMENT_RESTRICTION_SELECTOR)
              ->setRequired(TRUE),
          ],
        ] + $base_plugin_definition;
      }
    }

    return $this->derivatives;
  }

}
