<?php

/**
 * @file
 * Administrative page callbacks for Service Links module.
 */

/**
 * Theming function for build a draggable service links table.
 */
function theme_service_links_drag_table($variables) {
  $form = $variables['form'];
  $table['rows'] = array();

  foreach (element_children($form['service_links_show']) as $service_id) {
    $service = $form['service_links_show'][$service_id];
    $col = array();

    $service['name'] = array('#markup' => $service['#title']);
    $col[] = drupal_render($service['name']);

    $service['service'] = array('#markup' => $service['#service']);
    $col[] = drupal_render($service['service']);

    $service['show'] = array(
      '#type' => 'checkbox',
      '#checked' => $service['#default_value'],
      '#id' => $service['#id'],
      '#name' => $service['#name'],
    );
    $col[] = drupal_render($service['show']);

    $service['weight'] = $form['service_links_weight'][$service_id];
    $service['weight']['#attributes']['class'] = array('service-weight');
    $col[] = drupal_render($service['weight']);

    $table['weights'][] = $service['#weight'];
    $table['rows'][] = array('data' => $col, 'class' => array('draggable'));
  }

  $table['header'] = array(t('Service Name'), t('Group'), t('Show'), t('Weight'));

  drupal_alter('sl_servicestable', $table, $form);

  if (!empty($table['rows'])) {
    array_multisort($table['weights'], $table['rows']);
  }

  drupal_add_tabledrag('service_links', 'order', 'sibling', 'service-weight');

  $params = array(
    '#theme' => 'table',
    '#header' => $table['header'],
    '#rows' => $table['rows'],
    '#attributes' => array('id' => 'service_links'),
    '#empty' => t('No services available.'),
  );

  return theme('table', $params);
}

/**
 * Menu callback administration settings for general options.
 */
