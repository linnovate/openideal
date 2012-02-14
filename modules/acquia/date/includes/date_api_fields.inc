<?php
// $Id: date_api_fields.inc,v 1.1.2.9 2009/06/08 23:23:46 karens Exp $
/**
 *  Identify all potential date/timestamp fields.
 *
 *  @return
 *    array with fieldname, type, and table.
 *  @see 
 *    date_api_date_api_fields() which implements 
 *    the hook_date_api_fields() for the core date fields.
 */
function _date_api_fields($base = 'node') {
  // Make sure $base is never empty.
  if (empty($base)) {
    $base = 'node';
  }
  $cid = 'date_api_fields_'. $base;
  cache_clear_all($cid, 'cache_views');
  
  $all_fields = date_api_views_fetch_fields($base, 'field');
  $fields = array();
  foreach ((array) $all_fields as $name => $val) {
    $fromto = array();
    $tmp = explode('.', $name);
    $field_name = $tmp[1];
    $table_name = $tmp[0];
    $alias = str_replace('.', '_', $name);
      
    if (!$handler = views_get_handler($table_name, $field_name, 'field')) {
      continue; 
    }
    
    $handler_name = $handler->definition['handler'];
    $type = '';
    
    // For cck fields, get the date type.
    $custom = array();
    if (isset($handler->content_field)) {
      if ($handler->content_field['type'] == 'date') {
        $type = 'cck_string';
      }
      elseif ($handler->content_field['type'] == 'datestamp') {
        $type = 'cck_timestamp';
      }
      elseif ($handler->content_field['type'] == 'datetime') {
        $type = 'cck_datetime';
      }
    }
    
    // Allow custom modules to provide date fields.
    // The is_a() function makes this work for any handler
    // that was derived from 'views_handler_field_date'.
    elseif (is_a($handler, 'views_handler_field_date')) {
      foreach (module_implements('date_api_fields') as $module) {
        $function = $module .'_date_api_fields';
        if ($custom = $function("$table_name.$field_name")) {
          $type = 'custom';
          break;
        }
      }
    }

    // Don't do anything if this is not a date field we can handle.
    if (!empty($type)) {

      // Handling for simple timestamp fields
      $fromto = array($alias, $alias);
      $tz_handling = 'site';
      $related_fields = array();
      $timezone_field = '';
      $offset_field = '';
      $rrule_field = '';
      $delta_field = '';
      $granularity = array('year', 'month', 'day', 'hour', 'minute');
       
      // Handling for content field dates
      if (isset($handler->content_field['tz_handling'])) {
        $tz_handling = $handler->content_field['tz_handling'];
        $db_info = content_database_info($handler->content_field);
        if ($tz_handling == 'date') {
          $offset_field = $table_name .'.'. $db_info['columns']['offset']['column'];
        }
        $related_fields = array(
          $table_name .'.'. $field_name
          );
        if (isset($db_info['columns']['value2']['column'])) {
          $related_fields = array_merge($related_fields, array($table_name .'.'. $db_info['columns']['value2']['column']));
        }
        if (isset($db_info['columns']['timezone']['column'])) {
          $related_fields = array_merge($related_fields, array($table_name .'.'. $db_info['columns']['timezone']['column']));
          $timezone_field = $table_name .'.'. $db_info['columns']['timezone']['column'];
        }
        if (isset($db_info['columns']['rrule']['column'])) {
          $related_fields = array_merge($related_fields, array($table_name .'.'. $db_info['columns']['rrule']['column']));
          $rrule_field = $table_name .'.'. $db_info['columns']['rrule']['column'];
        }
      }
      // Get the delta value into the query.
      if (!empty($handler->content_field['multiple'])) {
        array_push($related_fields, "$table_name.delta");
        $delta_field = $table_name .'_delta';
      }

      // Handling for cck fromto dates
      if (isset($handler->content_field)) {
        switch ($handler->content_field['type']) {
          case 'date':
          case 'datetime':
          case 'datestamp':
            $db_info = content_database_info($handler->content_field);
            $fromto = array(
              $table_name .'_'. $db_info['columns']['value']['column'],
              $table_name .'_'. (!empty($handler->content_field['todate']) ? $db_info['columns']['value2']['column'] : $db_info['columns']['value']['column']),
              );
            break;
        }
        $granularity = !empty($handler->content_field['granularity']) ? $handler->content_field['granularity'] : array('year', 'month', 'day', 'hour', 'minute');
      }
      
      // CCK fields append a column name to the field, others do not
      // need a real field_name with no column name appended for cck date formatters
      switch ($type) {
        case 'cck_string':
          $sql_type = DATE_ISO;
          break;
        case 'cck_datetime':
          $sql_type = DATE_DATETIME;
          break;
        default:
          $sql_type = DATE_UNIX;
          break;
      }
      $fields['name'][$name] = array(
        'type' => $type,
        'sql_type' => $sql_type,
        'label' => $val['group'] .': '. $val['title'],
        'granularity' => $granularity,
        'fullname' => $name,
        'table_name' => $table_name,
        'field_name' => $field_name,
        'query_name' => $alias,
        'fromto' => $fromto,
        'tz_handling' => $tz_handling,
        'offset_field' => $offset_field,
        'timezone_field' => $timezone_field,
        'rrule_field' => $rrule_field,
        'related_fields' => $related_fields,
        'delta_field' => $delta_field,
      );
      
      // Allow the custom fields to over-write values.
      if (!empty($custom)) {
        foreach ($custom as $key => $val) {
          $fields['name'][$name][$key] = $val;
        }
      }
      if (isset($handler->content_field)) {
        if (substr($field_name, -1) == '2') {
          $len = (strlen($field_name) - 7);
        }
        else {
          $len = (strlen($field_name) - 6);
        }
        $fields['name'][$name]['real_field_name'] = substr($field_name, 0, $len);
      }
      else {
        $fields['name'][$name]['real_field_name'] = $field_name;
      }
      $fields['alias'][$alias] = $fields['name'][$name];
    }
  }
  cache_set($cid, $fields, 'cache_views');
  return $fields;
}
