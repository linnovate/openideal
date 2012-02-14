<?php
// $Id: service_links.admin.inc,v 1.1.2.15 2010/10/07 09:03:33 thecrow Exp $

/**
 * @file
 * Administrative page callbacks for service links module.
 */

/**
 * Theming function for build a draggable service links table.
 */
function theme_service_links_drag_table($form) {
  $table = array();

  foreach (element_children($form['service_links_show']) as $service_id) {
    $service = $form['service_links_show'][$service_id];
    $col = array();

    $service['name'] = array('#value' => $service['#title']);
    $col[] = drupal_render($service['name']);

    $service['service'] = array('#value' => $service['#service']);
    $col[] = drupal_render($service['service']);

    $service['show'] = array(
      '#type' => 'checkbox',
      '#value' => $service['#default_value'],
      '#id' => $service['#id'],
      '#name' => $service['#name'],
    );
    $col[] = drupal_render($service['show']);

    $service['weight'] = $form['service_links_weight'][$service_id];
    $service['weight']['#attributes']['class'] = 'service-weight';
    $col[] = drupal_render($service['weight']);

    $table['weights'][] = $service['#weight'];
    $table['rows'][] = array('data' => $col, 'class' => 'draggable');
  }

  if (empty($table['rows'])) {
    $table['rows'][] = array(array('data' => t('No service available.'), 'colspan' => '5'));
  }
  else {
    array_multisort($table['weights'], $table['rows']);
  }

  $table['header'] = array(t('Service Name'), t('Service Group'), t('Show'), t('Weight'));
  drupal_add_tabledrag('service_links', 'order', 'sibling', 'service-weight');

  return theme('table', $table['header'], $table['rows'], array('id' => 'service_links'));
}

/**
 * Menu callback administration settings for general options.
 */
function service_links_admin_settings() {

  $form['where_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t("Show for content's type or category"),
    '#description' => t("Set the content's types or categories you want to display links for."),
  );
  $form['where_to_show_the_links']['service_links_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#default_value' => variable_get('service_links_node_types', array()),
    '#options' => array_map('check_plain', node_get_types('names')),
    '#attributes' => array('class' => 'container-inline'),
  );
  if (module_exists('taxonomy')) {
    $terms = _service_links_get_terms();
    $count = count($terms);
    $form['where_to_show_the_links']['service_links_category_types'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Categories'),
      '#default_value' => variable_get('service_links_category_types', array()),
      '#options' => $terms,
      '#size' => ($count > 10 ? 10 : $count),
    );
  }

  $form['places_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Places where show Service Links'),
  );
  $form['places_to_show_the_links']['service_links_in_links'] = array(
    '#type' => 'select',
    '#title' => t('Service links in links'),
    '#default_value' => variable_get('service_links_in_links', SERVICE_LINKS_IN_BOTH),
    '#options' => array(
      SERVICE_LINKS_DISABLED => t('Disabled'),
      SERVICE_LINKS_IN_TEASER => t('Teaser view'),
      SERVICE_LINKS_IN_FULL => t('Full-page view'),
      SERVICE_LINKS_IN_BOTH => t('Teasers and full-page view')),
    '#description' => t('When display the services in the links section.'),
  );
  $form['places_to_show_the_links']['service_links_in_node'] = array(
    '#type' => 'select',
    '#title' => t('Service links in nodes'),
    '#default_value' => variable_get('service_links_in_node', SERVICE_LINKS_DISABLED),
    '#options' => array(
      SERVICE_LINKS_DISABLED => t('Disabled'),
      SERVICE_LINKS_IN_TEASER => t('Teaser view'),
      SERVICE_LINKS_IN_FULL => t('Full-page view'),
      SERVICE_LINKS_IN_BOTH => t('Teasers and full-page view')),
    '#description' => t('When display the services after the content text.'),
  );

  $form['how_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('How display Service Links'),
  );
 $form['how_to_show_the_links']['service_links_style'] = array(
    '#type' => 'select',
    '#title' => t('Choice a style'),
    '#default_value' => variable_get('service_links_style', SERVICE_LINKS_STYLE_IMAGE),
    '#options' => array(
      SERVICE_LINKS_STYLE_TEXT => t('Only Text'),
      SERVICE_LINKS_STYLE_IMAGE => t('Only Image'),
      SERVICE_LINKS_STYLE_IMAGE_AND_TEXT => t('Image and Text')
    ),
  );
  $form['how_to_show_the_links']['service_links_hide_if_unpublished'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't show links if the content is unpublished"),
    '#default_value' => variable_get('service_links_hide_if_unpublished', 0),
  );
  $form['how_to_show_the_links']['service_links_path_icons'] = array(
    '#type' => 'textfield',
    '#title' => t("Alternative icon's folder"),
    '#description' => t('If you have alternative icons write here the path from your index.php without trailing slash (b.e. %path1 or %path2)', array('%path1' => 'files/newicons', '%path2' => 'sites/all/files/newicons')),
    '#default_value' => service_links_expand_path(NULL, 'icons'),
    '#size' => 40,
  );
  $form['how_to_show_the_links']['service_links_label_in_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Set a label for the section into the node'),
    '#default_value' => variable_get('service_links_label_in_node', t('Bookmark/Search this post with')),
    '#size' => '40',
  );

  $form['extra_options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Extra Options'),
  );
  $form['extra_options']['service_links_new_window'] = array(
    '#type' => 'select',
    '#title' => t('Open link in...'),
    '#default_value' => variable_get('service_links_new_window', 0),
    '#options' => array(0 => t('Same window'), 1 => t('New window')),
  );
  $form['extra_options']['service_links_append_to_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Append the following text to your URL'),
    '#description' => t('This text will be inserted at the end of the current URL (b.e. %param => %url)', array('%param' => 'param1=value1&param2=value2', '%url' => 'http://domain.com/current-page&param1=value1&param2=value2')),
    '#default_value' => variable_get('service_links_append_to_url', ''),
    '#size' => 40,
  );

  $form['short_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Short Links'),
  );
  $form['short_links']['service_links_short_links_use'] = array(
    '#type' => 'select',
    '#title' => t('Use short links'),
    '#default_value' => variable_get('service_links_short_links_use', 0),
    '#options' => array(
      SERVICE_LINKS_SHORT_URL_USE_NEVER => t('Never'),
      SERVICE_LINKS_SHORT_URL_USE_WHEN_REQUESTED => t('Only when requested'),
      SERVICE_LINKS_SHORT_URL_USE_ALWAYS => t('Always')),
  );
  if (variable_get('service_links_short_links_use', 0) > 0) {
    $form['short_links']['service_links_short_links_type'] = array(
      '#type' => 'select',
      '#title' => t('How generate short links'),
      '#default_value' => variable_get('service_links_short_links_type', SERVICE_LINKS_SHORT_URL_TYPE_NODE),
      '#description' => t('If you select "Short URL," it will use the service selected with the <a href="@shorten">Shorten URLs module</a>. If you have not enabled the module, Service Links will default to TinyURL.', array('@shorten' => 'http://drupal.org/project/shorten')),
      '#options' => array(
        SERVICE_LINKS_SHORT_URL_TYPE_NODE => t('Use node/xxx alias'),
        SERVICE_LINKS_SHORT_URL_TYPE_SERVICE => t('Use Short Url service'),
        SERVICE_LINKS_SHORT_URL_TYPE_REDIRECT_DOMAIN => t('Redirect only the Domain name'),
        SERVICE_LINKS_SHORT_URL_TYPE_REDIRECT_ALL => t('Combo: domain redirect and node/xxx alias')),
    );
    $form['short_links']['service_links_domain_redirect'] = array(
      '#type' => 'textfield',
      '#title' => t('Domain to redirect'),
      '#description' => t('Write here the complete address without trailing slash (b.e. %name)', array('%name' => 'http://www.example.com') ),
      '#default_value' => variable_get('service_links_domain_redirect', ''),
      '#size' => 40,
    );
    $form['#validate'][] = 'service_links_admin_services_validate';
  }

  return system_settings_form($form);
}