function service_links_admin_settings() {

  $form['where_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t("Show for content types or categories"),
    '#description' => t("Set the content types or categories you want to display links for."),
  );
  $form['where_to_show_the_links']['service_links_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#default_value' => variable_get('service_links_node_types', array()),
    '#options' => array_map('check_plain', node_type_get_names()),
    '#attributes' => array('class' => array('container-inline')),
  );
  if (module_exists('taxonomy')) {
    $selected_vocs = variable_get('service_links_category_vocs', array());
    $vocabularies = _service_links_get_vocabularies();
    $form['where_to_show_the_links']['service_links_category_vocs'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show terms from the selected vocabularies'),
      '#default_value' => $selected_vocs,
      '#options' => $vocabularies,
      '#attributes' => array('class' => array('container-inline')),
    );

    $terms = _service_links_get_terms(array_values(array_filter($selected_vocs)));
    $count = count($terms);
    $selected_types = variable_get('service_links_category_types', array());
    if ($count > 0) {
      $form['where_to_show_the_links']['service_links_category_types'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        //'#title' => t('Categories'),
        '#default_value' => $selected_types,
        '#options' => $terms,
        //'#size' => ($count > 10 ? 10 : $count),
        '#attributes' => array('style' => 'display: none;'),
      );
      foreach ($vocabularies as $vid => $vocabulary) {
        if (in_array($vid, $selected_vocs)) {
          $terms = _service_links_get_terms(array($vid));
          $form['where_to_show_the_links']['service_links_category_by_voc_' . $vid] = array(
            '#type' => 'select',
            '#multiple' => TRUE,
            '#title' => t('@voc', array('@voc' => $vocabulary)),
            '#default_value' => $selected_types,
            '#options' => $terms,
            '#size' => ($count > 10 ? 10 : $count),
          );
        }
      }
    }
  }

  $form['places_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Where to show service links'),
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
    '#description' => t('When to display the services in the links section.'),
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
    '#description' => t('When to display the services after the content text.'),
  );

  $form['page_match'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $access = user_access('use PHP for service visibility');
  $visibility = variable_get('service_links_visibility_for_node', SERVICE_LINKS_VISIBILITY_NOT_LISTED);
  $pages = variable_get('service_links_page_match_for_node', '');
  if ($visibility == SERVICE_LINKS_VISIBILITY_PHP && !$access) {
    $form['page_match'] = array(
      'service_links_visibility_for_node' => array(
        '#type' => 'value', 
        '#value' => SERVICE_LINKS_VISIBILITY_PHP,
      ),
      'service_links_page_match_for_node' => array(
        '#type' => 'value',
        '#value' => $pages,
      ),
    );
  }
  else {
    $options = array(
      SERVICE_LINKS_VISIBILITY_NOT_LISTED => t('All pages except those listed'),
      SERVICE_LINKS_VISIBILITY_ONLY_LISTED => t('Only the listed pages'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $access) {
      $options += array(SERVICE_LINKS_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }

    $form['page_match']['service_links_visibility_for_node'] = array(
      '#type' => 'radios',
      '#title' => t('Show Service Links on specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_match']['service_links_page_match_for_node'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#default_value' => $pages,
      '#description' => $description,
    );
  }

  $form['how_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('How to display Service Links'),
  );
  $form['how_to_show_the_links']['service_links_style'] = array(
    '#type' => 'select',
    '#title' => t('Choose a style'),
    '#default_value' => variable_get('service_links_style', SERVICE_LINKS_STYLE_IMAGE),
    '#options' => array(
      SERVICE_LINKS_STYLE_TEXT => t('Only Text'),
      SERVICE_LINKS_STYLE_IMAGE => t('Only Image'),
      SERVICE_LINKS_STYLE_IMAGE_AND_TEXT => t('Image and Text'),
    ),
  );

  $styles = module_invoke_all('sl_styles', FALSE);
  if (!empty($styles)) {
    $form['how_to_show_the_links']['service_links_style']['#options'] += $styles;
  }

  $form['how_to_show_the_links']['service_links_hide_if_unpublished'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't show links if the content is unpublished"),
    '#default_value' => variable_get('service_links_hide_if_unpublished', 0),
  );
  $form['how_to_show_the_links']['service_links_hide_for_author'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't show links if the actual user is the author of the node"),
    '#default_value' => variable_get('service_links_hide_for_author', 0),
  );
  $form['how_to_show_the_links']['in_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service Links in nodes'),
  );
  $form['how_to_show_the_links']['in_node']['service_links_label_in_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Print the label'),
    '#default_value' => variable_get('service_links_label_in_node', t('Bookmark/Search this post with')),
    '#size' => '40',
  );
  $form['how_to_show_the_links']['in_node']['service_links_weight_in_node'] = array(
    '#type' => 'weight',
    '#delta' => 100,
    '#title' => t('Assign a weight'),
    '#default_value' => variable_get('service_links_weight_in_node', 10),
    '#description' => t('The weight help to place the Service Links section above or below the other elements.'),
  );

  $form['how_to_show_the_links']['icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service Links icons'),
  );
  $form['how_to_show_the_links']['icons']['service_links_path_icons'] = array(
    '#type' => 'textfield',
    '#title' => t('Standard folder'),
    '#description' => t('If you have alternative icons enter here the relative path from your index.php without trailing slash (i.e. %path1 or %path2)', array('%path1' => 'files/newicons', '%path2' => 'sites/all/files/newicons')),
    '#default_value' => service_links_expand_path(NULL, 'icons'),
    '#size' => 40,
  );
  $form['how_to_show_the_links']['icons']['service_links_check_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default icons if missing'),
    '#description' => t('Check every icon in the standard folder and if not available, consider the default path %path', array('%path' => service_links_expand_path(NULL, 'preset'))),
    '#default_value' => variable_get('service_links_check_icons', FALSE),
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
    '#description' => t('This text will be inserted at the end of the current URL (i.e. %param => %url)', array('%param' => 'param1=value1&param2=value2', '%url' => 'http://domain.com/current-page&param1=value1&param2=value2')),
    '#default_value' => variable_get('service_links_append_to_url', ''),
    '#size' => 40,
  );
  $form['extra_options']['title'] = array(
    '#type' => 'fieldset',
  );
  $form['extra_options']['title']['service_links_override_title'] = array(
    '#type' => 'select',
    '#title' => t('How to fill the title tag'),
    '#default_value' => variable_get('service_links_override_title', SERVICE_LINKS_TAG_TITLE_NODE),
    '#options' => array(
      SERVICE_LINKS_TAG_TITLE_NODE => t('Use the original node title'),
      SERVICE_LINKS_TAG_TITLE_OVERRIDE => t('Override the original title'),
    ),
  );
  $form['extra_options']['title']['service_links_override_title_text'] = array(
    '#type' => 'textfield',
    '#description' => t("Enter the text that override the title: use @title for referrer the original title, either the various tokens offered by <a href='@url'>Token</a> if it's enabled and the related option active.", array('@url' => 'http://www.drupal.org/project/token', '@title' => '<title>')),
    '#default_value' => variable_get('service_links_override_title_text', '<title>'),
    '#size' => 40,
  );

  if (module_exists('token')) {
    $form['extra_options']['title']['service_links_override_title']['#options'] += array(
      SERVICE_LINKS_TAG_TITLE_TOKEN => t('Parse the string with Token'),
    );
    $form['extra_options']['title']['token_help'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Token help'),
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }

  $form['short_links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Short Links'),
  );
  $form['short_links']['service_links_short_links_use'] = array(
    '#type' => 'select',
    '#title' => t('Use short links'),
    '#default_value' => variable_get('service_links_short_links_use', SERVICE_LINKS_SHORT_URL_USE_NEVER),
    '#options' => array(
      SERVICE_LINKS_SHORT_URL_USE_NEVER => t('Never'),
      SERVICE_LINKS_SHORT_URL_USE_WHEN_REQUESTED => t('Only when requested'),
      SERVICE_LINKS_SHORT_URL_USE_ALWAYS => t('Always')),
  );
  if (variable_get('service_links_short_links_use', SERVICE_LINKS_SHORT_URL_USE_NEVER) > 0) {
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
      '#description' => t('Write here the complete address without trailing slash (i.e. %name)', array('%name' => 'http://www.example.com') ),
      '#default_value' => variable_get('service_links_domain_redirect', ''),
      '#size' => 40,
    );
  }
  $form['#validate'][] = 'service_links_admin_services_validate';

  return system_settings_form($form);
}

