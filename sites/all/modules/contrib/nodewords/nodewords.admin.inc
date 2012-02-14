<?php
// $Id: nodewords.admin.inc,v 1.1.2.132 2009/12/30 18:28:20 kiam Exp $

/**
 * @file
 * Assign META tags to nodes, vocabularies, terms and pages.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Show the confirmation form for the page meta tags delete operation.
 */
function nodewords_pages_confirm_delete(&$form_state, $page) {
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $page->pid,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the meta tags for the page %name?', array('%name' => $page->name)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/nodewords/meta-tags/other',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 *
 */
function nodewords_pages_confirm_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    nodewords_delete_tags(NODEWORDS_TYPE_PAGE, $form_state['values']['pid']);
  }

  if (db_affected_rows()) {
    drupal_set_message(t('The configuration options have been saved.'), 'status');
  }

  $form_state['redirect'] = 'admin/content/nodewords/meta-tags/other';
}

/**
 * Return the list of pages with custom meta tags settings.
 */
function nodewords_pages_overview() {
  $enabled = array();
  $form = array('#tree' => TRUE);
  $pages = array();

  foreach (_nodewords_get_pages_data() as $pid => $page) {
    $pages[$pid] = '';

    if ($page->enabled) {
      $enabled[] = $pid;
    }

    $form['name'][$pid] = array(
      '#value' => $page->name,
    );

    $form['path'][$pid] = array(
      '#type' => 'value',
      '#value' => $page->path,
    );

    $form['weight'][$pid] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $page->weight,
    );

    $form['edit'][$pid] = array(
      '#value' => l(t('edit'), "admin/content/nodewords/meta-tags/other/edit/$page->pid"),
    );
    $form['delete'][$pid] = array(
      '#value' => l(t('delete'), "admin/content/nodewords/meta-tags/other/delete/$page->pid"),
    );
  }

  $form['add_meta_tags'] = array(
    '#value' => t(' <a href="@add_metatags">Add meta tags</a> for a new page.', array('@add_metatags' => url('admin/content/nodewords/meta-tags/other/add'))),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );

  if (!empty($pages)) {
    $form['enabled'] = array(
      '#type' => 'checkboxes',
      '#options' => $pages,
      '#default_value' => $enabled,
      '#checkall' => count($pages) > 1 ? 'nodewords-pages-overview-enabled' : FALSE,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
  }

  return $form;
}

function nodewords_pages_overview_submit($form, &$form_state) {
  $row = new stdClass();

  foreach ($form_state['values']['path'] as $pid => $value) {
    $row->pid = $pid;
    $row->path = $form_state['values']['path'][$pid];
    $row->weight = $form_state['values']['weight'][$pid];
    $row->enabled = $form_state['values']['enabled'][$pid];

    drupal_write_record('nodewords_custom', $row, 'pid');
    $done = TRUE;
  }

  if (isset($done)) {
    drupal_set_message(t('The configuration options have been saved.'));
  }
}

/**
 *
 */
function nodewords_pages_edit($form_state, $page = NULL) {
  $form = array();

  if (!isset($page)) {
    $page = (object) array(
      'name' => t('Custom page'),
      'path' => '',
      'weight' => 0,
      'enabled' => 1,
      'tags' => array(),
    );
  }
  else {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $page->pid,
    );
  }

  $nodewords_form = nodewords_form(
    NODEWORDS_TYPE_PAGE,
    $page->tags,
    array(
      'fieldset' => FALSE,
    )
  );

  if (empty($nodewords_form)) {
    return $form;
  }
  else {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t("A name to better identify the page in the list of custom pages. The name must contains at least one alpha-numeric character."),
      '#default_value' => $page->name,
      '#size' => 60,
      '#maxlength' => 60,
      '#required' => TRUE,
    );

    $form['path'] = array(
      '#type' => 'textarea',
      '#title' => t('Path'),
      '#description' => t("Enter the Drupal paths associated with the page. The character <q>*</q> is a wildcard. Example paths are <em>blog</em> for the blog page and <em>blog/*</em> for every personal blog. The list must contain at least one path."),
      '#default_value' => $page->path,
      '#size' => 60,
      '#required' => TRUE,
    );

    $form['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#description' => t('Pages with lower weight will be considered first. Only the first matching page will be used.'),
      '#delta' => 10,
      '#default_value' => $page->weight,
    );

    $form['enabled'] = array(
      '#type' => 'radios',
      '#options' => array(t('Disabled'), t('Enabled')),
      '#default_value' => $page->enabled,
    );

    $form['nodewords'] = $nodewords_form;
    $form['nodewords']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 40,
    );
  }

  return $form;
}

