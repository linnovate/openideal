<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Component\Datetime\Time;
use Drupal\Component\Serialization\Json;
use Drupal\Core\Database\Connection;
use Drupal\Core\Datetime\DateFormatter;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\openideal_statistics\Form\OpenidealStatisticsDateSelectForm;
use Drupal\openideal_statistics\OpenidealStatisticsFilterTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'OpenidealStatisticsByCategoryCharts' block.
 *
 * @Block(
 *  id = "openideal_statistics_per_day_charts_block",
 *  admin_label = @Translation("Charts per day"),
 * )
 *
 * @group openideal_charts
 */
class OpenidealStatisticsPerDayCharts extends BlockBase implements ContainerFactoryPluginInterface {

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
   * Date formatter.
   *
   * @var \Drupal\Core\Datetime\DateFormatter
   */
  protected $dateFormatter;

  /**
   * Database connection.
   *
   * @var \Drupal\Core\Database\Connection
   */
  protected $database;

  /**
   * Max total value.
   *
   * @var int
   */
  protected $max = 0;

  /**
   * Time.
   *
   * @var \Drupal\Component\Datetime\Time
   */
  protected $time;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EntityTypeManager $entityTypeManager,
    Json $json,
    DateFormatter $dateFormatter,
    Connection $database,
    Time $time
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
    $this->serializer = $json;
    $this->dateFormatter = $dateFormatter;
    $this->database = $database;
    $this->time = $time;
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
      $container->get('date.formatter'),
      $container->get('database'),
      $container->get('datetime.time')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $entity = $this->configuration['entity'];
    $data = $this->getData();

    $data = $this->serializer->encode($data);
    $build['#attached']['drupalSettings']['charts']['perDay'][$entity] = [
      'data' => $data,
      'bindTo' => '#per-day-' . $entity,
      'label' => $this->t("No. of @entity", ['@entity' => ($entity != 'node' ? $entity : 'ideas') . 's']),
      'max' => $this->max,
    ];
    $build['#cache']['tags'] = [$entity . '_list' . ($entity == 'node' ? ':idea' : '')];
    $build['#cache']['contexts'] = ['url.query_args'];

    $build[] = [
      '#type' => 'container',
      '#attributes' => [
        'class' => ['per-day-' . $entity],
        'id' => 'per-day-' . $entity,
      ],
    ];

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['entity'] = [
      '#type' => 'select',
      '#options' => [
        'user' => $this->t('Users'),
        'vote' => $this->t('Votes'),
        'comment' => $this->t('Comments'),
        'node' => $this->t('Ideas'),
      ],
      '#default_value' => $this->configuration['entity'] ?? '',
      '#required' => TRUE,
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['entity'] = $form_state->getValue('entity');
  }

  /**
   * Get data for chart.
   *
   * @return array
   *   Data.
   */
  protected function getData() {
    $entity = $this->configuration['entity'];
    $created = $entity == 'vote' ? 'timestamp' : 'created';
    // To loop through dates can't use request time because there is
    // difference of time during request executing.
    $to = time();
    $from = strtotime('-1 month');

    if ($filters = $this->getFilters()) {
      $to = $filters[OpenidealStatisticsDateSelectForm::TO] ?? $to;
      $from = $filters[OpenidealStatisticsDateSelectForm::FROM] ?? $from;
    }

    // @Todo: investigate if there is possibility to count occurrences of equal fields.
    $select = $this->database->select($this->getTable(), 'd');
    $select->addExpression("DATE_FORMAT(FROM_UNIXTIME(d.${created}), '%Y-%m-%d')", 'date');
    $select->condition("d.${created}", $this->time->getRequestTime() - $from, '>=');

    if ($entity == 'node') {
      $select->condition('d.type', 'idea');
    }

    $result = $select->execute()->fetchCol();

    $fetched_summarized_dates = [];
    foreach ($result as $item) {
      $fetched_summarized_dates[$item] = ($fetched_summarized_dates[$item] ?? 0) + 1;
    }
    // Need to get max value to set it properly in charts script.
    $this->max = max((empty($fetched_summarized_dates) ? [0] : $fetched_summarized_dates));

    $data = [];
    // Loop through month dates and set appropriate values.
    while ($from <= $to) {
      $from_date_formatted = $this->dateFormatter->format($from, 'html_date');
      $data[] = [
        'date' => $from_date_formatted,
        'total' => $fetched_summarized_dates[$from_date_formatted] ?? 0,
      ];
      $from = strtotime('+1 day', $from);
    }
    return $data;
  }

  /**
   * Get table name.
   *
   * @return string
   *   Table name.
   */
  private function getTable() {
    $entity = $this->configuration['entity'];
    switch ($entity) {
      case 'user':
        return 'users_field_data';

      case 'vote':
        return 'votingapi_vote';

      default:
        return $entity . '_field_data';
    }
  }

}
