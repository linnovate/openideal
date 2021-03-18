<?php

namespace Drupal\openideal_rest;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\Core\DependencyInjection\DependencySerializationTrait;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Process webhooks setting form.
 *
 * Adds ability to add plugins on-fly for the webhook.
 */
class WebhooksFormProcessor implements ContainerInjectionInterface {

  use DependencySerializationTrait;
  use StringTranslationTrait;

  /**
   * Config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * Constructs WebhooksFormProcessor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $configFactory
   *   Config factory.
   */
  public function __construct(ConfigFactoryInterface $configFactory) {
    $this->configFactory = $configFactory;
  }

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('config.factory'));
  }

  /**
   * Alter form.
   *
   * @param array $form
   *   Form.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   Form state.
   */
  public function alter(array &$form, FormStateInterface $form_state) {
    $plugins = $this->getPlugins($form_state);

    $form['plugins'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Plugins'),
      '#description' => $this->t('Each plugin should have unique name'),
      '#tree' => TRUE,
      '#prefix' => '<div id="webook-plugins">',
      '#suffix' => '</div>',
    ];

    $form['add_more'] = [
      '#type' => 'button',
      '#name' => 'add_more',
      '#value' => $this->t('Add another item'),
      '#add_more' => TRUE,
      '#ajax' => [
        'callback' => self::class . '::ajaxCallback',
        'wrapper' => 'webook-plugins',
      ],
    ];

    // Building the multiple form element. Adding first the the form existing
    // plugins.
    $start_key = 0;
    foreach ($plugins as $key => $plugin) {
      $form['plugins'][$start_key] = [
        '#type' => 'textfield',
        '#default_value' => $plugin,
      ];

      // Dirty disable plugins to make sure that the user won't delete them.
      // @todo Disable them on nicer way.
      if (in_array($plugin, ['Slack', 'Other'])) {
        $form['plugins'][$start_key]['#disabled'] = TRUE;
      }

      $start_key++;
    }

    // Increase number of elements if requested, or none exist.
    $trigger_element = $form_state->getTriggeringElement();
    if (!empty($trigger_element['#add_more']) || !$start_key) {
      $form['plugins'][] = [
        '#type' => 'textfield',
        '#default_value' => '',
      ];

    }

    $form['#submit'][] = [$this, 'submitForm'];
    $form['#validate'][] = [$this, 'validateForm'];
  }

  /**
   * Checks if plugin names are unique.
   *
   * @param array $form
   *   Form.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   Form state.
   */
  public function validateForm(array $form, FormStateInterface $form_state) {
    $plugins = $form_state->getValue('plugins');

    // @todo Make method for this.
    $plugins = array_filter($plugins, function ($plugin) {
      return trim($plugin) != '';
    });

    if (($unique = array_unique($plugins)) && !empty(array_diff_key($plugins, $unique))) {
      $form_state->setErrorByName('', $this->t('Plugins should have unique names.'));
    }
  }

  /**
   * Submit form.
   *
   * @param array $form
   *   Form.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   Form state.
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValue('plugins');

    // Remove empty values.
    $values = array_filter($values, function ($plugin) {
      return trim($plugin) != '';
    });

    // Set key as machine name on plugin label.
    $final_result = [];
    array_walk($values, function ($plugin) use (&$final_result) {
      $key = preg_replace('/[^a-z0-9_]+/', '_', strtolower($plugin));
      $final_result[$key] = $plugin;
    });

    $this->configFactory->getEditable('webhooks.settings')
      ->set('plugins', $final_result)
      ->save();
  }

  /**
   * Get plugins list.
   *
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   Form state.
   *
   * @return array
   *   Plugins.
   */
  protected function getPlugins(FormStateInterface $form_state) {
    // Get values from state, if not set then take from config factory.
    $values = $form_state->getvalue('plugins') ? array_filter($form_state->getvalue('plugins'))
      : $this->configFactory->get('webhooks.settings')->get('plugins');

    if (empty($values)) {
      return [];
    }

    $storage = $form_state->getStorage();

    foreach ($values as $value) {
      $storage[$value] = $value;
    }

    // Save values to storage.
    $form_state->setStorage($storage);

    return $storage;
  }

  /**
   * Ajax callback.
   *
   * @param array $form
   *   Form.
   *
   * @return array
   *   Randarable array.
   */
  public function ajaxCallback(array $form) {
    return $form['plugins'];
  }

}