/**
 * Check if domain address textbox is empty
 */
function service_links_admin_services_validate($form, &$form_state) {
  if (isset($form_state['values']['service_links_short_links_type']) && (int) $form_state['values']['service_links_short_links_type'] > 2) {
    if (!$form_state['values']['service_links_domain_redirect']) {
      form_set_error('service_links_domain_redirect', t('Domain redirect address is not set.'));
    }
    if (preg_match("/\/$/", $form_state['values']['service_links_domain_redirect'])) {
      form_set_error('service_links_domain_redirect', t('No trailing slash!'));
    }
  }
}

/**
 * Menu callback administration settings for services links list.
 */
function service_links_admin_services() {
  $settings = array();

  $services = service_links_get_links(NULL, TRUE);
  $settings['show'] = variable_get('service_links_show', NULL);
  $settings['weight'] = variable_get('service_links_weight', NULL);

  $form['service_links'] = array('#theme' => 'service_links_drag_table');
  $form['service_links']['service_links_show'] = array('#tree' => TRUE);
  $form['service_links']['service_links_weight'] = array('#tree' => TRUE);

  foreach ($services as $service_id => $service) {
    $icon = isset($service['icon']) ? service_links_expand_path($service['icon'], 'preset') : service_links_expand_path("$service_id.png", 'preset');
    $weight = isset($settings['weight'][$service_id]) ? $settings['weight'][$service_id] : 0;

    $form['service_links']['service_links_show'][$service_id] = array(
      '#service' => ucwords(str_replace('_', ' ', $service['module'])),
      '#weight' => $weight,
      '#type' => 'checkbox',
      '#title' => theme('image', $icon) . " " . t('Show %name link', array('%name' => $service['name'])),
      '#return_value' => 1,
      '#default_value' => isset($settings['show'][$service_id]) ? $settings['show'][$service_id] : 0,
    );
    $form['service_links']['service_links_weight'][$service_id] = array(
      '#type' => 'weight',
      '#delta' => 100,
      '#default_value' => $weight,
    );
  }

  return system_settings_form($form);
}
