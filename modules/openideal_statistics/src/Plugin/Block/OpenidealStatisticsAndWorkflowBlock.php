<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockManager;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'OpenidealStatisticsAndWorkflowBlock' block.
 *
 * @Block(
 *  id = "openideal_statistics_and_status",
 *  admin_label = @Translation("Statistics and status block"),
 *   context_definitions = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealStatisticsAndWorkflowBlock extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * Block manager.
   *
   * @var \Drupal\Core\Block\BlockManager
   */
  protected $blockManager;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    BlockManager $block_manager
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->blockManager = $block_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('plugin.manager.block')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $contexts = $this->getContexts();
    $build = [];
    if (isset($contexts['node'])
      && !$contexts['node']->getContextValue()->isNew()
      && isset($contexts['view_mode'])) {
      $node = $contexts['node'];
      $statistics_block = $this->blockManager->createInstance('openideal_statistics_idea_statistics');
      $statistics_block->setContext('node', $node);
      $statistics_block->setContext('view_mode', $contexts['view_mode']);
      $status = $this->blockManager->createInstance('openideal_statistics_status');
      $status->setContext('node', $node);
      $build = [
        '#type' => 'container',
        '#attributes' => ['class' => ['idea-statistics-and-status-block']],
        'statistics' => $statistics_block->build(),
        'status' => $status->build(),
        '#cache' => [
          'tags' => $node->getContextValue()->getCacheTags(),
        ],
      ];
    }

    return $build;
  }

}
