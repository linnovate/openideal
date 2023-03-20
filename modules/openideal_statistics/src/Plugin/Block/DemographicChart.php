<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Component\Serialization\Json;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\openideal_statistics\Form\OpenidealStatisticsDateSelectForm;
use Drupal\openideal_statistics\OpenidealStatisticsFilterTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'DemographicChart' block.
 *
 * @Block(
 *  id = "openideal_statistics_charts_block",
 *  admin_label = @Translation("Charts block"),
 * )
 *
 * @group openideal_charts
 */
class DemographicChart extends BlockBase implements ContainerFactoryPluginInterface {

  use OpenidealStatisticsFilterTrait;

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManager
   */
  protected $entityTypeManager;

  /**
   * Json serialization service.
   *
   * @var \Drupal\Component\Serialization\Json
   */
  protected $serializer;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EntityTypeManager $entityTypeManager,
    Json $json
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
    $this->serializer = $json;
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
      $container->get('serialization.json')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $data = $this->serializer->encode($this->getData());
    $build['#attached']['drupalSettings']['charts']['data'] = $data;
    $build['#attached']['library'][] = 'openideal_statistics/openideal_statistics.charts';
    $build['#cache']['contexts'] = ['url.query_args'];

    $build[] = [
      '#type' => 'container',
      '#attributes' => ['class' => ['charts']],
    ];

    return $build;
  }

  /**
   * Get data for chart.
   *
   * @return array
   *   Data.
   */
  private function getData() {
    $storage = $this->entityTypeManager->getStorage('user');
    $query = $storage->getQuery();
    $query->exists('field_gender')
      ->exists('field_age_group')
      ->accessCheck(TRUE)
      ->condition('status', '1');

    $filters = $this->getFilters();
    if (isset($filters[OpenidealStatisticsDateSelectForm::TO])) {
      $query->condition('created', $filters[OpenidealStatisticsDateSelectForm::TO], '<=');
    }
    if (isset($filters[OpenidealStatisticsDateSelectForm::FROM])) {
      $query->condition('created', $filters[OpenidealStatisticsDateSelectForm::FROM], '>=');
    }

    $ids = $query->execute();
    $users = $storage->loadMultiple($ids);

    $data = [];
    foreach ($users as $user) {
      $data[] = [
        'gender' => $user->field_gender->value,
        'age' => $user->field_age_group->value,
      ];
    }

    return $data;
  }

}
