<?php
// $Id: theme.inc,v 1.1.2.13 2009/02/27 14:10:35 karens Exp $
/**
 * Preprocess an ical feed
 */
function template_preprocess_calendar_view_ical(&$vars) {
  global $base_url;
  global $language;
 
  $view     = &$vars['view'];
  $options  = &$vars['options'];
  $items    = &$vars['rows'];
  
  $style    = &$view->style_plugin;

  // Figure out which display which has a path we're using for this feed. If there isn't
  // one, use the global $base_url
  $link_display = $view->display_handler->get_link_display();

  // Compare the link to the default home page; if it's the default home page, just use $base_url.
  if (empty($vars['link'])) {
    $vars['link'] = $base_url;
  }

  // Keep devel module from appending queries to ical export.
  $GLOBALS['devel_shutdown'] = FALSE;
  
  drupal_set_header('Content-Type: text/calendar; charset=utf-8');
  drupal_set_header('Content-Disposition: attachment; filename="calendar.ics"; ');
  require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_ical.inc');
  require_once('./'. drupal_get_path('module', 'date_api') .'/theme/theme.inc');
  require_once('./'. drupal_get_path('module', 'calendar') .'/includes/calendar.inc');
  
  $events = array();
  // Get the alias name for each of our data fields.
  foreach ($view->field as $name => $field) {
    // Some fields, like the node edit and delete links, have no alias.
    $field_alias = $field->field_alias != 'unknown' ? $field->field_alias : $name; 
    foreach (array('summary_field', 'description_field', 'location_field') as $data) {
      if ($field->field == $view->date_info->$data) {
        $$data = $field_alias;
      }
    }
  }
  // A summary field is required, default to the title.
  if (empty($summary_field) || $summary_field == 'node_title') {
    $summary_field = 'title';
  }
  foreach ($items as $node) {
    // We cannot process an event that is missing the summary info.
    if (empty($node->$summary_field)) {
      continue;
    }
    // Allow modules to affect item fields
    node_invoke_nodeapi($node, 'ical item');
    unset($node->view);
    $rrule_field = str_replace(array('_value2', '_value'), '_rrule', $node->datefield);
    $event = array();
    $event['summary'] = strip_tags($node->$summary_field);
    $event['start'] = $node->calendar_start_date;
    $event['end'] = $node->calendar_end_date;
    $event['description'] = !empty($description_field) && !empty($node->$description_field) ? $node->$description_field : '';
    $event['location'] = !empty($location_field) && !empty($node->$location_field) ? $node->$location_field : '';
    $event['url'] = !empty($node->url) ? $node->url : (is_numeric($node->nid) ? url("node/$node->nid", array('absolute' => TRUE)) : $node->nid);
    $event['uid'] = !empty($node->date_id) ? $node->date_id : $event['url'];
    $event['rrule'] = !empty($rrule_field) && !empty($node->$rrule_field) ? $node->$rrule_field : '';
    $events[$event['uid']] = $event;
  }
  
  $headertitle = filter_xss_admin($view->get_title());
  $title = variable_get('site_name', 'Drupal');
  $description = $headertitle . ($title ? ' | '. $title : '');
  
  $vars['calname'] = $description;
  $vars['events'] = $events;
  template_preprocess_date_vcalendar($vars);
}
