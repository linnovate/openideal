<?php

class imagecache_profiles_handler_field_user_picture extends views_handler_field_user_picture {
  function option_definition() {
    $options = parent::option_definition();
    $options['imagecache_preset'] = array(
      'default' => variable_get('user_picture_imagecache_profiles_default', ''),
      'translatable' => FALSE
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->options;
    $form['imagecache_preset'] = array(
      '#title' => t('Imagecache preset'),
      '#type'  => 'select',
      '#options' => image_style_options(TRUE),
      '#default_value' => $options['imagecache_preset'],
    );
  }

  function render($values) {
    // Fake an account object.
    $options = $this->options;
    $account = new stdClass();
    $account->uid = $values->{$this->aliases['uid']};
    $account->name = $values->{$this->aliases['name']};
    $account->mail = isset($values->{$this->aliases['mail']}) ? $values->{$this->aliases['mail']} : '';
    $account->picture = $values->{$this->field_alias};

    $variables['account'] = $account;

    if ($options['imagecache_preset']) {
      $variables['user_picture_style'] = $options['imagecache_preset'];
    }

    return theme('user_picture', $variables);
  }
}
