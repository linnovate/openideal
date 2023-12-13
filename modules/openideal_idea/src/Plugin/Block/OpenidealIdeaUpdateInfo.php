<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Datetime\DateFormatter;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Session\AccountProxy;
use Drupal\datetime\Plugin\Field\FieldType\DateTimeItem;
use Drupal\group\Access\GroupPermissionChecker;
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
 *   context_definitions = {
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
   * Group permission checker.
   *
   * @var \Drupal\group\Access\GroupPermissionChecker
   */
  protected $groupPermissionChecker;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    DateFormatter $date_formatter,
    OpenidealHelper $helper,
    AccountProxy $currentUser,
    GroupPermissionChecker $groupPermissionChecker
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->dateFormatter = $date_formatter;
    $this->helper = $helper;
    $this->currentUser = $currentUser;
    $this->groupPermissionChecker = $groupPermissionChecker;
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
      $container->get('current_user'),
      $container->get('group_permission.checker')
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
        'title' => $this->t('Posted on'),
        'access' => $this->configuration['use_created'],
      ];
      $build['#content']['changed'] = [
        'value' => $changed,
        'title' => $this->t('Recently updated'),
        'access' => $this->configuration['use_updated'],
      ];

      if ($node->access('update', $this->currentUser)) {
        $link = Link::createFromRoute($this->t('Edit'), 'entity.node.edit_form', ['node' => $node->id()])->toString()->getGeneratedLink();
        $build['#content']['edit'] = [
          'value' => $link,
          'title' => $this->t('Actions'),
          'access' => $this->configuration['use_edit'],
        ];
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
      '#description' => $this->t('Choose which elements you want to show in this block instance.'),
    ];
    $form['node_dates_info']['use_created'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Created'),
      '#description' => $this->t('Node created information'),
      '#default_value' => $this->configuration['use_created'],
    ];

    $form['node_dates_info']['use_updated'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Updated'),
      '#description' => $this->t('Node last modification'),
      '#default_value' => $this->configuration['use_updated'],
    ];
    $form['node_dates_info']['use_schedule'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Challenge status'),
      '#description' => $this->t('Challenge schedule status (only for challenge)'),
      '#default_value' => $this->configuration['use_schedule'],
    ];
    $form['node_dates_info']['use_edit'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Edit node link'),
      '#default_value' => $this->configuration['use_edit'],
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
    $this->configuration['use_edit'] = $block_branding['use_edit'];
  }

  /**
   * {@inheritDoc}
   */
  public function defaultConfiguration() {
    return [
      'use_created' => TRUE,
      'use_updated' => TRUE,
      'use_schedule' => FALSE,
      'use_edit' => FALSE,
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
