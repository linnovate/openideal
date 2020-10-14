<?php

namespace Drupal\openideal_idea\Controller;

use Drupal\Component\Utility\Html;
use Drupal\Component\Utility\Tags;
use Drupal\Core\Controller\ControllerBase;
use Drupal\views\ViewExecutableFactory;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class AutocompleteController.
 */
class AutocompleteController extends ControllerBase {

  /**
   * Drupal\Core\Entity\EntityTypeManagerInterface definition.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Views executable factory.
   *
   * @var \Drupal\views\ViewExecutableFactory
   */
  private $executableFactory;

  /**
   * Constructs a new AutocompleteController object.
   *
   * @param \Drupal\views\ViewExecutableFactory $executableFactory
   *   Executable factory.
   */
  public function __construct(ViewExecutableFactory $executableFactory) {
    $this->entityTypeManager();
    $this->executableFactory = $executableFactory;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('views.executable')
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
    /** @var \Drupal\views\Entity\View $view */
    $view = $this->entityTypeManager->getStorage('view')->load('ideas');
    if (!$view) {
      $this->getLogger('php')->error($this->t("Views not found"));
      return new JsonResponse();
    }
    $executable = $this->executableFactory->get($view);

    $executable->setDisplay('ideas_autocomplete_entity_reference');
    $executable->setExposedInput([
      'body' => $q,
      'title' => $q,
    ]);
    $executable->execute();
    $result = $executable->result;

    $matches = [];
    foreach ($result as $item) {
      $id = $item->_entity->id();
      $label = $item->_entity->label();
      $key = "$label ($id)";
      // Strip things like starting/trailing white spaces, line breaks and
      // tags.
      $key = preg_replace('/\s\s+/', ' ', str_replace("\n", '', trim(Html::decodeEntities(strip_tags($key)))));
      // Names containing commas or quotes must be wrapped in quotes.
      $key = Tags::encode($key);

      $matches[] = ['value' => $key, 'label' => $label];
    }
    return new JsonResponse($matches);
  }

}
