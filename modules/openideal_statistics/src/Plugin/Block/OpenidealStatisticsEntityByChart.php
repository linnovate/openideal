<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Component\Serialization\Json;
use Drupal\content_moderation\ModerationInformation;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'OpenidealStatisticsEntityByChart' block.
 *
 * @Block(
 *  id = "openideal_statistics_etity_by_chart_block",
 *  admin_label = @Translation("Entity by chart"),
 * )
 */
class OpenidealStatisticsEntityByChart extends BlockBase implements ContainerFactoryPluginInterface {

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
   * Moderation information.
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
    Json $json,
    ModerationInformation $moderationInformation
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
    $this->serializer = $json;
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
      $container->get('serialization.json'),
      $container->get('content_moderation.moderation_information')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $storage = $this->entityTypeManager->getStorage('node');
    $entity_query = $storage->getQuery();
    $query = $entity_query->condition('type', 'idea');
    $entity_by = $this->configuration['entity_by'];

    if ($entity_by == 'category') {
      $query->exists('field_category');
    }

    $ids = $query->execute();

    $ideas = $storage->loadMultiple($ids);

    $data = [];
    /** @var \Drupal\node\NodeInterface $idea */
    foreach ($ideas as $idea) {
      if ($entity_by == 'category') {
        $label = $idea->field_category->first()->entity->label();
      }
      else {
        $state = $this->moderationInformation->getOriginalState($idea);
        $label = $state->label();
      }
      $data[$label] = ($data[$label] ?? 0) + 1;
    }

    $data = $this->serializer->encode($data);
    $build['#attached']['drupalSettings']['charts']['byEntity'][$entity_by] = [
      'data' => $data,
      'bindTo' => '#entity-by-' . $entity_by,
    ];
    $build['#attached']['library'][] = 'openideal_statistics/openideal_statistics.charts';
    $build['#cache']['tags'] = ['node_list:idea'];

    $build[] = [
      '#type' => 'container',
      '#attributes' => ['id' => 'entity-by-' . $entity_by],
    ];

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['entity_by'] = [
      '#type' => 'select',
      '#options' => [
        'category' => $this->t('Category'),
        'status' => $this->t('By status'),
      ],
      '#default_value' => $this->configuration['entity_by'] ?? '',
      '#required' => TRUE,
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['entity_by'] = $form_state->getValue('entity_by');
  }

}
