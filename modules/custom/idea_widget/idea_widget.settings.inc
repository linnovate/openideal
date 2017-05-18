<?php
/**
 * idea widget settings  page
 * in this form can edit the tabs and fields that appear in the widget
 */
function idea_widget_admin_setting($form, &$form_state, $no_js_use = FALSE) {

   $form['#tree'] = TRUE;
   $tabs_fieldset = variable_get('oi_widget_tabs_fieldset',array());
   $num = count($tabs_fieldset);
   $form['oi_widget_tabs_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tabs in Idea Form'),
    '#prefix' => '<div id="form-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  if (empty($form_state['num_tabs'])) {
    $form_state['num_tabs'] = $num;
  }
  $data = field_info_instances("node",'idea');
  uasort($data, function($a, $b) {
    $a_weight = (is_array($a) && isset($a['widget']['weight'])) ? $a['widget']['weight'] : 0;
    $b_weight = (is_array($b) && isset($b['widget']['weight'])) ? $b['widget']['weight'] : 0;

    if ($a_weight == $b_weight) {
      return 0;
    }

    return ($a_weight < $b_weight) ? -1 : 1;
  });
  $options = array('title' => 'field_title:title');
  foreach ($data as $field_name=>$value) {
    $options[$field_name] = $field_name.':'. $value["label"];
  }
  
  for ($i = 0; $i < $form_state['num_tabs']; $i++) {
    $n = $i+1;
    $index = 'tab_'.$i;
    $form['oi_widget_tabs_fieldset'][$index] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div class="tabs-fieldset-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Tab ').$n,
      '#collapsible' => TRUE,
    );
    $form['oi_widget_tabs_fieldset'][$index]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Tab Title'),
      '#default_value' => $tabs_fieldset['tab_'.$i]['title']
    );
    $form['oi_widget_tabs_fieldset'][$index]['display_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Tab Title For display mode'),
        '#default_value' => isset($tabs_fieldset['tab_'.$i]['display_title'])? $tabs_fieldset['tab_'.$i]['display_title']: $tabs_fieldset['tab_'.$i]['title']
    );
    $form['oi_widget_tabs_fieldset'][$index]['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#default_value' => $tabs_fieldset['tab_'.$i]['weight']
    );
   $form['oi_widget_tabs_fieldset'][$index]['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Idea Fields'),
    '#collapsible' => TRUE,
   );
   $form['oi_widget_tabs_fieldset'][$index]['fields']['title']['is_checked'] = array(
    '#type' => 'checkbox',
    '#title' => 'field_title:title',
    '#default_value' => $tabs_fieldset[$index]['fields']['title']['is_checked'],
    );
  $form['oi_widget_tabs_fieldset'][$index]['fields']['title']['label'] = array(
    '#type' => 'textfield',
    '#description' => 'Write the field\'s label for editing',
    '#default_value' => $tabs_fieldset[$index]['fields']['title']['label']? $tabs_fieldset[$index]['fields']['title']['label']:'Title',
    '#states' => array(
      'invisible' => array(
        ":input[name='oi_widget_tabs_fieldset[$index][fields][title][is_checked]']" => array('checked' => FALSE),
      ),
    ),
    );
    $form['oi_widget_tabs_fieldset'][$index]['fields']['title']['display_label'] = array(
        '#type' => 'textfield',
        '#description' => 'Write the field\'s label for viewing',
        '#default_value' => $tabs_fieldset[$index]['fields']['title']['display_label']? $tabs_fieldset[$index]['fields']['title']['display_label']:'Title',
        '#states' => array(
            'invisible' => array(
                ":input[name='oi_widget_tabs_fieldset[$index][fields][title][is_checked]']" => array('checked' => FALSE),
            ),
        ),
    );
   foreach ($data as $field_name => $value) {
    $form['oi_widget_tabs_fieldset'][$index]['fields'][$field_name]['is_checked'] = array(
      '#type' => 'checkbox',
      '#title' => $field_name.':'. $value["label"],
      '#default_value' => $tabs_fieldset[$index]['fields'][$field_name]['is_checked'],
    );
    
    $form['oi_widget_tabs_fieldset'][$index]['fields'][$field_name]['label'] = array(
      '#type' => 'textfield',
      '#description' => 'Write the field\'s label for editing',
      '#default_value' => $tabs_fieldset[$index]['fields'][$field_name]['label']?$tabs_fieldset[$index]['fields'][$field_name]['label']:$value["label"],
      '#states' => array(
        'invisible' => array(
          ":input[name='oi_widget_tabs_fieldset[$index][fields][$field_name][is_checked]']" => array('checked' => FALSE),
          ),
        ),
      );
     $form['oi_widget_tabs_fieldset'][$index]['fields'][$field_name]['display_label'] = array(
         '#type' => 'textfield',
         '#description' => 'Write the field\'s label for viewing',
         '#default_value' => $tabs_fieldset[$index]['fields'][$field_name]['display_label']?$tabs_fieldset[$index]['fields'][$field_name]['display_label']:$value["label"],
         '#states' => array(
             'invisible' => array(
                 ":input[name='oi_widget_tabs_fieldset[$index][fields][$field_name][is_checked]']" => array('checked' => FALSE),
             ),
         ),
     );
   }
  }

  $form['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add More Tab'),
    '#submit' => array('idea_widget_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'idea_widget_add_more_callback',
      'wrapper' => 'form-fieldset-wrapper',
    ),
  );
  if ($form_state['num_tabs'] > 1) {
    $form['remove_item'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('idea_widget_add_more_remove_one'),
      '#ajax' => array(
      'callback' => 'idea_widget_add_more_callback',
      'wrapper' => 'form-fieldset-wrapper',
      ),
    );
  }
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    if (!empty($form['remove_item']['#ajax'])) {
      unset($form['remove_item']['#ajax']);
    }
    unset($form['add_name']['#ajax']);
  }
  $form['#submit'][] = 'idea_widget_admin_setting_submit';
  return system_settings_form($form);
}