/**
 * Check if domain address textbox is empty when should be not,
 * copy the selected terms into a unique list,
 * check the path of custom'icons.
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

  if (isset($form_state['values']['service_links_category_vocs']) && module_exists('taxonomy')) {
    $selected_vocs = array_filter($form_state['values']['service_links_category_vocs']);
    if (empty($selected_vocs)) {
      $form_state['values']['service_links_category_types'] = array();
    }
    else {
      $vocabularies = _service_links_get_vocabularies();
      $form_state['values']['service_links_category_types'] = array();
      foreach ($vocabularies as $vid => $vocabulary) {
        if (isset($form_state['values']['service_links_category_by_voc_' . $vid])) {
          $form_state['values']['service_links_category_types'] += $form_state['values']['service_links_category_by_voc_' . $vid];
        }

      }
    }
  }

  if (!empty($form_state['values']['service_links_path_icons'])) {
    if (!is_dir($form_state['values']['service_links_path_icons'])) {
      form_set_error('service_links_path_icons', t('The path for custom icons is not valid'));
    }
    elseif (preg_match("/\/$/", $form_state['values']['service_links_path_icons'])) {
      form_set_error('service_links_path_icons', t('No trailing slash!'));
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
    $icon = array(
      'path' => isset($service['icon']) ? service_links_expand_path($service['icon'], 'preset') : service_links_expand_path("$service_id.png", 'preset'),
    );
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

  if (empty($services)) {
    drupal_set_message(t('You need to load at least one of XXX Services module, please enable them in <a href="@url">admin > modules</a> page', array('@url' => url('admin/modules'))), 'warning');
  }

  return system_settings_form($form);
}

/**
 * Build an array of all taxonomy terms.
 */
function _service_links_get_terms($included_vocs = array()) {
  $types = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    if (in_array($vocabulary->vid, $included_vocs)) {
      $tree = taxonomy_get_tree($vocabulary->vid);
      foreach ($tree as $term) {
        $types[$term->tid] = check_plain($term->name);
      }
    }
  }

  return $types;
}

/**
 * Return a list of vocabularies generated by taxonomy module.
 */
function _service_links_get_vocabularies() {
  $list_vocs = taxonomy_get_vocabularies();
  $vocs = array();
  foreach ($list_vocs as $vocabulary) {
    if ($vocabulary->module == 'taxonomy') {
      $vocs[$vocabulary->vid] = check_plain($vocabulary->name);
    }
  }

  return $vocs;
}
