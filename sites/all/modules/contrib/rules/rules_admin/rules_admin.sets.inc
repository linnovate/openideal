<?php
// $Id: rules_admin.sets.inc,v 1.1.2.9 2009/08/03 16:42:25 fago Exp $


/**
 * @file Rules Sets Admin UI
 */

rules_include('rules_admin');

function rules_admin_sets_overview() {
  $sets = rules_get_configured_items('rule_sets');
  $sets = array_filter($sets, 'rules_admin_element_filter');
  ksort($sets);

  $header = array(t('Label'), t('Name'), t('Category'), t('Status'), t('Operations'));
  $rows = array();

  foreach ($sets as $name => $set_info) {
    $set_info += array('status' => 'default');

    $path = RULES_ADMIN_SET_PATH .'/'. $name;
    $ops = array();
    if ($set_info['status'] == 'custom') {
      $ops[] = l(t('delete'), $path .'/delete', array('query' => drupal_get_destination()));
    }
    else if ($set_info['status'] == 'altered') {
      $ops[] = l(t('revert'), $path .'/revert', array('query' => drupal_get_destination()));
    }
    $categories = isset($set_info['categories']) ? array_map('check_plain', $set_info['categories']) : array();

    $rows[] = array(
      l($set_info['label'], $path .'/edit'),
      check_plain($name),
      implode(', ', $categories),
      theme('rules_admin_configuration_status', $set_info['status']),
      implode(' ', $ops),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('There are no rule sets.'), 'colspan' => 6));
  }

  return theme('table', $header, $rows, array('class' => 'rules-sets-configurations'));
}

/**
 * Shows the delete form for items
 */
function rules_admin_item_form_delete(&$form_state, $item_type, $item) {
  $form = array();
  $form_state['item'] = $item;
  $form_state['item_type'] = $item_type;

  $path = array();
  $path['path'] = isset($_GET['destination']) ? $_GET['destination'] : RULES_ADMIN_PATH;

  return confirm_form($form,
    t('Are you sure you want to delete %label?', array('%label' => rules_get_element_label($item))),
    $path,
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

function rules_admin_item_form_delete_submit($form, &$form_state) {
  rules_item_delete($form_state['item_type'], $form_state['item']['name']);
  rules_clear_cache();
  drupal_set_message(t("%label has been deleted.", array('%label' => rules_get_element_label($form_state['item']))));
  $form_state['redirect'] = RULES_ADMIN_PATH;
}

/**
 * Shows the revert form for items
 */
function rules_admin_item_form_revert(&$form_state, $item_type, $item) {
  $form = array();
  $form_state['item'] = $item;
  $form_state['item_type'] = $item_type;

  $path = array();
  $path['path'] = isset($_GET['destination']) ? $_GET['destination'] : RULES_ADMIN_PATH;

  return confirm_form($form,
    t('Are you sure you want to revert %label?', array('%label' => rules_get_element_label($item))),
    $path,
    t('This action cannot be undone.'), t('Revert'), t('Cancel')
  );
}

function rules_admin_item_form_revert_submit($form, &$form_state) {
  rules_item_delete($form_state['item_type'], $form_state['item']['name']);
  rules_clear_cache();
  drupal_set_message(t("%label has been reverted.", array('%label' => rules_get_element_label($form_state['item']))));
  $form_state['redirect'] = RULES_ADMIN_PATH;
}

/**
 * Item type callback: Deleted rule set
 */
function rules_item_rule_set_delete($set_name) {
  // Delete all rules belonging to this set too
  $rules = rules_get_configured_items('rules');
  foreach ($rules as $name => $rule) {
    if ($rule['#set'] == $set_name) {
      rules_item_delete('rules', $name);
    }
  }
}

/**
 * Shows the rule set edit page form
 */
function rules_admin_form_edit_rule_set(&$form_state, $set_info) {
  $form_state['set'] = $set_info;
  $form = rules_admin_form_rule_set_settings($set_info);

  $form['active_header'] = array('#value' => '<br /><h3>'. t('Active rules') .'</h3>');
  $form['active'] = rules_admin_overview_table(array('set' => $set_info['name'], 'active' => TRUE));
  $form['active']['#suffix'] = '<br />';
  $form['inactive_header'] = array('#value' => '<h3>'. t('Inactive rules') .'</h3>');
  $form['inactive'] = rules_admin_overview_table(array('set' => $set_info['name'], 'active' => FALSE));

  if (variable_get('rules_show_fixed', FALSE)) {
    $form['fixed_header'] = array('#value' => '<h3>'. t('Fixed rules') .'</h3>');
    $form['fixed'] = rules_admin_overview_table(array('set' => $set_info['name'], 'active' => TRUE, 'fixed' => TRUE));
  }
  return $form;
}

/**
 * Returns the form for the settings of a rule set
 */
function rules_admin_form_rule_set_settings($set_info = array(), $add = FALSE) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rule set settings'),
    '#collapsible' => TRUE,
  );
  $form['settings']['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#description' => t('Choose an appropriate label for this rule set.'),
    '#default_value' => isset($set_info['label']) ? $set_info['label'] : '',
    '#required' => TRUE,
  );
  if ($add) {
    $form['settings']['name'] = array(
      '#title' => t('Machine readable name'),
      '#type' => 'textfield',
      '#description' => t('Specify a unique name containing only alphanumeric characters, and underscores.'),
      '#default_value' => isset($set_info['name']) ? $set_info['name'] : '',
      '#required' => TRUE,
    );
  }
  $form['settings']['categories'] = array(
    '#type' => 'textfield',
    '#title' => t('Categories'),
    '#default_value' => isset($set_info['categories']) ? implode(', ', $set_info['categories']) : '',
    '#description' => t('A comma-separated list of terms describing this rule set. Example: funny, bungee jumping.'),
    '#autocomplete_path' => RULES_ADMIN_PATH .'/autocomplete',
  );
  $form['settings']['button'] = array('#type' => 'submit', '#weight' => 10, '#value' => t('Save changes'));
  return $form;
}

