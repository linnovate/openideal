<?php
//$Id: date_repeat.inc,v 1.22.2.1.2.23 2009/06/09 10:33:48 karens Exp $
/**
 * @file
 * Implementation of Date Repeat API calculations for the CCK Date field.
 *
 * Consolidated here so the code isn't parsed if repeats aren't being used
 * or processed, and to make it easier to maintain.
 *
 * The current implementation adds a repeat form to the date field so the user
 * can select the repeat rules. That selection is built into an RRULE
 * which is stored in the zero position of the field values. During widget
 * validation, the rule is parsed to see what dates it will create,
 * and multiple values are added to the field, one for each repeat date.
 * That update only happens when the rule, the from date, or the to date
 * change, no need to waste processing cycles for other changes to the node
 * values.
 *
 * Lots of possible TODOs, the biggest one is figuring out the best
 * way to handle dates with no UNTIL date since we can't add an infinite
 * number of values to the field. For now, we require the UNTIL date.
 */

/**
 * Widget processing for date repeat form element.
 *
 * Create the RRULE as a top-level element rather than a delta level
 * element, we'll compute the repeat sequence in the widget validation
 * to create the element delta values.
 */
function _date_repeat_widget(&$element, $field, $items, $delta) {
  $element['rrule'] = array(
    '#type' => 'date_repeat_rrule',
    '#default_value' => isset($items[0]['rrule']) ? $items[0]['rrule'] : '',
    '#date_timezone' => $element['#date_timezone'],
    '#date_format'      => date_limit_format(date_input_format($element, $field), $field['granularity']),
    '#date_text_parts'  => (array) $field['widget']['text_parts'],
    '#date_increment'   => $field['widget']['increment'],
    '#date_year_range'  => $field['widget']['year_range'],
    '#date_label_position' => $field['widget']['label_position'],
    '#prev_value' => isset($items[0]['value']) ? $items[0]['value'] : '',
    '#prev_value2' => isset($items[0]['value2']) ? $items[0]['value2'] : '',
    '#prev_rrule' => isset($items[0]['rrule']) ? $items[0]['rrule'] : '',
    '#date_repeat_widget' => str_replace('_repeat', '', $field['widget']['type']),
    '#date_repeat_collapsed' => $field['repeat_collapsed'],
    );

  return $element;
}
/**
 * Validation for date repeat form element.
 *
 * Create multiple values from the RRULE results.
 * Lots more work needed here.
 */
function _date_repeat_widget_validate($element, &$form_state) {
  require_once('./'. drupal_get_path('module', 'date_repeat') .'/date_repeat_form.inc');
  $form_values = $form_state['values'];
  $field_name = $element['#field_name'];
  $fields = content_fields();
  $field = $fields[$field_name];
  $item = $form_values[$field_name];
  $values = date_repeat_merge($element['#post'][$field_name]['rrule'], $element['rrule']);
  
  // If no start date was set, clean up the form and return.
  // If no repeats are set, clean up the form and return.
  if (empty($form_values[$field_name]['value']) || $values['FREQ'] == 'NONE') {
    $form_values[$field_name]['rrule'] = NULL;
    form_set_value($element, array($form_values[$field_name]), $form_state);
    return;
  }

  // Require the UNTIL date for now.
  // The RRULE has already been created by this point, so go back
  // to the posted values to see if this was filled out.
  $error_field = implode('][', $element['#parents']) .'][rrule][UNTIL][datetime][date';
  if (empty($values['UNTIL']['datetime'])) {
    form_set_error($error_field, t('The UNTIL value is required for repeating dates.'));
  }
  if (form_get_errors()) {
    return;
  }

  // If the rule, the start date, or the end date have changed, re-calculate
  // the repeating dates, wipe out the previous values, and populate the
  // field with the new values.

  // TODO
  // Is it right to not do anything unless there are changes? Will that
  // confuse anyone? Commenting that out for now...
  $rrule = $form_values[$field_name]['rrule'];
  if (!empty($rrule)
    //&& ($rrule != $element['rrule']['#prev_rrule']
    //|| $form_values[$field_name][0]['value'] != $element['rrule']['#prev_value']
    //|| $form_values[$field_name][0]['value2'] != $element['rrule']['#prev_value2'])
    ) {

    $item = $form_values[$field_name];
    
    // Avoid undefined index problems on dates that don't have all parts.
    $possible_items = array('value', 'value2', 'timezone', 'offset', 'offset2');
    foreach ($possible_items as $key) {
      if (empty($item[$key])) {
        $item[$key] = '';
      }
    }    
    $value = date_repeat_build_dates($rrule, $values, $field, $item);
    form_set_value($element, $value, $form_state);
  }
  else {
    // If no changes are needed, move the RRULE back to the zero value
    // item of the field.
    form_set_value(array('#parents' => array($field_name, 0, 'rrule')), $rrule, $form_state);
    form_set_value($element['rrule'], NULL, $form_state);
  }
}

