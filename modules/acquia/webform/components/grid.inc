<?php
// $Id: grid.inc,v 1.3.2.20 2010/01/21 02:53:36 quicksketch Exp $

/**
 * @file
 *   Webform module grid component.
 */

/**
 * Create a default grid component.
 */
function _webform_defaults_grid() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'description' => '',
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form for editing this component.
 * Use care naming the form items, as this correlates directly to the database schema.
 * The component "Name" and "Description" fields are added to every component type and
 * are not necessary to specify here (although they may be overridden if desired).
 * @return
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_grid($currfield) {
  $edit_fields = array();
  $edit_fields['extra']['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $currfield['extra']['options'],
    '#description' => t('Options to select across the top. One option per line. Key-value pairs may be entered seperated by pipes. i.e. safe_key|Some readable option') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -3,
    '#required' => TRUE,
  );
  $edit_fields['extra']['questions'] = array(
    '#type' => 'textarea',
    '#title' => t('Questions'),
    '#default_value' => $currfield['extra']['questions'],
    '#description' => t('Questions list down the left side. One question per line.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -2,
    '#required' => TRUE,
  );
  $edit_fields['extra']['optrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Options'),
    '#default_value' => $currfield['extra']['optrand'],
    '#description' => t('Randomizes the order of options on the top when they are displayed in the form.'),
  );
  $edit_fields['extra']['qrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Questions'),
    '#default_value' => $currfield['extra']['qrand'],
    '#description' => t('Randomize the order of the questions on the side when they are displayed in the form.'),
  );
  return $edit_fields;
}

function _webform_edit_validate_grid($form_values) {
  // Currently no validation for selects.
  return TRUE;
}

function _webform_render_grid($component, $random = TRUE) {
  $form_item = array(
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_grid',
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
  );

  $questions = _webform_grid_options($component['extra']['questions']);
  $options = _webform_grid_options($component['extra']['options']);

  if ($component['extra']['optrand'] && $random) {
    // This maneuver shuffles the array keys, then uses them as
    // the basis for ordering the options.
    $aux = array();
    $keys = array_keys($options);
    shuffle($keys);
    foreach ($keys as $key) {
      $aux[$key] = $options[$key];
      unset($options[$key]);
    }
    $options = $aux;
  }
  if ($component['extra']['qrand'] && $random) {
    $aux = array();
    $keys = array_keys($questions);
    shuffle($keys);
    foreach ($keys as $key) {
      $aux[$key] = $questions[$key];
      unset($questions[$key]);
    }
    $questions = $aux;
  }
  foreach ($questions as $question) {
    if ($question != '') {
      // Remove quotes from keys to prevent HTML breakage.
      $form_item[str_replace(array('"', "'"), '', $question)] = array(
        '#title'         => $question,
        '#required'      => $component['mandatory'],
        '#prefix'        => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
        '#suffix'        => '</div>',
        '#options'       => $options,
        '#type'          => 'radios',
      );
    }
  }

  return $form_item;
}

/**
 * Display the result of a grid submission. The output of this function will be
 * displayed under the "results" tab then "submissions"
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema
 * @param $enabled
 *   If enabled, the value may be changed. Otherwise it will set to readonly.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_submission_display_grid($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_grid($component, FALSE);
  $cid = 0;
  foreach (element_children($form_item) as $key) {
    $form_item[$key]['#default_value'] = $data['value'][$cid++];
    $form_item[$key]['#disabled'] = !$enabled;
  }
  return $form_item;
}

/**
 * Translates the submitted 'safe' form values back into their un-edited
 * original form.
 *
 * @param $data
 *   The POST data associated with the component
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema
 * @return
 *   Nothing
 */
function _webform_submit_grid(&$data, $component) {
  $options = drupal_map_assoc(array_flip(_webform_grid_options($component['extra']['options'])));

  // Questions are a bit more tricky, since quotes were removed from them in
  // _webform_render_grid(). Build a list of no_quotes => with_qoutes questions.
  $questions = array();
  foreach (_webform_grid_options($component['extra']['questions']) as $key => $question) {
    $safe_question = str_replace(array('"', "'"), '', $question);
    $questions[$safe_question] = $question;
  }

  if (is_array($data)) {
    foreach ($data as $key => $value) {
      if ($value !== '') {
        $data[$key] = $options[$value];
      }
    }
  }
  elseif ($data !== '') {
    $data = $options[$data];
  }

  // Put the form in the original option order before saving.
  // Return the final data with the quotes back in place.
  $ordered_data = array();
  foreach ($questions as $safe_question => $question) {
    if (isset($data[$safe_question])) {
      $ordered_data[$question] = $data[$safe_question];
    }
  }

  $data = $ordered_data;
}
/**
 * Format the output of emailed data for this component.
 *
 * @param $data
 *   A string or array of the submitted data.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output to be included in the email.
 */
function theme_webform_mail_grid($data, $component) {
  $questions = _webform_grid_options($component['extra']['questions']);
  $output = $component['name'] .":\n";
  foreach ($questions as $key => $question) {
    $output .= '  - '. $question .':'. ($data[$question] == '' ? '' : ' '. $data[$question]) ."\n";
  }
  return $output;
}

