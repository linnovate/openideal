<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Datetime\DateFormatter;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Session\AccountProxy;
use Drupal\datetime\Plugin\Field\FieldType\DateTimeItem;
use Drupal\node\NodeInterface;
use Drupal\openideal_challenge\OpenidealContextEntityTrait;
use Drupal\openideal_idea\OpenidealHelper;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Node info' block.
 *
 * @Block(
 *  id = "openideal_idea_info_block",
 *  admin_label = @Translation("Node info"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealIdeaUpdateInfo extends BlockBase implements ContainerFactoryPluginInterface {

  use OpenidealContextEntityTrait;

  /**
   * Date formatter.
   *
   * @var \Drupal\Core\Datetime\DateFormatter
   */
  protected $dateFormatter;

  /**
   * Openideal helper.
   *
   * @var \Drupal\openideal_idea\OpenidealHelper
   */
  protected $helper;

  /**
   * Current user.
   *
   * @var \Drupal\Core\Session\AccountProxy
   */
  protected $currentUser;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    DateFormatter $date_formatter,
    OpenidealHelper $helper,
    AccountProxy $currentUser
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->dateFormatter = $date_formatter;
    $this->helper = $helper;
    $this->currentUser = $currentUser;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('date.formatter'),
      $container->get('openideal_idea.helper'),
      $container->get('current_user')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [];
    if ($node = $this->getEntity($this->getContexts())) {
      $build = ['#theme' => 'openideal_idea_info_block'];
      $created = $this->dateFormatter->format($node->getCreatedTime(), 'openideal_date');
      $changed = $this->dateFormatter->format($node->getChangedTime(), 'openideal_date');
      if ($node->bundle() == 'challenge') {
        $status = $this->getChallengeStatus($node) + ['access' => $this->configuration['use_schedule']];
        $build['#content']['challenge_status'] = $status;
      }

      $build['#content']['created'] = [
        'value' => $created,
        'title' => $this->t('Created'),
        'access' => $this->configuration['use_created'],
      ];
      $build['#content']['changed'] = [
        'value' => $changed,
        'title' => $this->t('Changed'),
        'access' => $this->configuration['use_updated'],
      ];

      $member = $this->helper->getGroupMember($this->currentUser, $node);
      if ($member && $member->hasPermission('update any group_node:idea entity')) {
        $link = Link::createFromRoute($this->t('Edit'), 'entity.node.edit_form', ['node' => $node->id()])->toString();
        $build['#content']['edit'] = $link;
      }
      $build['#cache']['tags'] = $node->getCacheTags();
      $build['#cache']['contexts'][] = 'route';
    }

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['node_dates_info'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Toggle node dates info elements'),
      '#description' => $this->t('Choose which dates elements you want to show in this block instance.'),
    ];
    $form['node_dates_info']['use_created'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Created'),
      '#description' => $this->t('Node created information'),
      '#default_value' => $this->configuration['use_created'],
    ];

    $form['node_dates_info']['use_updated'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Site name'),
      '#description' => $this->t('Updated'),
      '#default_value' => $this->configuration['use_updated'],
    ];
    $form['node_dates_info']['use_schedule'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Challenge status'),
      '#description' => $this->t('Challenge schedule status (only for challenge)'),
      '#default_value' => $this->configuration['use_schedule'],
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $block_branding = $form_state->getValue('node_dates_info');
    $this->configuration['use_created'] = $block_branding['use_created'];
    $this->configuration['use_updated'] = $block_branding['use_updated'];
    $this->configuration['use_schedule'] = $block_branding['use_schedule'];
  }

  /**
   * {@inheritDoc}
   */
  public function defaultConfiguration() {
    return [
      'use_created' => TRUE,
      'use_updated' => TRUE,
      'use_schedule' => FALSE,
    ] + parent::defaultConfiguration();
  }

  /**
   * Get Challenge status.
   *
   * @param \Drupal\node\NodeInterface $node
   *   Challenge.
   *
   * @return array|array[]
   *   Randarable array.
   */
  protected function getChallengeStatus(NodeInterface $node) {
    $settings = [
      'label' => 'hidden',
      'settings' => [
        'datetime_type' => DateTimeItem::DATETIME_TYPE_DATETIME,
        'date_format' => 'custom',
        'custom_date_format' => 'd/m/Y',
      ],
    ];
    $is_open = $node->field_is_open->value;
    if ($is_open && !$node->field_schedule_close->isEmpty()) {
      $view = $node->field_schedule_close->view($settings);
      $view['#attributes']['class'][] = 'challenge-status--deadline';
      return [
        'title' => $this->t('Challenge deadline'),
        'value' => $view,
      ];
    }
    elseif (!$is_open && !$node->field_schedule_open->isEmpty()) {
      $view = $node->field_schedule_open->view($settings);
      $view['#attributes']['class'][] = 'challenge_status--opening';

      return [
        'title' => $this->t('Challenge opening'),
        'value' => $view,
      ];
    }
    else {
      $value = $is_open ? $this->t('Open') : $this->t('Close');
      return [
        'title' => $this->t('Challenge status'),
        'value' => $value,
      ];

    }
  }

}
