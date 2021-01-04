<?php

namespace Drupal\openideal_theme\Plugin\Layout;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Layout\LayoutDefault;

/**
 * OpenidealBEMLayout class of layouts with two columns.
 */
class OpenidealBEMLayout extends LayoutDefault {

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $configuration = parent::defaultConfiguration();
    return $configuration + [
      'main_class' => 'two-columns-flexible',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['main_class'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Main class for the template'),
      '#default_value' => $this->configuration['main_class'],
    ];
    return parent::buildConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);
    $this->configuration['main_class'] = $form_state->getValue('main_class');
  }

  /**
   * {@inheritdoc}
   */
  public function build(array $regions) {
    $build = parent::build($regions);
    $main_class = $this->configuration['main_class'];
    $build['#attributes']['class'][] = $main_class;

    foreach ($this->getPluginDefinition()->getRegions() as $region => $info) {
      $build[$region]['#attributes']['class'][] = $main_class . '--section__' . $region;
    }
    return $build;
  }

}
