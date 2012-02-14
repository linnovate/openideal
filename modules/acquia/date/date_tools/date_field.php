$content[type]  = array (
  'name' => 'Event',
  'type' => 'event',
  'description' => 'Events have a start date and an optional end date.',
  'title_label' => 'Title',
  'body_label' => 'Body',
  'min_word_count' => '0',
  'help' => '',
  'node_options' => 
  array (
    'status' => true,
    'promote' => true,
    'sticky' => false,
    'revision' => false,
  ),
  'comment' => 2,
  'old_type' => 'event',
  'orig_type' => 'event',
  'module' => 'node',
  'custom' => '1',
  'modified' => '1',
  'locked' => '0',
);
$content[fields]  = array (
  0 => 
  array (
    'label' => 'Date',
    'field_name' => 'field_event_date',
    'type' => 'date',
    'widget_type' => 'date_select',
    'change' => 'Change basic information',
    'weight' => '-2',
    'default_value' => 'now',
    'default_value_code' => '',
    'default_value2' => 'blank',
    'default_value_code2' => '',
    'input_format' => 'Y-m-d H:i:s',
    'input_format_custom' => '',
    'year_range' => '-3:+3',
    'increment' => '15',
    'advanced' => 
    array (
      'label_position' => 'above',
      'text_parts' => 
      array (
        'year' => 0,
        'month' => 0,
        'day' => 0,
        'hour' => 0,
        'minute' => 0,
        'second' => 0,
      ),
    ),
    'label_position' => 'above',
    'text_parts' => 
    array (
    ),
    'description' => '',
    'group' => false,
    'required' => 0,
    'multiple' => '0',
    'repeat' => 0,
    'todate' => 'optional',
    'granularity' => 
    array (
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
    ),
    'output_format_date' => 'l, j F, Y - H:i',
    'output_format_custom' => '',
    'output_format_date_long' => 'l, F j, Y - H:i',
    'output_format_custom_long' => '',
    'output_format_date_medium' => 'D, m/d/Y - H:i',
    'output_format_custom_medium' => '',
    'output_format_date_short' => 'm/d/Y - H:i',
    'output_format_custom_short' => '',
    'tz_handling' => 'date',
    'timezone_db' => 'UTC',
    'repeat_collapsed' => '0',
    'op' => 'Save field settings',
    'module' => 'date',
    'widget_module' => 'date',
    'columns' => 
    array (
      'value' => 
      array (
        'type' => 'varchar',
        'length' => 20,
        'not null' => false,
        'sortable' => true,
      ),
      'value2' => 
      array (
        'type' => 'varchar',
        'length' => 20,
        'not null' => false,
        'sortable' => true,
      ),
      'timezone' => 
      array (
        'type' => 'varchar',
        'length' => 50,
        'not null' => false,
        'sortable' => true,
      ),
      'offset' => 
      array (
        'type' => 'int',
        'not null' => false,
        'sortable' => true,
      ),
      'offset2' => 
      array (
        'type' => 'int',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' => 
    array (
      'label' => 
      array (
        'format' => 'above',
      ),
      'teaser' => 
      array (
        'format' => 'default',
      ),
      'full' => 
      array (
        'format' => 'default',
      ),
      4 => 
      array (
        'format' => 'default',
      ),
    ),
  ),
);