/**
 * Validate function for the meta tags edit page.
 */
function nodewords_pages_edit_validate($form, &$form_state) {
  if (trim($form_state['values']['name']) == '') {
    form_set_error('name', t('The name contains only spaces, and other not printable characters.'));
  }

  if (($path = trim($form_state['values']['path'])) != '') {
    $bool = db_result(
      db_query("SELECT 1 FROM {nodewords_custom} WHERE path = '%s' AND pid <> %d",
        $path,
        isset($form_state['values']['pid']) ? $form_state['values']['pid'] : 0
      )
    );

    if ($bool) {
      form_set_error('path', t('The path is already used.'));
    }
  }
  else {
    form_set_error('path', t('The path contains only spaces, and other not printable characters.'));
  }
}

/**
 * Submission function for the meta tags edit page.
 */
function nodewords_pages_edit_submit($form, &$form_state) {
  $form_state['values']['path'] = trim($form_state['values']['path']);

  drupal_write_record('nodewords_custom', $form_state['values'], isset($form_state['values']['pid']) ? 'pid' : array());
  nodewords_save_tags(NODEWORDS_TYPE_PAGE, $form_state['values']['pid'], $form_state['values']['nodewords'], TRUE);

  drupal_set_message(t('The configuration options have been saved.'));

  $form_state['redirect'] = 'admin/content/nodewords/meta-tags/other';
}

/**
 * Menu callback: settings form.
 */
