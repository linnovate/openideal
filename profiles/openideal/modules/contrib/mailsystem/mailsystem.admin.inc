<?php

/**
 * @file
 * Administrative form for setting the mail_system variable.
 */
function mailsystem_admin_settings() {
  $args = array(
    '!interface' => url('http://api.drupal.org/api/drupal/includes--mail.inc/interface/MailSystemInterface/7'),
    '@interface' => 'MailSystemInterface',
    '!format' => url('http://api.drupal.org/api/drupal/includes--mail.inc/function/MailSystemInterface%3A%3Aformat/7'),
    '@format' => 'format()',
    '!mail' => url('http://api.drupal.org/api/drupal/includes--mail.inc/function/MailSystemInterface%3A%3Amail/7'),
    '@mail' => 'mail()',
    '!default_class' => url('http://api.drupal.org/api/drupal/modules--system--system.mail.inc/class/DefaultMailSystem/7'),
    '@default_class' => mailsystem_default_value(),
    '%module' => 'module',
    '%key' => 'key',
  );
  $form = array('#submit' => array('mailsystem_admin_settings_submit'));
  $mail_system = mailsystem_get();
  $mail_defaults = mailsystem_defaults();
  $mailsystem_classes = mailsystem_get_classes();
  $descriptions = array();
  foreach (system_rebuild_module_data() as $item) {
    if ($item->status) {
      $descriptions[$item->name] = (
        empty($item->info['package'])
        ? '' : $item->info['package']
      ) . ' » ' . t('!module module', array('!module' => $item->info['name']));
    }
  }
  asort($descriptions);
  $form['mailsystem'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail System Settings'),
    '#description' => t(
      'Drupal provides a default <a href="!interface"><code>@interface</code></a> class called <a href="!default_class"><code>@default_class</code></a>. Modules may provide additional classes. Each <a href="!interface"><code>@interface</code></a> class may be associated with one or more identifiers, composed of a %module and an optional %key. Each email being sent also has a %module and a %key. To decide which class to use, Drupal uses the following search order: <ol><li>The class associated with the %module and %key, if any.</li><li>The class associated with the %module, if any.</li><li>The site-wide default <a href="!interface"><code>@interface</code></a> class.</li></ol>', $args
    ),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );
  $form['mailsystem'][mailsystem_default_id()] = array(
    '#type' => 'select',
    '#title' => t(
      'Site-wide default <a href="!interface"><code>@interface</code></a> class', $args
    ),
    '#options' => $mailsystem_classes,
    '#default_value' => $mail_system[mailsystem_default_id()],
  );
  $mailsystem_classes = array(
    mailsystem_default_id() => t('Remove this setting.')
  ) + $mailsystem_classes;
  foreach (array_diff_key($mail_system, $mail_defaults) as $id => $class) {
    // Separate $id into $module and $key.
    $module = $id;
    while ($module && empty($descriptions[$module])) {
      // Remove a key from the end
      $module = implode('_', explode('_', $module, -1));
    }
    // If an array key of the $mail_system variable is neither "default-system"
    // nor begins with a module name, then it should be unset.
    if (empty($module)) {
      watchdog('mailsystem', "Removing bogus mail_system key %id.", array('%id' => $id), WATCHDOG_WARNING);
      unset($mail_system[$id]);
      continue;
    }
    // Set $title to the human-readable module name.
    $title = preg_replace('/^.* » /', '', $descriptions[$module]);
    if ($key = substr($id, strlen($module) + 1)) {
      $title .= " ($key key)";
    }
    $title .= ' class';
    $form['mailsystem'][$id] = array(
      '#type' => 'select',
      '#title' => $title,
      '#options' => $mailsystem_classes,
      '#default_value' => $class,
    );
    unset($descriptions[$module]);
  }
  // Generate a list of themes which may used to render emails.
  $theme_options = array('current' => t('Current'), 'default' => t('Default'));
  if (module_exists('domain_theme')) {
    $theme_options['domain'] = t('Domain Theme');
  }
  // Get a list of all themes.
  $themes = list_themes();
  foreach ($themes as $name => $theme) {
    if ($theme->status == 1) {
      $theme_options[$name] = $theme->info['name'];
    }
  }
  $form['mailsystem']['mailsystem_theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme to render the emails'),
      '#description' => t('Select the theme that will be used to render the emails. This can be either the current theme, the default theme, the domain theme or any active theme.'),
      '#options' => $theme_options,
      '#default_value' => variable_get('mailsystem_theme', 'current'),
  );
  $form['class'] = array(
    '#type' => 'fieldset',
    '#title' => t('New Class'),
    '#description' => t(
      'Create a new <a href="!interface"><code>@interface</code></a> that inherits its methods from other classes. The new class will be named after the other classes it uses.', $args
    ),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $mailsystem_classes[mailsystem_default_id()] = '--Select--';
  $form['class']['format'] = array(
    '#type' => 'select',
    '#title' => t(
      'Class to use for the <a href="!format"><code>@format</code></a> method', $args
    ),
    '#options' => $mailsystem_classes,
  );
  $form['class']['mail'] = array(
    '#type' => 'select',
    '#title' => t(
      'Class to use for the <a href="!mail"><code>@mail</code></a> method', $args
    ),
    '#options' => $mailsystem_classes,
  );
  $form['identifier'] = array(
    '#type' => 'fieldset',
    '#title' => t('New Setting'),
    '#description' => t('Add a new %module and %key to the settings list.',
      array(
        '%module' => 'module',
        '%key' => 'key',
      )
    ),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  array_unshift($descriptions, t('-- Select --'));
  $form['identifier']['module'] = array(
    '#type' => 'select',
    '#title' => t('Module'),
    '#options' => $descriptions,
  );
  $form['identifier']['key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#size' => 80,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Processes mailsystem_admin_settings form.
 */
function mailsystem_admin_settings_submit($form, &$form_state) {
  variable_set('mailsystem_theme', $form_state['values']['mailsystem']['mailsystem_theme']);
  // Rebuild the theme registry to make changes needed by theme rendering.
  drupal_theme_rebuild();
  unset($form_state['values']['mailsystem']['mailsystem_theme']);

  $default_id = mailsystem_default_id();
  $mail_system = array(
    $default_id => (
      empty($form_state['values'][$default_id])
      ? mailsystem_default_value()
      : $form_state['values'][$default_id]
    )
  );
  foreach (element_children($form_state['values']['mailsystem']) as $module) {
    $class = $form_state['values']['mailsystem'][$module];
    if (!empty($class) && $class != $default_id) {
      $mail_system[$module] = $class;
    }
  }
  unset($form_state['values']['mailsystem']);
  if ($form_state['values']['class']['format'] === mailsystem_default_id()) {
    unset($form_state['values']['class']['format']);
  }
  if ($form_state['values']['class']['mail'] === mailsystem_default_id()) {
    unset($form_state['values']['class']['mail']);
  }
  if ($form_state['values']['class']) {
    $new_class = mailsystem_create_class($form_state['values']['class']);
  }
  else {
    $new_class = $mail_system[mailsystem_default_id()];
  }
  unset($form_state['values']['class']);
  if ($id = $form_state['values']['identifier']['module']) {
    if (!empty($form_state['values']['identifier']['key'])) {
      $id .= '_' . $form_state['values']['identifier']['key'];
    }
    $mail_system[$id] = $new_class;
  }
  unset($form_state['values']['identifier']);
  variable_set('mail_system', $mail_system);
  drupal_set_message(t('The configuration options have been saved.'));
}
