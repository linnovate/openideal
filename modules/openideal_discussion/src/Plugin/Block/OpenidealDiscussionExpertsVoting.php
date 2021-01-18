<?php

namespace Drupal\openideal_discussion\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityFieldManager;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Routing\CurrentRouteMatch;
use Drupal\field\Entity\FieldConfig;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides experts voting block for discussion page.
 *
 * @Block(
 *   id = "openideal_discussion_experts_voting",
 *   admin_label = @Translation("Experts voting widget")
 * )
 */
class OpenidealDiscussionExpertsVoting extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * Route match.
   *
   * @var \Drupal\Core\Routing\CurrentRouteMatch
   */
  protected $routeMatch;

  /**
   * Entity field manager.
   *
   * @var \Drupal\Core\Entity\EntityFieldManager
   */
  protected $fieldManager;

  /**
   * {@inheritDoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, CurrentRouteMatch $routeMatch, EntityFieldManager $fieldManager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->routeMatch = $routeMatch;
    $this->fieldManager = $fieldManager;
  }

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_route_match'),
      $container->get('entity_field.manager')
    );
  }

  /**
   * {@inheritDoc}
   */
  public function build() {
    $build = [];
    /** @var \Drupal\node\NodeInterface $node */
    if (($node = $this->routeMatch->getParameter('node')) && $node->bundle() === 'discussion' && !$node->get('field_idea')->isEmpty()) {
      $build['#cache']['contexts'] = ['user.roles', 'user.permissions'];
      $build['#cache']['tags'] = $node->getCacheTags();
      /** @var \Drupal\node\Entity\Node $idea */
      $idea = $node->get('field_idea')->first()->get('entity')->getTarget()->getValue();
      $settings = [
        'label' => 'inline',
        'settings' => [
          'show_results' => '1',
          'style' => 'fontawesome-stars',
        ],
      ];
      $fields = $idea->getFieldDefinitions();
      foreach ($fields as $field_name => $field_definition) {
        if ($field_definition instanceof FieldConfig && $field_definition->getType() == 'voting_api_field') {
          $build['content'][] = $idea->{$field_name}->view($settings);
        }
      }
    }
    return $build;
  }

}