function nodewords_settings_form() {
  $edit_tags = variable_get('nodewords_edit', array());
  $form = array();
  $head_tags = variable_get('nodewords_head', array());
  $options = array();
  $tags_info = nodewords_get_possible_tags();

  if (!empty($tags_info)) {
    foreach ($tags_info as $name => $info) {
      $options[$name] = $info['widget:label'];
    }

    uasort($options, 'strnatcmp');
  }

  $form['edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags to show on edit forms'),
    '#description' => t('Select the meta tags that appear in the edit forms. Users with administration permissions can edit all the meta tags, even when they are not enabled to appear in the edit forms.'),
    '#collapsible' => TRUE,
    '#group' => 'nodewords',
  );

  if (!empty($tags_info)) {
    $form['edit']['nodewords_edit'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $edit_tags,
      '#checkall' => TRUE,
    );
  }
  else {
    $form['edit']['nodewords_edit'] = array(
      '#type' => 'value',
      '#value' => $edit_tags,
    );
  }

  $form['head'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags to output in HTML'),
    '#description' => t('Select the meta tags you want to appear in the HEAD section of the HTML pages.'),
    '#collapsible' => TRUE,
    '#group' => 'nodewords',
  );

  if (!empty($tags_info)) {
    $form['head']['nodewords_head'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $head_tags,
      '#checkall' => TRUE,
    );
  }
  else {
    $form['head']['nodewords_head'] = array(
      '#type' => 'value',
      '#value' => $head_tags,
    );
  }

  $form['metatags_creation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags creation options'),
    '#description' => t('These options change the way the meta tags are generated.'),
    '#collapsible' => TRUE,
    '#group' => 'nodewords',
  );

  $form['metatags_creation']['nodewords_enable_user_metatags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the user profile meta tags'),
    '#default_value' => variable_get('nodewords_enable_user_metatags', TRUE),
  );

  $form['metatags_creation']['nodewords_list_repeat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Repeat meta tags for lists'),
    '#description' => t('Some search engines punish sites that use the same meta tags on different pages. Uncheck this option if you want to suppress the repetition of the same meta tags on pages that use the pager - if unchecked, Drupal will only display the meta tags for the first page and not for subsequent pages. If unsure, select this option.'),
    '#default_value' => variable_get('nodewords_list_repeat', FALSE),
  );

  $form['metatags_creation']['nodewords_use_frontpage_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use front page meta tags'),
    '#description' => t('Check this option if you want to use the <a href="@front-page-url" title="Meta tags for front page">meta tags for the front page</a> even if the <a href="@site-settings-url" title="Site information">default front page</a> specified is a view, panel or node - in this case, the meta tags specified for the view, panel or node will be ignored. If you want to use the meta tags of the view, panel or node instead, uncheck this option. If unsure, select this option and specify the meta tags you want on the <a href="@front-page-url" title="Meta tags for front page">meta tags for the front page</a>.', array('@front-page-url' => url('admin/content/nodewords/meta-tags/frontpage'), '@site-settings-url' => url('admin/settings/site-information'))),
    '#default_value' => variable_get('nodewords_use_frontpage_tags', TRUE),
  );

  $default_size = variable_get('nodewords_max_size', 350);

  $form['metatags_creation']['nodewords_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum meta tags length'),
    '#description' => t('The maximum length to use for the meta tags form fields.'),
    '#default_value' => $default_size,
    '#required' => TRUE,
    '#element_validate' => array('nodewords_max_size_validate'),
    '#size' => 6,
    '#maxlength' => 6,
  );

  if ($default_size < 350) {
    $form['metatags_creation']['nodewords_max_size']['#description'] .= ' ' . t('Most of the search engines are now indexing 350 characters, which is not the maximum length actually set for the meta tags. See <a href="@google-blog">Official Google Blog: Two new improvements to Google results pages</a> for more information.', array('@google-blog' => 'http://googleblog.blogspot.com/2009/03/two-new-improvements-to-google-results.html'));
  }

  $form['metatags_creation']['metatags_generation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags content generation options'),
    '#description' => t('These options change how a meta tag content is generated from the node content. These settings apply to specific meta tags.'),
    '#collapsible' => TRUE,
  );

  $options = array(
    NODEWORDS_GENERATION_NONE => t('Do not generate meta tags content'),
    NODEWORDS_GENERATION_WHEN_EMPTY => t('Generate meta tag content when the meta tag content is empty'),
  );

  $form['metatags_creation']['metatags_generation']['nodewords_metatags_generation_method'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get(
      'nodewords_metatags_generation_method', NODEWORDS_GENERATION_NONE
    ),
  );

  $options = array(
    NODEWORDS_GENERATION_BODY => t('Generate meta tags content from the node body'),
    NODEWORDS_GENERATION_TEASER => t('Generate meta tags content from the node teaser'),
    NODEWORDS_GENERATION_TEASER_BODY => t('Generate meta tags content from the node teaser, or the node body when the node teaser is empty'),
  );

  $form['metatags_creation']['metatags_generation']['nodewords_metatags_generation_source'] = array(
    '#type' => 'radios',
    '#title' => t('Generation source'),
    '#options' => $options,
    '#default_value' => variable_get(
      'nodewords_metatags_generation_source', NODEWORDS_GENERATION_TEASER
    ),
  );

  $form['metatags_creation']['metatags_generation']['nodewords_use_alt_attribute'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace the tag IMG content with the attribute ALT'),
    '#default_value' => variable_get('nodewords_use_alt_attribute', TRUE),
  );

  $options = array(
    'imagebrowser' => 'imagebrowser.module',
    'img_assist' => 'img_assist.module',
  );

  $form['metatags_creation']['metatags_generation']['nodewords_filter_modules_output'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Filter the text added by third-party modules in the node teaser'),
    '#options' => $options,
    '#default_value' => variable_get('nodewords_filter_modules_output', array()),
    '#checkall' => TRUE,
  );

  $form['metatags_creation']['metatags_generation']['nodewords_filter_regexp'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom regular expression'),
    '#description' => t('A regular expression used to filter the text added in the node teaser from a third-party module. The regular expression uses the <a href="http://www.php.net/manual/en/pcre.pattern.php">Perl compatible</a> syntax.'),
    '#element_validate' => array('nodewords_filter_regexp_validate'),
    '#default_value' => variable_get('nodewords_filter_regexp', ''),
    '#size' => 60,
  );

  $form['metatags_creation']['taxonomy'] = array();

  foreach ($tags_info as $name => $info) {
    $function = $info['tag:function:prefix'] . '_settings_form';
    $options = array(
      'parameters' => !empty($info['tag:function:parameters']) ? $info['tag:function:parameters'] : array(),
    );

    if (function_exists($function)) {
      $function($form, 'nodewords_settings_form', $options);
    }
  }

  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 100;

  return $form;
}