function rules_admin_form_edit_rule_set_submit($form, &$form_state) {
  $set_info = $form_state['set'];
  $set_info['label'] = $form_state['values']['label'];
  if (!isset($set_info['status']) || $set_info['status'] == 'default') {
    $set_info['status'] = 'altered';
  }
  $set_info['categories'] = array_filter(array_map('trim', explode(',', $form_state['values']['categories'])));
  unset($set_info['name']);
  rules_item_save('rule_sets', $form_state['set']['name'], $set_info);
  drupal_set_message(t("The rule set %label has been updated.", array('%label' => $set_info['label'])));
}

/**
 * Form for adding a rule set
 */
function rules_admin_form_add_rule_set(&$form_state) {
  //in case of non-js we degrade to a multistep form
  $set_info = isset($form_state['set']) ? $form_state['set'] : array('arguments' => array());

  $form = rules_admin_form_rule_set_settings($set_info, TRUE);
  unset($form['settings']['button']);

  //add form for specifying arguments
  $form['args'] = array(
    '#type' => 'fieldset',
    '#title' => t('Arguments'),
    '#collapsible' => TRUE,
    '#prefix' => '<div id="rules-set-args">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'rules_admin_form_arguments',
    '#description' => t('You may specify some arguments, which have to be passed to the rule set when it is invoked. For each argument you have to specify a certain data type, a label and a unique machine readable name containing only alphanumeric characters, and underscores.'),
  );
  foreach ($set_info['arguments'] as $name => $info) {
    $form['settings']['args'][$name] = _rules_admin_form_argument($name, $info);
  }
  for ($i = 0; $i < 3; $i++) {
    $form['args'][$i] = _rules_admin_form_argument();
  }

  $form['args']['rules_more'] = array(
    '#type' => 'submit',
    '#value' => t('More arguments'),
    '#description' => t("If the amount of boxes above isn't enough, click here to add more arguments."),
    '#weight' => 1,
    '#submit' => array('rules_admin_form_add_rule_set_rebuild'), // If no javascript action.
    '#ahah' => array(
      'path' => RULES_ADMIN_SET_PATH .'/js',
      'wrapper' => 'rules-set-args',
      'method' => 'replace',
    ),
  );
  $form['button'] = array('#type' => 'submit', '#weight' => 10, '#value' => t('Save'));
  return $form;
}

/**
 * Validates the set and builds it -> $form_state['set']
 */