/**
 * function _webform_help_select
 * Module specific instance of hook_help().
 */
function _webform_help_grid($section) {
  switch ($section) {
    case 'admin/settings/webform#grid_description':
      return t('Allows creation of grid questions, denoted by radio buttons.');
  }
}

/**
 * Module specific instance of hook_theme().
 */
function _webform_theme_grid() {
  return array(
    'webform_grid' => array(
      'arguments' => array('grid_element' => NULL),
    ),
    'webform_mail_grid' => array(
      'arguments' => array('data' => NULL, 'component' => NULL),
    ),
  );
}

/**
 * Calculate and returns statistics about results for this component from all
 * submission to this webform. The output of this function will be displayed
 * under the "results" tab then "analysis".
 *
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @param $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will be limited
 *   to these sids.
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_rows_grid($component, $sids = array()) {
  // Generate the list of options and questions.
  $options = _webform_grid_options($component['extra']['options']);
  $questions = array_values(_webform_grid_options($component['extra']['questions']));

  // Generate a lookup table of results.
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (".implode(",", $placeholders).")" : "";
  $query = 'SELECT no, data, count(data) as datacount '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND cid = %d '.
    " AND data != '' ". $sidfilter .
    ' GROUP BY no, data';
  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  $counts = array();
  while ($data = db_fetch_object($result)) {
    $counts[$data->no][$data->data] = $data->datacount;
  }

  // Create an entire table to be put into the returned row.
  $rows = array();
  $header = array('');

  // Add options as a header row.
  foreach ($options as $option) {
    $header[] = $option;
  }

  // Add questions as each row.
  foreach ($questions as $qkey => $question) {
    $row = array($question);
    foreach ($options as $okey => $option) {
      $row[] = !empty($counts[$qkey][$okey]) ? $counts[$qkey][$okey] : 0;
    }
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows, array('class' => 'webform-grid'));

  return array(array(array('data' => $output, 'colspan' => 2)));
}

/**
 * function _webform_table_data_select
 * Return the result of this component's submission for display in a table. The output of this function will be displayed under the "results" tab then "table"
 * @param $data An array of information containing the submission result, directly correlating to the webform_submitted_data database schema
 * @returns Textual output formatted for human reading.
 */
function _webform_table_data_grid($data, $component) {
  $questions = array_values(_webform_grid_options($component['extra']['questions']));
  $output = '';
  // Set the value as a single string.
  if (is_array($data['value'])) {
    foreach ($data['value'] as $item => $value) {
      if ($value !== '') {
        $output .= $questions[$item] .': '. check_plain($value) .'<br />';
      }
    }
  }
  else {
    $output = check_plain(!isset($data['value']['0']) ? '' : $data['value']['0']);
  }
  return $output;
}

/**
 * Return the header information for this component to be displayed in a comma
 * seperated value file. The output of this function will be displayed under
 * the "results" tab then "download".
 *
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_grid($component) {
  $header = array();
  $header[0] = array('');
  $header[1] = array($component['name']);
  $items = _webform_grid_options($component['extra']['questions']);
  $count = 0;
  foreach ($items as $key => $item) {
    // Empty column per sub-field in main header.
    if ($count != 0) {
      $header[0][] = '';
      $header[1][] = '';
    }
    // The value for this option.
    $header[2][] = $item;
    $count++;
  }

  return $header;
}

/**
 * Return the result of a textfield submission. The output of this function will
 * be displayed under the "results" tab then "submissions".
 *
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for CSV, not including either prefixed or trailing
 *   commas.
 */
function _webform_csv_data_grid($data, $component) {
  $questions = array_keys(_webform_grid_options($component['extra']['questions']));
  $return = array();
  foreach ($questions as $key => $question) {
    $return[] = isset($data['value'][$key]) ? $data['value'][$key] : '';
  }
  return $return;
}

function theme_webform_grid($grid_element) {
  $rows = array();
  $header = array('');
  $first = TRUE;
  foreach (element_children($grid_element) as $key) {
    $question_element = $grid_element[$key];

    // Set the header for the table.
    if ($first) {
      foreach ($question_element['#options'] as $option) {
        $header[] = $option;
      }
      $first = FALSE;
    }

    // Create a row with the question title.
    $row = array(check_plain($question_element['#title']));

    // Render each radio button in the row.
    $radios = expand_radios($question_element);
    foreach (element_children($radios) as $key) {
      unset($radios[$key]['#title']);
      $row[] = drupal_render($radios[$key]);
    }
    $rows[] = $row;
  }

  return theme('form_element', $grid_element, theme('table', $header, $rows, array('class' => 'webform-grid')));
}

/**
 * Utility function to split user-entered values from new-line separated
 * text into an array of options.
 */
function _webform_grid_options($text) {
  $options = array();
  $rows = array_filter(explode("\n", _webform_filter_values(trim($text))));
  foreach ($rows as $option) {
    $option = trim($option);
    if (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
      $options[$matches[1]] = $matches[2];
    }
    else {
      $options[$option] = $option;
    }
  }
  return $options;
}
