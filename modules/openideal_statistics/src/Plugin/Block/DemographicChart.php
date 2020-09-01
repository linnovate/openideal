<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Component\Serialization\Json;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'DemographicChart' block.
 *
 * @Block(
 *  id = "openideal_statistics_charts_block",
 *  admin_label = @Translation("Charts block"),
 * )
 */
class DemographicChart extends BlockBase implements ContainerFactoryPluginInterface {

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
    $storage = $this->entityTypeManager->getStorage('user');
    $user_query = $storage->getQuery();
    $ids = $user_query->exists('field_gender')
      ->exists('field_age_group')
      ->condition('status', '1')
      ->execute();

    $users = $storage->loadMultiple($ids);

    $data = [];
    foreach ($users as $user) {
      $data[] = [
        'gender' => $user->field_gender->value,
        'age' => $user->field_age_group->value,
      ];
    }

    $data = $this->serializer->encode($data);
    $build['#attached']['drupalSettings']['charts']['data'] = $data;
    $build['#attached']['library'][] = 'openideal_statistics/openideal_statistics.charts';
    $build['#cache']['tags'] = ['user_list'];

    $build[] = [
      '#type' => 'container',
      '#attributes' => ['class' => ['charts']],
    ];

    return $build;
  }

}
