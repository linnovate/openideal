<?php
//$Id: calendar.views_default.inc,v 1.1.2.21 2009/02/21 16:33:15 karens Exp $
/**
 * Set up so it can be used as an API to create default calendars for
 * specific date fields.
 * 
 * Use variable_set() to establish criteria for default calendars.
 * Set the variable in custom modules or in settings. 
 * 
 * Example: Add a new default calendar to custom
 * calendars that are already configured:
 *  
 * $options = variable_get('calendar_default_view_options', array());
 * $option = array(
 *   'name' => 'example_event',
 *   'description' => 'An example event calendar for the date field.',
 *   'path' => 'example_event',
 *   'types' => array('example_content_type'),
 *   'date_fields' => array('field_example_date'),
 *   );
 * $options[] = $option;
 * variable_set('calendar_default_view_options', $options);
 * 
*/
function calendar_views_default_views() {
  $views = array();
  
  // Construct the default view with default options.
  $view = calendar_views_construct();
  $views[$view->name] = $view;
  
  // Then see if there are any custom calendars to be created 
  // using variable_get().
  $calendar_options = variable_get('calendar_default_view_options', array());
  
  foreach ((array) $calendar_options as $calendar_option) {
    $view = calendar_views_construct($calendar_option);
    $views[$view->name] = $view;
  }
  return $views;
}

/**
 * 
 * Construct a default calendar to match specified options.
 * Views calls it without options, so the basic 
 * default view will use the default values.
 * 
 * @param $options: an optional array of options to 
 * create default calendars.
 * 
 * Possible options include:
 * @param string $name:           
 *   The view name, if empty, defaults to 'calendar'.
 * @param string $description:           
 *   The view description, if empty, defaults to generic description.
 * @param string $path:           
 *   The view url, if empty, defaults to 'calendar'.
 * @param array  $types:          
 *   Array of content types to limit the calendar to those types.  
 *   If empty, defaults to no type filter.
 * @param array  $date_fields:    
 *   Date fields used to filter the calendar.
 *   If empty, defaults to array('changed') for node.changed.
 * @param array  $display_fields: 
 *   Fields to display in the calendar.
 *   If empty, defaults to title and date fields.
 * 
 * @return the default calendar array.
 */
