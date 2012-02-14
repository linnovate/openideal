<?php
// $Id: admin.theme.inc,v 1.1.2.1 2010/02/22 16:02:42 yhahn Exp $

/**
 * Theme switcher admin block.
 */
function admin_block_theme() {
  if (user_access('select different theme')) {
    $themes = list_themes();
    if (count($themes) > 2) {
      return array(
        'subject' => t('Switch theme'),
        'content' => drupal_get_form('admin_block_theme_form', $themes),
      );
    }
  }
}

/**
 * Devel admin block form.
 */
function admin_block_theme_form($form_state, $themes) {
  $options = array();
  foreach ($themes as $theme) {
    if ($theme->status) {
      $options[$theme->name] = isset($theme->info['name']) ? check_plain($theme->info['name']) : $theme->name;
    }
  }
  $form = array();
  $form['theme_default'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('theme_default', 'garland'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['#submit'] = array('system_settings_form_submit');
  return $form;
}