function idea_widget_add_more_add_one($form, &$form_state) {
  $form_state['num_tabs']++;
  $form_state['rebuild'] = TRUE;
}

function idea_widget_add_more_callback($form, $form_state) {
  return $form['oi_widget_tabs_fieldset'];
}

function idea_widget_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_tabs'] > 1) {
    $form_state['num_tabs']--;
  }
  $form_state['rebuild'] = TRUE;
}

function idea_widget_admin_setting_submit(&$form, &$form_state){
  $idea_widget = array();
  $tabs = $form_state['values']['oi_widget_tabs_fieldset'];
  $source_fields_data = field_info_instances("node",'idea');
  foreach($tabs as $key => $value) {
    $fields = $value['fields'];
    $fields_data = array();
    foreach ($fields as $key => $v) {
      if($v['is_checked']){
      $field = array();
      $field['name'] = $key;
      $field['description'] = $source_fields_data[$key]['description'];
      if($key == 'title'){
        $field['required'] = TRUE;
        $field['type'] = 'textfield';
      }
      else {
        $field_info = field_info_field($key);
        $field_instance = field_info_instance('node', $key, 'idea');
        $field['type'] = $source_fields_data[$key]['widget']['type'];
        $field['required'] = $source_fields_data[$key]['required'];
        if ($field_info['type'] == 'taxonomy_term_reference') {
          $myvoc = taxonomy_vocabulary_machine_name_load($field_info['settings']['allowed_values'][0]['vocabulary']);
          $tree = taxonomy_get_tree($myvoc->vid);
          if (count($tree)) {
            foreach ($tree as $term) {
              $options[$term->tid] = $term->name;
            }
          }
          $field['options'] = $options;
          $field['type'] = 'options_select';
        }
        else if ($field_info['type'] == 'entityreference') {
          $field['widget'] = $field_instance['widget']['type'];
          $field['type'] = 'entityreference';
        }
        else if($field_info['type'] == 'list_text'){
          $field['options'] = i18n_field_translate_allowed_values($field_info);
        }
        else if ($field_info['type'] ==  'datetime') {
          $field['type'] = 'datetime';
        }
        else {
          if ($field_info['type'] == 'image' || $field_info['type'] == 'file') {
            $field['type'] = $field_info['type'];
            $field['file_extensions'] = $source_fields_data[$key]['settings']['file_extensions'];
          }
          else {
            if (strpos($source_fields_data[$key]['widget']['type'], 'textarea') !== FALSE) {
              $field['type'] = 'textarea';
            }
            else if (strpos($source_fields_data[$key]['widget']['type'], 'textfield') !== FALSE) {
              $field['type'] = 'textfield';
            }
          }
        }
      }
      $field = array_merge($field,$v);
      $fields_data[] = $field;
    }
   }
   //exit();
   $idea_widget[] = array('name' => $value['title'], 'display_name' => $value['display_title'], 'weight' => $value['weight'], 'fields' => $fields_data);
  }
  variable_set('oi_idea_widget', $idea_widget);
}