function calendar_views_construct($options = NULL) {

$name = NULL;
$description = NULL;
$path = NULL;
$types = NULL;
$date_fields = NULL;
$display_fields = NULL;
  
if (empty($options)) {
  $disabled = TRUE;
}
else {
  $disabled = FALSE;
  extract($options);
}
if (empty($name)) {
  $name = 'calendar';
}
if (empty($description)) {
  $description = 'A multi-dimensional calendar view with back/next navigation.';
}
if (empty($path)) {
  $path = 'calendar';
}
if (empty($types)) {
  $types = array();
}
if (empty($date_fields)) {
  $date_fields = array('changed');
}
$colors = array();
$date_link_type = '';
foreach ($types as $type => $label) {
  $colors[0][$type] = '#ffffff';
  $date_link_type = $type;
}
// Can handle core node date fields or CCK date fields.

$fields = array();
$alias_fields = $date_fields;
$sort_fields = array();
$upcoming_fields = array();

foreach ($date_fields as $key => $field_name) {
  if (substr($field_name, 0, 6) == 'field_') {
    $table = 'node_data_'. $field_name;
    $alias_fields[$key] = $field_name .'_value';
    $alias = $table .'.'. $field_name .'_value';
    $cck_field = TRUE;
  }
  else {
    $table = 'node';
    $alias_fields[$key] = $field_name;
    $alias = $table .'.'. $field_name;
    $cck_field = FALSE;
  }
  $fields[$alias] = $alias;
  
  // Add a sort for each date field:
  $sort_fields[$field_name] = array(
    'order' => 'ASC',
    'delta' => '-1',
    'id' => $field_name . ($cck_field ? '_value' : ''),
    'table' =>  $table,
    'field' => $field_name . ($cck_field ? '_value' : ''),
    'relationship' => 'none',
  );
}

// Set up fields section with some of the basic options.
// Won't handle all possible options, but should cover
// the main ones needed for Drupal core and CCK fields.

if (empty($display_fields)) {
  $display_fields = array('title' => array());
  foreach ($date_fields as $field) {
    $display_fields[$field] = array();
  }
}

foreach ($display_fields as $key => $value) {
  $cck_field = substr($key, 0, 6) == 'field_' ? TRUE : FALSE;
  $date_field = $cck_field && in_array($key, $date_fields) ? TRUE : FALSE;
  $display_fields[$key]['label'] = '';
  $display_fields[$key]['link_to_node'] = $key == 'title' ? 1 : 0;
  $display_fields[$key]['exclude'] = 0;
  $display_fields[$key]['id'] = $date_field ? $key .'_value' : $key;
  $display_fields[$key]['field'] = $date_field ? $key .'_value' : $key;
  $display_fields[$key]['table'] = $cck_field ? 'node_data_'. $key : 'node';
  $display_fields[$key]['relationship'] = 'none';
  if (in_array($key, array('changed'))) {
    $display_fields[$key]['date_format'] = 'small';
  }
  elseif ($cck_field) {
    $display_fields[$key]['label_type'] = 'none';
    $display_fields[$key]['format'] = 'time';
    $display_fields[$key]['multiple'] = array(
      'group' => 0,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => 0,
      );
  }
  // Upcoming and iCal fields should display the whole date, not just time.
  $upcoming_fields[$key] = $display_fields[$key];
  $upcoming_fields[$key]['format'] = 'default';
}

$filters = array(
  'status' => array(
    'operator' => '=',
    'value' => 1,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
);
// Limit to types provided:
if (!empty($types)) {
  $filters += array(
    'type' => array(
    'operator' => 'in',
    'value' => drupal_map_assoc(array_keys($types)),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  );
}
// Filters for Upcoming and iCal views:
$upcoming_filters = $filters + array(
  'date_filter' => array(
    'operator' => '>=',
    'value' => array(
      'value' => NULL,
      'min' => NULL,
      'max' => NULL,
      'default_date' => 'now',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'date_fields' => $fields,
    'granularity' => 'day',
    'form_type' => 'date_select',
    'default_date' => 'now',
    'default_to_date' => '',
    'id' => 'date_filter',
    'table' => 'node',
    'field' => 'date_filter',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
);

$view = new view;
$view->name = $name;
$view->description = $description;
$view->tag = 'Calendar';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = $disabled; /* Edit this to true to make a default view disabled initially */

// Defaults.
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', $display_fields);
$handler->override_option('sorts', $sort_fields);

$handler->override_option('arguments', array(
  'date_argument' => array(
    'default_action' => 'default',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'default_argument_type' => 'date',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'date_fields' => $fields,
    'year_range' => '-3:+3',
    'date_method' => 'OR',
    'granularity' => 'month',
    'id' => 'date_argument',
    'table' => 'node',
    'field' => 'date_argument',
    'relationship' => 'none',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_php' => '',
    'override' => array(
      'button' => 'Override',
    ),
    'default_options_div_prefix' => '',
  ),
));
$handler->override_option('filters', $filters);
$handler->override_option('access', array(
  'type' => 'none',
  'role' => array(),
  'perm' => '',
));
$handler->override_option('title', 'Calendar');
if (!empty($header)) {
  $handler->override_option('header', $header);
  // The only format we can be sure of is filtered.
  $handler->override_option('header_format', '1');
}
$handler->override_option('header_empty', 1);
$handler->override_option('items_per_page', 0);
$handler->override_option('use_more', 0);
$handler->override_option('style_plugin', 'calendar_nav');
$handler->override_option('style_options', array(
));

// Calendar page.
$handler = $view->new_display('calendar', 'Calendar page', 'calendar_1');
$handler->override_option('style_options', array());
$handler->override_option('path', $path);
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler->override_option('calendar_colors', array($colors));
$handler->override_option('calendar_colors_vocabulary', array());
$handler->override_option('calendar_colors_taxonomy', array());
$handler->override_option('calendar_popup', 0);
$handler->override_option('calendar_date_link', $date_link_type);
// Calendar block.
$handler = $view->new_display('calendar_block', 'Calendar block', 'calendar_block_1');
$handler->override_option('style_options', array());
$handler->override_option('block_description', 'Calendar');
$handler->override_option('block_caching', -1);

// Year view.
$handler = $view->new_display('calendar_period', 'Year view', 'calendar_period_1');
$handler->override_option('style_plugin', 'calendar_style');
$handler->override_option('style_options', array(
  'display_type' => 'year',
  'name_size' => 1,
  'max_items' => 0,
));
$handler->override_option('attachment_position', 'after');
$handler->override_option('inherit_arguments', TRUE);
$handler->override_option('inherit_exposed_filters', TRUE);
$handler->override_option('displays', array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
));
$handler->override_option('calendar_type', 'year');

// Month view.
$handler = $view->new_display('calendar_period', 'Month view', 'calendar_period_2');
$handler->override_option('style_plugin', 'calendar_style');
$handler->override_option('style_options', array(
  'display_type' => 'month',
  'name_size' => '99',
  'with_weekno' => '1',
  'date_fields' => NULL,
  'max_items' => 0,
));
$handler->override_option('attachment_position', 'after');
$handler->override_option('inherit_arguments', TRUE);
$handler->override_option('inherit_exposed_filters', TRUE);
$handler->override_option('displays', array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
));
$handler->override_option('calendar_type', 'month');

// Day view.
$handler = $view->new_display('calendar_period', 'Day view', 'calendar_period_3');
$handler->override_option('style_plugin', 'calendar_style');
$handler->override_option('style_options', array(
  'name_size' => '99',
  'with_weekno' => 0,
  'max_items' => 0,
  'max_items_behavior' => 'more',
  'groupby_times' => 'hour',
  'groupby_times_custom' => '',
  'groupby_field' => '',
));
$handler->override_option('attachment_position', 'after');
$handler->override_option('inherit_arguments', TRUE);
$handler->override_option('inherit_exposed_filters', TRUE);
$handler->override_option('displays', array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
));
$handler->override_option('calendar_type', 'day');

// Week view.
$handler = $view->new_display('calendar_period', 'Week view', 'calendar_period_4');
$handler->override_option('style_plugin', 'calendar_style');
$handler->override_option('style_options', array(
  'name_size' => '99',
  'with_weekno' => 0,
  'max_items' => 0,
  'max_items_behavior' => 'more',
  'groupby_times' => 'hour',
  'groupby_times_custom' => '',
  'groupby_field' => '',
));
$handler->override_option('attachment_position', 'after');
$handler->override_option('inherit_arguments', TRUE);
$handler->override_option('inherit_exposed_filters', TRUE);
$handler->override_option('displays', array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
));
$handler->override_option('calendar_type', 'week');

// Block view.
$handler = $view->new_display('calendar_period', 'Block view', 'calendar_period_5');
$handler->override_option('style_plugin', 'calendar_style');
$handler->override_option('style_options', array(
  'display_type' => 'month',
  'name_size' => '1',
));
$handler->override_option('attachment_position', 'after');
$handler->override_option('inherit_arguments', TRUE);
$handler->override_option('inherit_exposed_filters', TRUE);
$handler->override_option('displays', array(
  'calendar_1' => 0,
  'default' => 0,
  'calendar_block_1' => 'calendar_block_1',
));
$handler->override_option('calendar_type', 'month');


// iCal feed.
if (module_exists('calendar_ical')) {
$handler = $view->new_display('calendar_ical', 'iCal feed', 'calendar_ical_1');
$handler->override_option('arguments', array());
$handler->override_option('filters', $upcoming_filters);
$handler->override_option('style_plugin', 'ical');
$handler->override_option('style_options', array(
  'mission_description' => FALSE,
  'description' => '',
  'summary_field' => 'node_title',
  'description_field' => '',
  'location_field' => '',
));
$handler->override_option('row_plugin', '');
$handler->override_option('path', $path .'/ical');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler->override_option('displays', array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 'calendar_block_1',
));
$handler->override_option('sitename_title', FALSE);
}

// Upcoming events block.
$handler = $view->new_display('block', 'Upcoming', 'block_1');
$handler->override_option('fields', $upcoming_fields);
$handler->override_option('arguments', array());
$handler->override_option('filters', $upcoming_filters);
$handler->override_option('use_more', 1);
$handler->override_option('items_per_page', 5);
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'grouping' => '',
  'type' => 'ul',
));
$handler->override_option('title', 'Upcoming');
$handler->override_option('block_description', 'Upcoming');
$handler->override_option('block_caching', -1);

return $view;
}