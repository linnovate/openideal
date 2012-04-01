<?php

interface DrupalMetaTagInterface {

  /**
   * Constructor
   *
   * @param array $info
   *   The information about the meta tag from metatag_get_info().
   */
  function __construct(array $info, array $data = array());

  function getForm();

  //function validateForm();

  //function processForm();

  function getValue();

  function getElement();
}

class DrupalDefaultMetaTag implements DrupalMetaTagInterface {

  protected $info;
  protected $data = array('value' => '');

  function __construct(array $info, array $data = NULL) {
    $this->info = $info;
    if (isset($data)) {
      $this->data = $data;
    }
  }

  public function getForm(array $options = array()) {
    return array();
  }

  public function getValue(array $options = array()) {
    return $this->data['value'];
  }

  public function getElement(array $options = array()) {
    $element = isset($this->info['element']) ? $this->info['element'] : array();

    $value = $this->getValue($options);
    if (strlen($value) === 0) {
      return array();
    }

    $element += array(
      '#theme' => 'metatag',
      '#tag' => 'meta',
      '#id' => 'metatag_' . $this->info['name'],
      '#name' => $this->info['name'],
      '#value' => $value,
    );

    // Add header information if desired.
    if (!empty($this->info['header'])) {
      $element['#attached']['drupal_add_http_header'][] = array($this->info['header'], $value);
    }

    return array(
      '#attached' => array('drupal_add_html_head' => array(array($element, $element['#id']))),
    );
    return $element;
  }
}

/**
 * Text-based meta tag controller.
 */
class DrupalTextMetaTag extends DrupalDefaultMetaTag {

  public function getForm(array $options = array()) {
    $options += array(
      'token types' => array(),
    );

    $form['value'] = isset($this->info['form']) ? $this->info['form'] : array();

    $form['value'] += array(
      '#type' => 'textfield',
      '#title' => $this->info['label'],
      '#description' => !empty($this->info['description']) ? $this->info['description'] : '',
      '#default_value' => isset($this->data['value']) ? $this->data['value'] : '',
      '#element_validate' => array('token_element_validate'),
      '#token_types' => $options['token types'],
      '#maxlength' => 255,
    );

    return $form;
  }

  public function getValue(array $options = array()) {
    $options += array(
      'token data' => array(),
      'clear' => TRUE,
      'sanitize' => FALSE,
      'raw' => FALSE,
    );

    $value = $this->data['value'];
    if (empty($options['raw'])) {
      $value = token_replace($value, $options['token data'], $options);
    }
    $value = strip_tags(decode_entities($value));
    $value = trim($value);
    return $value;
  }
}

/**
 * Link type meta tag controller.
 */
class DrupalLinkMetaTag extends DrupalTextMetaTag {

  public function getElement(array $options = array()) {
    $element = isset($this->info['element']) ? $this->info['element'] : array();

    $value = $this->getValue($options);
    if (strlen($value) === 0) {
      return array();
    }

    $element += array(
      '#theme' => 'metatag_link_rel',
      '#tag' => 'link',
      '#id' => 'metatag_' . $this->info['name'],
      '#name' => $this->info['name'],
      '#value' => $value,
    );

    if (!isset($this->info['header']) || !empty($this->info['header'])) {
      // Also send the generator in the HTTP header.
      // @todo This does not support 'rev' or alternate link headers.
      $element['#attached']['drupal_add_http_header'][] = array('Link', '<' . check_plain($value) . '>;' . drupal_http_header_attributes(array('rel' => $element['#name'])), TRUE);
    }

    return array(
      '#attached' => array('drupal_add_html_head' => array(array($element, $element['#id']))),
    );
    return $element;
  }
}

/**
 * Title meta tag controller.
 *
 * This extends DrupalTextMetaTag as we need to alter variables in
 * template_preprocess_html() rather output a normal meta tag.
 */
class DrupalTitleMetaTag extends DrupalTextMetaTag {

  public function getElement(array $options = array()) {
    $element = array();
    $value = check_plain($this->getValue($options));
    $element['#attached']['metatag_set_preprocess_variable'][] = array('html', 'head_title', $value);
    $element['#attached']['metatag_set_preprocess_variable'][] = array('html', 'head_array', array('title' => $value));
    return $element;
  }
}

/**
 * Multiple value meta tag controller.
 */
class DrupalListMetaTag extends DrupalDefaultMetaTag {

  public function getForm(array $options = array()) {
    $form['value'] = isset($this->info['form']) ? $this->info['form'] : array();

    $form['value'] += array(
      '#type' => 'checkboxes',
      '#title' => $this->info['label'],
      '#description' => !empty($this->info['description']) ? $this->info['description'] : '',
      '#default_value' => isset($this->data['value']) ? $this->data['value'] : array(),
    );

    return $form;
  }

  public function getValue(array $options = array()) {
    $values = array_keys(array_filter($this->data['value']));
    sort($values);
    return implode(', ', $values);
  }
}