/**
 * Function to validate the regular expression.
 *
 */
function nodewords_filter_regexpr_validate($element, &$form_state) {

  if (!empty($element['#value'])) {
    $value = trim($element['#value']);

    if (empty($value)) {
      form_error($element, t('The regular expression contains only spaces, and other not printable characters.'));
    }
  }
}

/**
 * Function to validate the value entered for the maximum meta tags length.
 *
 */
function nodewords_max_size_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $value = trim($element['#value']);

    if (empty($value) || (!empty($value) && (!is_numeric($value) || $value <= 0))) {
      form_error($element, t('The maximum length must be a number greater than zero.'));
    }
  }
}

/**
 * Front page settings form.
 */
function nodewords_tags_form(&$form_state, $type = NODEWORDS_TYPE_DEFAULT, $id = 0) {
  $form = array();
  $form['#nodewords_type'] = $type;
  $form['#nodewords_id'] = $id;

  $form['nodewords'] = nodewords_form(
    $type,
    nodewords_load_tags($type, $id),
    array(
      'fieldset' => FALSE,
    )
  );

  if (!empty($form['nodewords'])) {
    $form['nodewords']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#submit' => array('nodewords_tags_form_submit'),
      '#weight' => 40,
    );

    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('nodewords_tags_form_reset'),
      '#weight' => 42,
    );
  }

  return $form;
}

/**
 * Submission function for the meta tags edit page (reset button).
 */
function nodewords_tags_form_reset($form, &$form_state) {
  nodewords_delete_tags($form['#nodewords_type'], $form['#nodewords_id']);
  drupal_set_message(t('The configuration options have been reset to their default values.'));
}

/**
 * Submission function for the meta tags edit page.
 */
function nodewords_tags_form_submit($form, &$form_state) {
  nodewords_save_tags($form['#nodewords_type'], $form['#nodewords_id'], $form_state['values']['nodewords'], TRUE);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Render the list of pages with meta tags.
 */
function theme_nodewords_pages_overview($form) {
  $has_pages = isset($form['name']) && is_array($form['name']);
  $rows = array();

  if ($has_pages) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['name'][$key]);

      $form['weight'][$key]['#attributes']['class'] = 'page-weight';

      $row[] = drupal_render($form['weight'][$key]);
      $row[] = drupal_render($form['enabled'][$key]);
      $row[] = drupal_render($form['edit'][$key]);
      $row[] = drupal_render($form['delete'][$key]);

      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are currently no meta tags defined.'),
        'colspan' => '5'
      )
    );
  }

  $header[] = t('Name');
  $header[] = t('Weight');

  $header[] = array(
    'data' => t('Enabled'),
    'class' => count($rows) > 1 ? 'nodewords-pages-overview-enabled' : '',
  );

  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '2',
  );

  drupal_add_tabledrag('nodewords', 'order', 'sibling', 'page-weight');

  return theme('table', $header, $rows, array('id' => 'nodewords')) . drupal_render($form);
}

/**
 * @} End of "addtogroup nodewords" .
 */
