<?php

namespace Drupal\openideal_slideshow\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'Slideshow' block.
 *
 * @Block(
 *   id = "openidel_slideshow_block",
 *   admin_label = @Translation("Slideshow"),
 *   category = @Translation("Openideal"),
 *   context_definitions = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class Slideshow extends BlockBase {

  /**
   * {@inheritDoc}
   */
  public function build() {
    $contexts = $this->getContexts();
    $images = $this->getImages();
    $tags = isset($contexts['node']) ? $contexts['node']->getContextValue()->getCacheTags() : [];
    return empty($images)
      ? []
      : [
        '#theme' => 'openideal_slideshow',
        '#items' => $images,
        '#image_style' => $this->configuration['image_style'],
        '#attached' => [
          'library' => ['openideal_slideshow/openideal_slideshow.carousel'],
        ],
        '#cache' => [
          'tags' => $tags,
        ],
      ];
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $image_styles = image_style_options(TRUE);

    $form['image_style'] = [
      '#type' => 'select',
      '#title' => $this->t('Image styles'),
      '#options' => $image_styles,
      '#default_value' => $this->configuration['image_style'],
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['image_style'] = $form_state->getValue('image_style');
  }

  /**
   * {@inheritDoc}
   */
  public function defaultConfiguration() {
    return [
      'image_style' => '',
    ];
  }

  /**
   * Get the images from node.
   */
  protected function getImages() {
    $contexts = $this->getContexts();
    $result = [];

    if (!empty($contexts['node']) && $contexts['node']->getContextValue()->isNew()) {
      return $result;
    }

    $node = $contexts['node']->getContextValue();
    if ($node->bundle() == 'challenge' && !$node->field_main_image->isEmpty()) {
      $file = $node->field_main_image->first();
      $entity = $file->entity;
      $entity->_referringItem = $file;
      $result[] = $entity;
    }

    /** @var \Drupal\file\Plugin\Field\FieldType\FileFieldItemList  $images */
    $images = $node->field_images;
    foreach ($images as $image) {
      $entity = $image->entity;
      $entity->_referringItem = $image;
      $result[] = $entity;
    }

    return $result;
  }

}
