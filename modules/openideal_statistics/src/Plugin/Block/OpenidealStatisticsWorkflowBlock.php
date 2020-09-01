<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\content_moderation\ModerationInformation;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'OpenidealStatisticsWorkflowBlock' block.
 *
 * @Block(
 *  id = "openideal_statistics_status",
 *  admin_label = @Translation("Workflow status."),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealStatisticsWorkflowBlock extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityManager;

  /**
   * Content moderation info.
   *
   * @var \Drupal\content_moderation\ModerationInformation
   */
  protected $moderationInformation;

  /**
   * Constructs a new OpenidealStatisticsWorkflowBlock object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param string $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_manager
   *   Block plugin manager.
   * @param \Drupal\content_moderation\ModerationInformation $moderationInformation
   *   Content moderation info.
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EntityTypeManagerInterface $entity_manager,
    ModerationInformation $moderationInformation
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityManager = $entity_manager;
    $this->moderationInformation = $moderationInformation;
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
   * {@inheritdoc}
   */
  public function build() {
    $contexts = $this->getContexts();
    $build = [];
    if (isset($contexts['node']) && !$contexts['node']->getContextValue()->isNew()) {
      $node = $contexts['node']->getContextValue();
      $state = $this->moderationInformation->getOriginalState($node);

      $build = [
        'status' => [
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attributes' => ['class' => ['idea-statistics-and-status-block--status']],
          '#value' => $state->label(),
        ],
        '#cache' => [
          'tags' => $node->getCacheTags(),
        ],
      ];
    }

    return $build;
  }

}