function rules_admin_form_add_rule_set_validate($form, &$form_state) {

  if (!eregi('^[0-9a-z_]*$', $form_state['values']['name'])) {
    form_set_error('name', t('The name may contain only digits, numbers and underscores.'));
  }

  //validate the arguments
  unset($form_state['values']['args']['rules_more']);
  foreach ($form_state['values']['args'] as $i => $values) {
    if (array_filter($values)) {
      foreach (array('name', 'type', 'label') as $key) {
        if (!$values[$key]) {
          form_set_error(implode('][', array('args', $i, $key)), t('All fields of an argument are required.'));
        }
      }
      if (!eregi('^[0-9a-z_]*$', $values['name'])) {
        form_set_error(implode('][', array('args', $i, 'name')), t('The name may contain only digits, numbers and underscores.'));
      }
    }
    else {
      unset($form_state['values']['args'][$i]);
    }
  }
  //build  the set
  $set_info = array('arguments' => array());
  $set_info['label'] = $form_state['values']['label'];
  $set_info['name'] = $form_state['values']['name'];

  foreach ($form_state['values']['args'] as $i => $values) {
    if (isset($set_info['arguments'][ $values['name'] ])) {
      form_set_error('args]['. $i .'][name', t('Each name may be used only once.'));
    }
    else {
      $set_info['arguments'][ $values['name'] ] = array(
        'label' => $values['label'],
        'type' => $values['type'],
      );
    }
  }
  $form_state['set'] = $set_info;
}

function rules_admin_form_add_rule_set_rebuild($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function _rules_admin_form_argument($name = '', $info = array()) {
  $form = array();
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#size' => 40,
    '#default_value' => isset($info['label']) ? $info['label'] : '',
  );
  $options = rules_extract_property(array_filter(rules_get_data_types(), 'rules_admin_element_filter'), 'label');
  $options = array_map('drupal_ucfirst', $options);
  asort($options);
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Data type'),
    '#options' => array(0 => '--') + $options,
    '#default_value' => isset($info['type']) ? $info['type'] : '',
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine readable variable name'),
    '#size' => 40,
    '#default_value' => $name,
  );
  return $form;
}

/**
 * A generic AHAH JS callback. Gets the cached form and passes it to the given callback
 * to alter it. The callback returns the a reference on the to be rendered form,
 * which is rendered and returned to the JS.
 */
function rules_admin_form_js($callback) {
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);

  $render = &$callback($form);

  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);
  // Render the new output.
  print drupal_to_js(array('data' => drupal_render($render), 'status' => TRUE));
  exit();
}

/**
 * rules AHAH JS callback for adding arguments
 */
function &rules_admin_form_add_rule_set_add_arguments(&$form) {
  $start = count($form['args']) - 1;
  for ($i = $start; $i - $start < 3; $i++) {
    $form['args'][$i] = _rules_admin_form_argument();
  }
  return $form['args'];
}

/**
 * Themes the rules set arguments adding form
 */
function theme_rules_admin_form_arguments($form) {
  drupal_add_js(drupal_get_path('module', 'rules_admin') .'/rules_admin.js', 'module');

  $rows = array();
  $headers = array(
    t('Data type'),
    t('Label'),
    t('Machine readable name'),
  );

  $button = drupal_render($form['rules_more']);
  unset($form['rules_more']);

  foreach (element_children($form) as $key) {
    // No need to print the field title every time.
    unset($form[$key]['label']['#title'], $form[$key]['name']['#title'], $form[$key]['type']['#title']);
    // Build the table row.
    $row = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['type']), 'class' => 'rules-argument-data-type'),
        array('data' => drupal_render($form[$key]['label']), 'class' => 'rules-argument-label'),
        array('data' => drupal_render($form[$key]['name']), 'class' => 'rules-argument-name'),
      ),
    );
    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  $output = theme('table', $headers, $rows);
  $output .= drupal_render($form) . $button;
  return $output;
}

function rules_admin_form_add_rule_set_submit($form, &$form_state) {
  $set_info = $form_state['set'];
  $set_info['status'] = 'custom';
  $set_info['categories'] = array_filter(array_map('trim', explode(',', $form_state['values']['categories'])));
  unset($set_info['name']);
  rules_item_save('rule_sets', 'rules_'. $form_state['set']['name'], $set_info);
  drupal_set_message(t("The rule set %label has been added.", array('%label' => $set_info['label'])));
  $form_state['redirect'] = RULES_ADMIN_SET_PATH;
}

