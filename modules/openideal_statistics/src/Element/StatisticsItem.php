<?php

namespace Drupal\openideal_statistics\Element;

use Drupal\Core\Render\Element\RenderElement;
use Drupal\Core\Template\Attribute;

/**
 * Provides a statistics_item render element.
 *
 * @RenderElement("statistics_item")
 */
class StatisticsItem extends RenderElement {

  /**
   * {@inheritdoc}
   */
  public function getInfo() {
    return [
      '#theme' => 'statistics_item',
      '#show_title' => TRUE,

      // These three variable are replaced/inserted into appropriate places in
      // self::preRenderStatisticsItem().
      '#img_class' => NULL,
      '#lazy_element' => NULL,
      '#item_title' => NULL,

      '#img' => [
        'attributes' => [
          'class' => [
            'item-svg',
            // Add.
            'idea-statistics-block--list__item--',
          ],
        ],
      ],
      '#bottom' => [
        'attributes' => ['class' => ['idea-statistics-block--list__item--bottom']],
        'count' => [
          'attributes' => ['class' => ['idea-statistics-block--list__item--count']],
          'value' => NULL,
        ],
        'title' => [
          'value' => NULL,
          'attributes' => ['class' => ['idea-statistics-block--list__item--title']],
        ],
      ],
      '#pre_render' => [
        [self::class, 'preRenderStatisticsItem'],
      ],
    ];
  }

  /**
   * Pre-render callback.
   *
   * @param array $element
   *   The renderable array representing the element.
   *
   * @return array
   *   The passed in element with changes made to attributes depending on
   *   context.
   */
  public static function preRenderStatisticsItem(array $element) {
    // Set/configure/prepare attributes.
    if ($key = array_search(
        'idea-statistics-block--list__item--',
        $element['#img']['attributes']['class']
      ) && isset($element['#img_class'])) {
      $element['#img']['attributes']['class'][$key] .= $element['#img_class'];
      $element['#bottom']['count']['attributes']['data-openideal-vote'] = $element['#img_class'];
      unset($element['#img_class']);
    }

    if (isset($element['#lazy_element'])) {
      $element['#bottom']['count']['value'] = $element['#lazy_element'];
      unset($element['#lazy_element']);
    }

    if (isset($element['#item_title'])) {
      $element['#img']['attributes']['title'] = $element['#item_title'];
      $element['#bottom']['title']['value'] = $element['#item_title'];
      unset($element['#item_title']);
    }

    $element['#img']['attributes'] = new Attribute($element['#img']['attributes']);
    $element['#bottom']['attributes'] = new Attribute($element['#bottom']['attributes']);
    $element['#bottom']['count']['attributes'] = new Attribute($element['#bottom']['count']['attributes']);
    $element['#bottom']['title']['attributes'] = new Attribute($element['#bottom']['title']['attributes']);
    return $element;
  }

}
