<?php

/**
 * A handler to provide proper displays for dates.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_custom_date extends views_handler_field_date {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $time = REQUEST_TIME;

    $form['date_format'] = array(
      '#type' => 'select',
      '#title' => t('Date format'),
      '#options' => array(
        'small' => format_date($time, 'short'),
        'medium' => format_date($time, 'medium'),
        'large' => format_date($time, 'long'),
        'custom' => t('Custom'),
        'raw time ago' => t('Time ago'),
        'time ago' => t('Time ago (with "ago" appended)'),
        'raw time span' => t('Time span (future dates start with - )'),
        'time span' => t('Time span (with "ago/hence" appended)'),
        'ideal format' => t('Ideal Format'),
      ),
      '#default_value' => isset($this->options['date_format']) ? $this->options['date_format'] : 'small',
    );
    $form['custom_date_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom date format'),
      '#description' => t('If "Custom", see <a href="http://us.php.net/manual/en/function.date.php" target="_blank">the PHP docs</a> for date formats. If "Time ago" this is the the number of different units to display, which defaults to two.'),
      '#default_value' => isset($this->options['custom_date_format']) ? $this->options['custom_date_format'] : '',
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-options-date-format' => array('custom', 'raw time ago', 'time ago', 'raw time span', 'time span')),
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    $format = $this->options['date_format'];
    if (in_array($format, array('custom', 'raw time ago', 'time ago', 'raw time span', 'time span'))) {
      $custom_format = $this->options['custom_date_format'];
    }

    if (!$value) {
      // TODO Please change this theme call to use an associative array for the $variables parameter.
      return theme('views_nodate');
    }
    else {
      $time_diff = REQUEST_TIME - $value; // will be positive for a datetime in the past (ago), and negative for a datetime in the future (hence)
      switch ($format) {
        case 'raw time ago':
          return format_interval($time_diff, is_numeric($custom_format) ? $custom_format : 2);
        case 'time ago':
          return t('%time ago', array('%time' => format_interval($time_diff, is_numeric($custom_format) ? $custom_format : 2)));
        case 'raw time span':
          return ($time_diff < 0 ? '-' : '') . format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2);
        case 'time span':
          return t(($time_diff < 0 ? '%time hence' : '%time ago'), array('%time' => format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2)));
        case 'custom':
          return format_date($value, $format, $custom_format);
        case 'ideal format':
          return _get_time_format($value);
        default:
          return format_date($value, $format);
      }
    }
  }
}