/**
 * Helper function to build repeating dates from a $node_field.
 * 
 * Pass in either the RRULE or the $form_values array for the RRULE,
 * whichever is missing will be created when needed.
 */
function date_repeat_build_dates($rrule = NULL, $values = NULL, $field, $item) {
  include_once('./'. drupal_get_path('module', 'date_api') .'/date_api_ical.inc');
  $field_name = $field['field_name'];
  
  if (empty($rrule)) {
    $rrule = date_api_ical_build_rrule($values);
  }
  elseif (empty($values)) {
    $values = date_ical_parse($rrule);
  }
    
  // By the time we get here, the start and end dates have been 
  // adjusted back to UTC, but we want localtime dates to do
  // things like '+1 Tuesday', so adjust back to localtime.
  $timezone = date_get_timezone($field['tz_handling'], $item['timezone']);
  $timezone_db = date_get_timezone_db($field['tz_handling']);
  $start = date_make_date($item['value'], $timezone_db, $field['type'], $field['granularity']);
  if ($timezone != $timezone_db){
    date_timezone_set($start, timezone_open($timezone));
  }
  if (!empty($item['value2']) && $item['value2'] != $item['value']) {
    $end = date_make_date($item['value2'], date_get_timezone_db($field['tz_handling']), $field['type'], $field['granularity']);
    date_timezone_set($end, timezone_open($timezone));
  }
  else {
    $end = $start;
  }

  $duration = date_difference($start, $end);
  $start_datetime = date_format($start, DATE_FORMAT_DATETIME);
    
  if (!empty($values['UNTIL']['datetime'])) {
    $end = date_ical_date($values['UNTIL'], $timezone);
  }
  $end_datetime = date_format($end, DATE_FORMAT_DATETIME);
  
  // Split the RRULE into RRULE and EXDATE parts.
  $parts = date_repeat_split_rrule($rrule);
  $parsed_exceptions = (array) $parts[1];
  $exceptions = array();
  foreach ($parsed_exceptions as $exception) {
    $date = date_ical_date($exception);
    $exceptions[] = date_format($date, 'Y-m-d');
  }
  
  $dates = date_repeat_calc($rrule, $start_datetime, $end_datetime, $exceptions, $timezone);
  $value = array();
  foreach ($dates as $delta => $date) {
    // date_repeat_calc always returns DATE_DATETIME dates, which is
    // not necessarily $field['type'] dates.
    // Convert returned dates back to db timezone before storing.
    $date_start = date_make_date($date, $timezone, DATE_DATETIME, $field['granularity']);  
    date_timezone_set($date_start, timezone_open($timezone_db));    
    $date_end = drupal_clone($date_start);
    date_modify($date_end, '+'. $duration .' seconds');
    $value[$delta] = array(
      'value' => date_format($date_start, date_type_format($field['type'])),
      'value2' => date_format($date_end, date_type_format($field['type'])),
      'offset' => date_offset_get($date_start),
      'offset2' => date_offset_get($date_end),
      'timezone' => $timezone,
      'rrule' => $rrule,
      );
  }
  return $value;  
}