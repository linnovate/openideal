<?php

namespace Drupal\openideal_idea\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Entity\EntityReferenceSelection\SelectionPluginManagerInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Request;

/**
 * Ideas autocomplete controller.
 */
class IdeasAutocompleteController extends ControllerBase {

  /**
   * Limit autocomplete results.
   */
  const LIMIT = 10;

  /**
   * The entity reference selection handler plugin manager.
   *
   * @var \Drupal\Core\Entity\EntityReferenceSelection\SelectionPluginManagerInterface
   */
  protected $selectionManager;

  /**
   * Constructs a IdeasAutocompleteController object.
   *
   * @param \Drupal\Core\Entity\EntityReferenceSelection\SelectionPluginManagerInterface $selection_manager
   *   The entity reference selection handler plugin manager.
   */
  public function __construct(SelectionPluginManagerInterface $selection_manager) {
    $this->selectionManager = $selection_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('plugin.manager.entity_reference_selection')
    );
  }

  /**
   * Retrieves suggestions for ides duplication.
   *
   * @param \Symfony\Component\HttpFoundation\Request $request
   *   The current request.
   *
   * @return \Symfony\Component\HttpFoundation\JsonResponse
   *   A JSON response containing autocomplete suggestions.
   */
  public function autocomplete(Request $request) {
    $q = $request->query->get('q');
    $matches = [];

    if (!isset($q)) {
      return new JsonResponse($matches);
    }

    $matches = [];

    $selection_settings = [
      'view' =>
        [
          'view_name' => 'ideas',
          'display_name' => 'ideas_autocomplete_entity_reference',
          'arguments' =>
            [],
        ],
      'match_operator' => 'CONTAINS',
      'match_limit' => self::LIMIT,
      'target_type' => 'node',
      'handler' => 'views',
    ];

    // Need to user selection manager instead of entity.autocomplete_matcher
    // because of in such way we have more control
    // and can set url for the redirection.
    $handler = $this->selectionManager->getInstance($selection_settings);

    $entity_labels = $handler->getReferenceableEntities($q, 'CONTAINS', self::LIMIT);

    foreach ($entity_labels as $values) {
      foreach ($values as $entity_id => $label) {
        $node_url = Url::fromRoute('entity.node.canonical', ['node' => $entity_id], ['query' => ['suggested-idea' => TRUE]])->toString();
        $matches[] = ['value' => $label, 'label' => $label, 'url' => $node_url];
      }
    }

    return new JsonResponse($matches);
  }

}
