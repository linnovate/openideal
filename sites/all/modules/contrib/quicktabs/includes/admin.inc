<?php
// $Id: admin.inc,v 1.1.6.5 2010/08/31 17:34:06 katbailey Exp $

/**
 * Page callback for quicktabs admin landing page.
 */
function quicktabs_list() {
  $header = array(
    array('data' => t('Quicktab')),
    array('data' => t('Storage')),
    array('data' => t('Operations'), 'colspan' => module_exists('ctools') ? 4 : 3),
  );
  $rows = array();
  
  foreach (quicktabs_get_all_quicktabs() as $quicktabs) {
    if (module_exists('ctools')) {
      // Determine storage
      switch ($quicktabs['export_type']) {
        case EXPORT_IN_DATABASE | EXPORT_IN_CODE:
          $storage = t('Overridden');
          $delete = l(t('Revert'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/delete');
          break;
        case EXPORT_IN_DATABASE:
          $storage = t('Normal');
          $delete = l(t('Delete'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/delete');
          break;
        case EXPORT_IN_CODE:
          $storage = t('Default');
          $delete = '';
          break;
      }
      $tablerow = array(
        array('data' => $quicktabs['machine_name']),
        array('data' => $storage),
        array('data' => l(t('Edit'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/edit')),
        array('data' => l(t('Export'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/export')),
        array('data' => l(t('Clone'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/clone')),
        array('data' => $delete),
      );
    }
    else {
      $tablerow = array(
        array('data' => $quicktabs['title']),
        array('data' => t('Normal')),
        array('data' => l(t('Edit'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/edit')),
        array('data' => l(t('Clone'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/clone')),
        array('data' => l(t('Delete'), 'admin/build/quicktabs/'. $quicktabs['machine_name'] .'/delete')),
      );
    }
    $rows[] = $tablerow;
  }
  if (!empty($rows)) {
    $output = theme('table', $header, $rows, array('id' => 'quicktabs'));
  }
  else {
    $output = '<p>'. t('Click on the "New QT block" tab to get started.') .'</p>';
  }
  return $output;
}

/**
 * Clone QuickTabs.
 */
function quicktabs_clone($quicktabs) {
  $quicktabs['machine_name'] = 0;
  $quicktabs['title'] = '';
  return drupal_get_form('quicktabs_form', 'clone', $quicktabs);
}

/**
 * Build the quicktab creation and edit form.
 */
function quicktabs_form($form_state, $formtype, $quicktabs = array()) {
  $form = _qt_admin_main_form($form_state, $quicktabs);

  $no_tabs = TRUE;
  if (empty($quicktabs['tabs'])) {
    $quicktabs['tabs'] = array(
      0 => array(),
      1 => array(),
    );
  }
  else {
    $no_tabs = FALSE;
    $tab_titles = array();
  }

  // Add the current tabs to the form.
  foreach ($quicktabs['tabs'] as $delta => $details) {
    $details['delta'] = $delta;
    $form['qt_wrapper']['tabs'][$delta] = _quicktabs_form($details);
    if (isset($details['title'])) {
      $tab_titles[$delta] = $details['title'];
    }
  }
  
  if (!$no_tabs) {
    $form['default_tab'] = array(
      '#type' => 'select',
      '#title' => t('Default tab'),
      '#options' => $tab_titles,
      '#default_value' => $quicktabs['default_tab'],
      '#weight' => -6,
    );
  }

  return $form;
}

/*
 * The main section of admin page.
 */
function _qt_admin_main_form($form_state, &$quicktabs) {
  $form['#cache'] = TRUE;

  // the contents of $quicktabs will either come from the db or from $form_state
  if (isset($form_state['quicktabs'])) {
    $quicktabs = $form_state['quicktabs'] + (array)$quicktabs;
  }

  if (empty($quicktabs['machine_name'])) {
    $form['machine_name'] = array(
      '#title' => t('Machine Name'),
      '#type' => 'textfield',
      '#description' => t('A unique ID that will be used internally and in the CSS ID of your quicktabs block. must contain only lowercase letters, numbers, and underscores.'),
      '#default_value' => '',
      '#weight' => -8,
      '#required' => TRUE,
  );
  }
  else {
    $form['machine_name'] = array('#type' => 'value', '#value' => $quicktabs['machine_name']);
    $form['machine_name_display'] = array(
      '#title' => t('Machine-readable name'),
      '#type' => 'item',
      '#value' => "<code>{$quicktabs['machine_name']}</code>",
      '#description' => t('The machine name will be used internally by quicktabs and will be used in the CSS ID of your quicktabs block.'),
      '#weight' => -8,
    );
  }

  $form['title'] = array(
    '#title' => t('Block title'),
    '#type' => 'textfield',
    '#description' => t('This will appear as the name of this block in administer >> site building >> blocks.'),
    '#default_value' => isset($quicktabs['title']) ? $quicktabs['title'] : '',
    '#weight' => -10,
  );

  $form['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#options' => quicktabs_style_options(),
    '#default_value' => isset($quicktabs['style']) ? $quicktabs['style'] : 'default',
    '#description' => t('Choose the quicktab style.'),
    '#weight' => -8,
  );

  $form['ajax'] = array(
    '#type' => 'radios',
    '#title' => t('Ajax'),
    '#options' => array(
      TRUE => t('Yes') .': '. t('Load only the first tab on page view.'),
      FALSE => t('No') .': '. t('Load all tabs on page view.'),
    ),
    '#default_value' => isset($quicktabs['ajax']) ? $quicktabs['ajax'] : 0,
    '#description' => t('Choose how the content of tabs should be loaded.<p>By choosing "Yes", only the first tab will be loaded when the page first viewed. Content for other tabs will be loaded only when the user clicks the other tab. This will provide faster initial page loading, but subsequent tab clicks will be slower. This can place less load on a server.</p><p>By choosing "No", all tabs will be loaded when the page is first viewed. This will provide slower initial page loading, and more server load, but subsequent tab clicks will be faster for the user. Use with care if you have heavy views.</p>'),
    '#weight' => -6,
  );

  $form['hide_empty_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide empty tabs'),
    '#default_value' => isset($quicktabs['hide_empty_tabs']) ? $quicktabs['hide_empty_tabs'] : 0,
    '#description' => t('Empty and restricted tabs will not be displayed. Could be useful when the tab content is not accessible.<br />This option does not work in ajax mode.'),
    '#weight' => -4,
  );

  // Add a wrapper for the tabs and Add Another Tab button.
  $form['qt_wrapper'] = array(
    '#tree' => FALSE,
    '#weight' => -3,
    '#prefix' => '<div class="clear-block" id="quicktabs-tabs-wrapper">',
    '#suffix' => '</div>',
  );

  $form['qt_wrapper']['tabs'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="quicktabs-tabs">',
    '#suffix' => '</div>',
    '#theme' => 'quicktabs_admin_form_tabs',
  );

  $form['qt_wrapper']['tabs_more'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="add-more-tabs-button">',
    '#suffix' => '<label for="edit-tabs-more">'. t('Add tab') .'</label></div>',
    '#value' => t('More tabs'),
    '#attributes' => array('class' => 'add-tab', 'title' => t('Click here to add more tabs.')),
    '#weight' => 1,
    '#submit' => array('qt_more_tabs_submit'), // If no javascript action.
    '#ahah' => array(
      'path' => 'quicktabs/ahah',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['submit_form'] = array(
    '#type' => 'submit',
    '#weight' => 10,
    '#value' => t('Save'),
  );

  return $form;
}

/*
 * Build one row (one tabpage) on the QT admin form.
 */
function _quicktabs_form($details) {
  $form['#tree'] = TRUE;
  $delta = $details['delta'];

  $form['weight'] = array(
    '#type' => 'weight',
    '#default_value' => isset($details['weight']) ? $details['weight'] : $delta-100,
    '#delta' => 100,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#size' => '10',
    '#default_value' => isset($details['title']) ? $details['title'] : '',
  );

  $tabtypes = array(
    'block' => t('Block'),
    'node' => t('Node'),
    'qtabs' => t('QTab'),
    'callback' => t('QT Callback'),
  );

  if (module_exists('views')) {
    $views = quicktabs_get_views();
    $views_keys = array_keys($views);
    $tabtypes['view'] = t('View!translate_as_list_of_content', array('!translate_as_list_of_content' => ''));

    $selected_view = (isset($details['vid']) ? $details['vid'] : (isset($views_keys[0]) ? $views_keys[0] : ''));
    $form['view']['vid'] = array(
      '#type' => 'select',
      '#options' => $views,
      '#default_value' => $selected_view,
      '#title' => t('Select a view'),
      '#ahah' => array(
        'path' => 'quicktabs/ahah/',
        'wrapper' => 'quicktabs-tabs',
        'method' => 'replace',
        'event' => 'change',
      ),
    );
    $form['view']['display'] = array(
      '#type' => 'select',
      '#title' => 'display',
      '#options' => _quicktabs_get_views_displays($selected_view),
      '#default_value' => isset($details['display']) ? $details['display'] : '',
    );
    $form['view']['args'] = array(
      '#type' => 'textfield',
      '#title' => 'arguments',
      '#size' => '40',
      '#required' => FALSE,
      '#default_value' => isset($details['args']) ? $details['args'] : '',
      '#description' => t('Additional arguments to send to the view as if they were part of the URL in the form of arg1/arg2/arg3. You may use %0, %1, ..., %N to grab arguments from the URL.'),
    );
    $form['view']['get_displays'] = array(
      '#type' => 'submit',
      '#value' => 'vdisp_'. $delta,
      '#submit' => array('qt_get_displays_submit'),
    );
  }

  $form['block']['bid'] = array(
    '#type' => 'select',
    '#options' => quicktabs_get_blocks(),
    '#default_value' => isset($details['bid']) ? $details['bid'] : '',
    '#title' => t('Select a block'),
  );
  $form['block']['hide_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the title of this block'),
    '#default_value' => isset($details['hide_title']) ? $details['hide_title'] : 1,
  );

  $form['node']['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Node'),
    '#description' => t('The node ID of the node.'),
    '#maxlength' => 10,
    '#size' => 20,
    '#default_value' => isset($details['nid']) ? $details['nid'] : '',
  );
  $form['node']['teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Teaser view'),
    '#default_value' => isset($details['teaser']) ? $details['teaser'] : 0,
  );
  $form['node']['hide_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the title of this node'),
    '#default_value' => isset($details['hide_title']) ? $details['hide_title'] : 1,
  );

  $form['qtabs']['machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Quicktab'),
    '#description' => t('The quicktab machine name.') .' '. t('Different styles may not work when putting an ajax quicktab inside ajax quicktab.'),
    '#maxlength' => 10,
    '#size' => 20,
    '#default_value' => isset($details['machine_name']) ? $details['machine_name'] : '',
  );

  $form['callback']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Callback Path'),
    '#description' => t('The path to your custom menu callback.') .' '. t('This should be a special menu callback that can deliver content specially for use in Quicktabs.'),
    '#size' => 20,
    '#default_value' => isset($details['path']) ? $details['path'] : '',
  );
  
  $form['type'] = array(
    '#type' => 'radios',
    '#options' => $tabtypes,
    '#default_value' => isset($details['type']) ? $details['type'] : 'block',
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#prefix' => '<div>',
    '#suffix' => '<label for="edit-remove">'. t('Delete') .'</label></div>',
    '#value' => 'remove_'. $delta,
    '#attributes' => array('class' => 'delete-tab', 'title' => t('Click here to delete this tab.')),
    '#submit' => array('qt_remove_tab_submit'),
    '#ahah' => array(
      'path' => 'quicktabs/ahah',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

/**
 * Theme function for quicktabs admin page.
 * Theme the form elements for the tabs as draggable table rows.
 *
 * @ingroup themeable
 */
function theme_quicktabs_admin_form_tabs($form) {
  drupal_add_tabledrag('qt-tablist-table', 'order', 'sibling', 'qt-tabs-weight');
  $rows = array();
  $headers = array(
    t('Tab title'),
    t('Tab weight'),
    t('Tab type'),
    t('Tab content'),
    t('Operations'),
  );

  foreach (element_children($form) as $key) {
    $form[$key]['weight']['#attributes']['class'] = 'qt-tabs-weight';
    // Build the table row.
    $row = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['title']), 'class' => 'qt-tab-title'),
        array('data' => drupal_render($form[$key]['weight']), 'class' => 'qt-tab-weight'),
        array('data' => drupal_render($form[$key]['type']), 'class' => 'qt-tab-type'),
        // tab content (only 1 tab content (block, node or view) will be displayed. see: quicktabs_form.js)
        array('data' => drupal_render($form[$key]['block']), 'class' => 'qt-tab-content qt-tab-block-content'),
        array('data' => (module_exists('views') ? drupal_render($form[$key]['view']) : ''), 'class' => 'qt-tab-content qt-tab-view-content'),
        array('data' => drupal_render($form[$key]['node']), 'class' => 'qt-tab-content qt-tab-node-content'),
        array('data' => drupal_render($form[$key]['callback']), 'class' => 'qt-tab-content qt-tab-callback-content'),
        array('data' => drupal_render($form[$key]['qtabs']), 'class' => 'qt-tab-content qt-tab-qtabs-content'),
        array('data' => drupal_render($form[$key]['remove']), 'class' => 'qt-tab-remove'),
      ),
      'class' => 'draggable',
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  $output = theme('table', $headers, $rows, array('id' => 'qt-tablist-table'));
  $output .= drupal_render($form);

  // Add our JS file, which has some Drupal core JS overrides, and ensures ahah behaviours get re-attached
  drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs_ahah.js', 'module', 'footer');
  drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs_form.js');
  drupal_add_css(drupal_get_path('module', 'quicktabs') .'/css/quicktabs-admin.css');
  return $output;
}

/**
 * Submit handler for the "Add Tab" button.
 */
function qt_more_tabs_submit($form, &$form_state) {
  $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state);
  // Add an empty tab at the end.
  $quicktabs['tabs'][] = array();

  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Tab" button.
 */
function qt_remove_tab_submit($form, &$form_state) {
  // Get the tab delta for the clicked button.
  $delta = $form_state['clicked_button']['#parents'][1];
  // Copy the form_state because unset() does not work on variable
  // that is passed by reference.
  $form_state_copy = $form_state;
  unset($form_state_copy['values']['tabs'][$delta]);
  $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state_copy);

  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the Views drop down.
 */
function qt_get_displays_submit($form, &$form_state) {
  $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state);

  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
}

/**
 * ahah callback.
 */
function quicktabs_ahah() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $qt_form = $form['qt_wrapper']['tabs'];
  unset($qt_form['#prefix'], $qt_form['#suffix']); // Prevent duplicate wrappers.
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array(
    'status'   => TRUE,
    'data'     => theme('status_messages') . drupal_render($qt_form),
    'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
  ));
}

/**
 * Validation handler for quicktabs admin page.
 */
function quicktabs_form_validate($form, &$form_state) {
  // We don't want it to validate when we're just adding or removing tabs.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    if (empty($form_state['values']['machine_name'])) {
      form_set_error('machine_name', t('The quicktabs ID is required.'));
    }
    else if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['machine_name'])) {
      form_set_error('type', t('The quicktabs ID must contain only lowercase letters, numbers, and underscores.'));
    }
    if (empty($form_state['values']['title'])) {
      form_set_error('title', t('Title is required for the quicktab block.'));
    }
    else {
      if (!isset($form_state['values']['tabs'])) {
        form_set_error('', t('At least one tab should be created.'));
      }
      foreach ($form_state['values']['tabs'] as $j => $tab) {
        if (empty($tab['title'])) {
          form_set_error('tabs]['. $j .'][title', t('Title is required for each tab.'));
        }
        elseif ($tab['type'] == 'qtabs' && $tab['qtabs']['machine_name'] == $form_state['values']['machine_name']) {
          form_set_error('tabs]['. $j .'][qtabs][machine_name', t('You cannot put a quicktab inside itself.'));
        }
        elseif ($tab['type'] == 'view') {
          // Remove spaces from view arguments.
          $form_state['values']['tabs'][$j]['view']['args'] = str_replace(' ', '', $form_state['values']['tabs'][$j]['view']['args']);
        }
        elseif ($tab['type'] == 'callback') {
          $menu_item = menu_get_item($tab['callback']['path']);
          if (!$menu_item || $menu_item['type'] != MENU_CALLBACK) {
            form_set_error('tabs]['. $j .'][callback][path', t('You must enter a valid menu callback for the path.'));
          }
        }
      }
    }
  }
}

/**
 * Submit handler for quicktabs admin page.
 */
function quicktabs_form_submit($form, &$form_state) {
  // We don't want it to submit when we're just adding or removing tabs.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state);
    drupal_alter('quicktabs', $quicktabs, 'presave');

    $exists = quicktabs_load($quicktabs['machine_name']);
    if ($exists && empty($exists['in_code_only'])) {
      $ret = drupal_write_record('quicktabs', $quicktabs, 'machine_name');
      if ($ret == SAVED_UPDATED) {
        drupal_set_message(t('The quicktab block has been updated.'));
      }
    }
    else {
      $ret = drupal_write_record('quicktabs', $quicktabs);
      if ($ret == SAVED_NEW) {
        drupal_set_message(t('The quicktab block has been created.'));
      }
    }
    drupal_goto('admin/build/quicktabs');
  }
}

/**
 * Deletion of quicktab block.
 */
function quicktabs_block_delete(&$form_state, $quicktabs) {
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $quicktabs['machine_name']);
  $form['title'] = array('#type' => 'hidden', '#value' => $quicktabs['title']);
  return confirm_form($form, t('Are you sure you want to delete the quicktab block %name?', array('%name' => $quicktabs['title'])), 'admin/build/quicktabs', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for quicktab block deletion.
 */
function quicktabs_block_delete_submit($form, &$form_state) {
  db_query("DELETE FROM {quicktabs} WHERE machine_name = '%s'", $form_state['values']['machine_name']);
  drupal_set_message(t('The quicktab block %name has been removed.', array('%name' => $form_state['values']['title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/build/quicktabs';
};

/**
 * Export form for quicktabs.
 */
function quicktabs_export_form(&$form_state, $quicktabs) {
  ctools_include('export');

  // Generate export code
  $code = '$items = array();' ."\n";
  $code .= ctools_export_object('quicktabs', (object) $quicktabs, '');
  $code .= '$items["'. $quicktabs['machine_name'] .'"] = $quicktabs;' ."\n";
  $code .= 'return $items;';

  // Create form
  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#default_value' => $code,
    '#rows' => substr_count($code, "\n") + 1,
    '#resizable' => FALSE,
    '#description' => t('Place this code in your module\'s implementation of <code>hook_quicktabs_default_quicktabs()</code> to provide it as a default quicktab.'),
  );
  $form['done'] = array(
    '#type' => 'submit',
    '#value' => t('Done'),
  );
  $form['#redirect'] = 'admin/build/quicktabs';

  return $form;  
}

/**
 * Callback function for admin/settings/quicktabs. Display the settings form.
 */
function quicktabs_settings() {
  $options = array();
  $styles = module_invoke_all('quicktabs_tabstyles');
  // The keys used for options must be valid html id-s.
  // Removing the css file path, because that can't be used.
  foreach ($styles as $style) {
    $options[$style] = $style;
  }
  ksort($options);

  $form['quicktabs_tabstyle'] = array(
    '#type' => 'radios',
    '#title' => t('Quicktab styles'),
    '#options' => array('nostyle' => t('No style')) + $options,
    '#default_value' => variable_get('quicktabs_tabstyle', 'nostyle'),
    '#description' => t('Select the default style for quicktabs.'),
    '#attributes' => array('class' => 'quicktabs-tabstyles clear-block'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for QuickTabs settings.
 */
function quicktabs_settings_submit($form, &$form_state) {
  variable_set('quicktabs_tabstyle', $form_state['values']['quicktabs_tabstyle']);
  drupal_set_message(t('The default quicktab style has been saved.'));
}

/**
 * Theme function for quicktabs settings page.
 *
 * @ingroup themeable
 */
function theme_quicktabs_settings($form) {
  drupal_add_css(drupal_get_path('module', 'quicktabs') .'/css/quicktabs-admin.css');

  $tabs['one'] = array('title' => t('One'), 'type' => 'freetext', 'text' => t('First tab'));
  $tabs['two'] = array('title' => t('Two'), 'type' => 'freetext', 'text' => t('Second tab'));
  $tabs['three'] = array('title' => t('Three'), 'type' => 'freetext', 'text' => t('Third tab'));
  $quicktabs['tabs'] = $tabs;
  $quicktabs['ajax'] = FALSE;
  $quicktabs['hide_empty_tabs'] = FALSE;

  // Preview for each style.
  foreach ($form['quicktabs_tabstyle']['#options'] as $style => $style_title) {
    $quicktabs['style'] = $style;
    $quicktabs['machine_name'] = drupal_strtolower($style);
    $preview = '<div class="quicktabs-preview">'. theme('quicktabs', $quicktabs) .'</div>';
    $form['quicktabs_tabstyle'][$style]['#description'] = t('%style preview', array('%style' => $style_title)) .':<br />'. $preview;
  }
  return drupal_render($form);
}

/**
 * Helper function to get all views.
 */
function quicktabs_get_views() {
  $enabled_views = array();
  $views = views_get_all_views();

  foreach ($views as $view) {
    // Skip disabled views.
    if (!empty($views[$view->name]->disabled)) {
      continue;
    }
    $enabled_views[$view->name] = $view->name;
  }
  ksort($enabled_views);
  return $enabled_views;
}

/**
 * Helper function to get all blocks.
 */
function quicktabs_get_blocks() {
  static $blocksarray;
  if (empty($blocksarray)) {
    global $theme_key;
    init_theme();
    $result = db_query("SELECT * FROM {blocks} WHERE theme = '%s'", $theme_key);
    $old_blocks = array();
    while ($old_block = db_fetch_array($result)) {
      $old_blocks[$old_block['module']][$old_block['delta']] = $old_block;
    }
    $blocks = array();
    foreach (module_list() as $module) {
      $module_blocks = module_invoke($module, 'block', 'list');
      if ($module_blocks) {
        foreach ($module_blocks as $delta => $block) {
          if (empty($old_blocks[$module][$delta])) {
            // If it's a new block, add identifiers.
            $block['module'] = $module;
            $block['delta']  = $delta;
            $block['theme']  = $theme_key;
            $blocks[] = $block;
          }
          else {
            $old_blocks[$module][$delta]['info'] = $block['info'];
            // Add this block to the list of blocks we return.
            $blocks[] = $old_blocks[$module][$delta];
          }
        }
      }
    }
    $blocksarray = array();
    foreach ($blocks as $i => $block) {
      if ($block['module'] != 'quicktabs') {
        $key = $block['module'] .'_delta_'. $block['delta'];
        $blocksarray[$key] = $block['info'];
      }
    }
  }
  return $blocksarray;
}

/**
 * Helper function to get all view displays.
 */
function _quicktabs_get_views_displays($view_name) {
  $displays = array();
  if (empty($view_name)) {
    // No view.
    return $displays;
  }

  $views = views_get_all_views();
  $view = $views[$view_name];

  if (empty($view->display)) {
    // This view is broken.
    return $displays;
  }

  foreach ($view->display as $id => $display) {
    $displays[$id] = $id .': '. $display->display_title;
  }
  return $displays;
}

/**
 * Helper function to convert the data on admin form into quicktabs presentation.
 */
function _quicktabs_convert_form_to_quicktabs($form_state) {
  $formvalues_tabs = array();
  if (isset($form_state['values']['tabs'])) {
    foreach ($form_state['values']['tabs'] as $j => $tab) {
      $formvalues_tabs[$j] = $tab[$tab['type']];
      $formvalues_tabs[$j]['title'] = $tab['title'];
      $formvalues_tabs[$j]['weight'] = $tab['weight'];
      $formvalues_tabs[$j]['type'] = $tab['type'];
      $weight[$j] = $tab['weight'];
    }
    array_multisort($weight, SORT_ASC, $formvalues_tabs);
  }
  $quicktabs = array(
    'title' => $form_state['values']['title'],
    'ajax' => $form_state['values']['ajax'],
    'hide_empty_tabs' => $form_state['values']['hide_empty_tabs'],
    'default_tab' => isset($form_state['values']['default_tab']) ? $form_state['values']['default_tab'] : 0,
    'style' => $form_state['values']['style'],
    'tabs' => $formvalues_tabs,
  );
  if (isset($form_state['values']['machine_name'])) {
    $quicktabs['machine_name'] = $form_state['values']['machine_name'];
  }
  else {
    $quicktabs['machine_name'] = 0;
  }
  return $quicktabs;
}
