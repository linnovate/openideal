<?php
// $Id: date.views_convert.inc,v 1.1.2.1 2009/04/30 11:44:52 karens Exp $

/**
 * Implementation of hook_views_convert().
 */
function date_views_convert($display, $type, &$view, $field, $id = NULL) {
  static $date_fields;
  if (!isset($date_fields)) {
    $date_fields = array();
    $content_types = content_types();
    foreach ($content_types as $content_type) {
      foreach ($content_type['fields'] as $content_field) {
        if ($content_field['module'] == 'date') {
          $date_fields["$content_field[field_name]_value_default"] = 'default';
          $date_fields["$content_field[field_name]_value_year"] = 'year';
          $date_fields["$content_field[field_name]_value_month"] = 'month';
          $date_fields["$content_field[field_name]_value_day"] = 'day';
          $date_fields["$content_field[field_name]_value_to|default"] = 'to|default';
          $date_fields["$content_field[field_name]_value_to|year"] = 'to|year';
          $date_fields["$content_field[field_name]_value_to|month"] = 'to|month';
          $date_fields["$content_field[field_name]_value_to|day"] = 'to|day';
        }
      }
    }
  }
  switch ($type) {
    case 'filter':
      if (isset($date_fields[$field['field']])) {
        $item = $view->get_item($display, 'filter', $id);
        switch ($date_fields[$field['field']]) {
          case 'default':
            $field_name = substr($field['field'], 0, -14);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value');
            $item['granularity'] = 'second';
            break;
          case 'year':
            $field_name = substr($field['field'], 0, -11);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value');
            $item['granularity'] = 'year';
            break;
          case 'month':
            $field_name = substr($field['field'], 0, -12);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value');
            $item['granularity'] = 'month';
            break;
          case 'day':
            $field_name = substr($field['field'], 0, -10);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value');
            $item['granularity'] = 'day';
            break;
          case 'to|default':
            $field_name = substr($field['field'], 0, -17);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value2');
            $item['granularity'] = 'second';
            break;
          case 'to|year':
            $field_name = substr($field['field'], 0, -14);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value2');
            $item['granularity'] = 'year';
            break;
          case 'to|month':
            $field_name = substr($field['field'], 0, -15);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value2');
            $item['granularity'] = 'month';
            break;
          case 'to|day':
            $field_name = substr($field['field'], 0, -13);
            $item['date_fields'] = array('node_data_'. $field_name .'.'. $field_name .'_value2');
            $item['granularity'] = 'day';
            break;
        }
        $item['table'] = 'node';
        $item['field'] = 'date_filter';
        $item['form_type'] = 'date_text';
        $item['operator'] = $field['operator'] == '<>' ? '!=' : $field['operator'];
        if ($field['value'] == 'now' || $field['options'] == 'now') {
          $item['default_date'] = 'now';
        }
        else {
          $item['value'] = $field['value'];
          $item['default_date'] = $field['options'];
        }
        $view->set_item($display, 'filter', $id, $item);
      }
      break;
  }
}
