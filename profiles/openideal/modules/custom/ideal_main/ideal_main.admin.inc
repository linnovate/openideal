<?php 
/**
 *  Creating graphs for the last 10 days of node creating, comments and votes.
 */
function ideal_dashboard() { 
  $period = _ideal_main_get_time_from_url();
  $from = isset($period[0]) ? $period[0] : strtotime('-1 month');
  $to   = isset($period[1]) ? $period[1] : time();

  // Chart for ideas, comments, votes and users created per day.
  $header = $nodes = $comments = $votes = $users = array();
  $i = 0;
  $f = $from;
  while ($f <= $to) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', $f);
    // The number of nodes created each day.
    $nodes[$i] = db_query("SELECT COUNT(*) FROM {node} WHERE type = :type AND FROM_UNIXTIME(created,'%d.%m.%y') = :dete", array(':dete' => $header[$i], ':type' => 'idea'))->fetchField();
      
    $i ++;
    $f = strtotime("+1 day", $f);
  }
  // Building the rows, array of the data point arrays.
  $rows = array($nodes);
  // The labels for the rows.
  $columns = array('No. Of Ideas');
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartIdeas'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart1',
    'options' => array( // Optionals.
      'curveType' => "function",
      'forceIFrame' => FALSE,  
      'title' => 'Ideas per day',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
  // Chart for ideas, comments, votes and users created per day.
  $i = 0;
  $f = $from;
  while ($f <= $to) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', $f);
    // The number of comments created each day.
    $comments[$i]  = db_query("SELECT COUNT(*) FROM {comment} WHERE FROM_UNIXTIME(created,'%d.%m.%y') = :dete", array(':dete' => $header[$i]))->fetchField();
    // The number of voted placed each day.
  
    $i ++;
    $f = strtotime("+1 day", $f);
  }
  // Building the rows, array of the data point arrays.
  $rows = array($comments);
  // The labels for the rows.
  $columns = array('No. Of Comments',);
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartComments'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart2',
    'options' => array( // Optionals.
      'curveType' => "function",
      'colors' => array('green'),   
      'forceIFrame' => FALSE,  
      'title' => 'Comments per day',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
  // Chart for ideas, comments, votes and users created per day.
  $i = 0;
  $f = $from;
  while ($f <= $to) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', $f);
     // The number of voted placed each day.
    $votes[$i] = db_query("SELECT COUNT(*) FROM {votingapi_vote} WHERE  FROM_UNIXTIME(timestamp,'%d.%m.%y') = :dete", array(':dete' => $header[$i]))->fetchField();
    
    $i ++;
    $f = strtotime("+1 day", $f);
  }
  // Building the rows, array of the data point arrays.
  $rows = array($votes);
  // The labels for the rows.
  $columns = array('No. Of Votes');
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartVotes'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart3',
    'options' => array( // Optionals.
      'curveType' => "function",
      'colors' => array('purple'),   
      'forceIFrame' => FALSE,  
      'title' => 'Votes per day',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
  // Chart for ideas, comments, votes and users created per day.
  $i = 0;
  $f = $from;
  while ($f <= $to) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', $f);
    // The number of users join each day.
    $users[$i]     = db_query("SELECT COUNT(*) FROM {users} WHERE  FROM_UNIXTIME(created,'%d.%m.%y') = :dete", array(':dete' => $header[$i]))->fetchField();
    
    $i ++;
    $f = strtotime("+1 day", $f);
  }
  // Building the rows, array of the data point arrays.
  $rows = array($users);
  // The labels for the rows.
  $columns = array('No. Of Users');
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartUsers'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart4',
    'options' => array( // Optionals.
      'curveType' => "function",
      'colors' => array('orange'),   
      'forceIFrame' => FALSE,  
      'title' => 'Ideas Users per day',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
  // Chart for ideas per category
  $voc = taxonomy_vocabulary_machine_name_load('categories');
  $tree = taxonomy_get_tree($voc->vid);
  $header = array();
  foreach ($tree as $term) {
    $header[] = $term->name;
    $terms[] = ideal_count_term_ideas($term->tid);
    
  }
  $columns = array('Ideas in category');
  $rows = array($terms);
  
  $settings = array();
  $settings['chart']['chartCategory'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'PieChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart5',
    'options' => array( // Optionals.
      'curveType' => "function", 
      'is3D' => TRUE,  
      'forceIFrame' => FALSE, 
      'title' => 'Ideas per category',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
  // Chart for projects in progress
  $nodes = $header = array();
  $in_action = field_info_field('field_in_action');
  foreach ($in_action['settings']['allowed_values'] as $k => $value) {
    $header[] = $value;
    // The number of nodes created each day.
    $nodes[] = db_query("SELECT COUNT(*) FROM {field_data_field_in_action} WHERE field_in_action_value = :value", array(':value' => $k))->fetchField();
  }
  
  $columns = array('Projects in progress');
  $rows = array($nodes);
  
  $settings = array();
  $settings['chart']['chartProgress'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'PieChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart6',
    'options' => array( // Optionals.
      'curveType' => "function", 
      'is3D' => TRUE,  
      'forceIFrame' => FALSE, 
      'title' => 'Projects in Progress',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
   // Chart for ideas, comments, votes and users created per day.
  $header = $nodes = array();
  $fid = db_query("SELECT fid FROM {flags} WHERE name = :name", array(':name' => 'popular_idea'))->fetchField();
  $i = 0;
  $f = $from;
  while ($f <= $to) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', $f);
    // The number of nodes become popular each day.
    $nodes[$i]  = db_query("SELECT COUNT(*) FROM {flag_content} WHERE fid = :fid AND FROM_UNIXTIME(timestamp,'%d.%m.%y') = :dete", array(':fid' => $fid, ':dete' => $header[$i]))->fetchField();
    
    $i ++;
    $f = strtotime("+1 day", $f);
  }
  // Building the rows, array of the data point arrays.
  $rows = array($nodes);
  // The labels for the rows.
  $columns = array('No. Of Ideas become populars');
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartPopular'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart7',
    'options' => array( // Optionals.
      'curveType' => "function",
      'colors' => array('red'), 
      'forceIFrame' => FALSE,  
      'title' => 'Ideas become populars',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  $form = drupal_get_form('ideal_dashboard_form');
  $form = drupal_render($form);
  return $form . '<div id="chart1"></div><div id="chart2"></div><div id="chart3"></div><div id="chart4"></div><div id="chart5"></div><div id="chart6"></div><div id="chart7"></div>';
}

/**
 * Provide a simple form to change time periods
 */
function ideal_dashboard_form() {
  $form = array();
  
  // Load some javascript for datepicker
  drupal_add_library('system','ui.datepicker');
  drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickadate" ).datepicker({
      dateFormat: "dd.mm.yy",
      autoSize: true
    });});', 'inline'); 

  
  $period_options = array(
    '1_week' => t('1 week'),
    '2_weeks' => t('2 weeks'),
    '1_month' => t('1 month'),
    '3_months' => t('3 months'),
    '6_months' => t('6 months'),
    '1_year' => t('1 year'),
    '2_years' => t('2 years'),
  );

  $period = isset($_GET['period']) ? filter_xss($_GET['period']) : NULL;
  if (!$period || !array_key_exists($period, $period_options)) {
    $period = '1_month'; // Default value
  }

  $form['message'] = array(
    '#type' => 'item',
    '#value' => t('Select a timeframe and click Update to see what\'s happening on your site during that time, as well as a total for all the activity during that timeframe.'),
  );

  $form['option'] = array(
    '#type' => 'radios',
    '#options' => array(
      'period' => '',
      'custom' => '',
    ),
    '#default_value' => isset($_GET['option']) ? filter_xss($_GET['option']) : 'period',
  );

  $form['period'] = array(
    '#type' => 'select',
    '#options' => $period_options,
    '#default_value' => $period,
  );

  $form['custom_from'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#attributes' => array('class' => array('pickadate')),  
    '#default_value' => isset($_GET['from']) ? filter_xss($_GET['from']) : '',
  );

  $form['custom_to'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#attributes' => array('class' => array('pickadate')),  
    '#default_value' => isset($_GET['to']) ? filter_xss($_GET['to']) : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Validate handler for quant_time_form()
 */
function ideal_dashboard_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Make sure a time option is checked
  if (!$values['option']) {
    form_set_error('error', t('An option must be selected'));
  }

  // If custom option, make sure we have both dates
  if ($values['option'] == 'custom' &&
    !($values['custom_from'] && $values['custom_to'])) {
    form_set_error('option', t('You must specify both dates'));
  }
  else if ($values['option'] == 'custom') {
    // Convert the times
    $now = time();
    $from = strtotime($values['custom_from']);
    $to = strtotime($values['custom_to']);

    // Make sure from date exists
    if (!$from) {
      form_set_error('custom_from', t('The from date must be formatted correctly %format.', array('%format' => '(MM/DD/YY)')));
    }

    // Make sure to date exists
    if (!$to) {
      form_set_error('custom_to', t('The to date must be formatted correctly %format.', array('%format' => '(MM/DD/YY)')));
    }

    // Make sure from is less than to
    if ($from > $to) {
      form_set_error('custom_from', t('The from date must be before the to date'));
    }

    // Make sure to date is not past current time
    if ($to > $now) {
      form_set_error('custom_to', t('The to date must not be past today'));
    }
  }
}

/**
 * Submit handler for quant_time_form()
 */
function ideal_dashboard_form_submit(&$form, $form_state) {
  // Add a query to the URL which will be used to determine the query
  switch ($form_state['values']['option']) {
    case 'period':
      drupal_goto($_GET['q'], array('query' => array('option' => 'period', 'period' => $form_state['values']['period'])));
      break;

    case 'custom':
      drupal_goto($_GET['q'], array('query' => array('option' => 'custom', 'from' => $form_state['values']['custom_from'], 'to' => $form_state['values']['custom_to'])));
      break;
  }
}

/**
 * Retrieve the time period or interval from the URL
 *
 * @param $period
 *   If the set option is a period, the return value will
 *   be a single timestamp representing how far to go back
 *   in time from the current. If the set option is custom,
 *   the return value will be an array representing a given
 *   time interval. The first value will be the "from" timestamp
 *   and the second value will be the "to" timestamp. If neither
 *   can be determined, FALSE will be returned. If no queries are
 *   in the URL, a timestamp for a 1 month period will be returned.
 */
function _ideal_main_get_time_from_url() {
  // Get the option
  if (isset($_GET['option'])) {
    switch ($_GET['option']) {
      case 'custom':
        $from = strtotime($_GET['from']);
        $to = strtotime($_GET['to']);
        if (is_numeric($from) && is_numeric($to)) {
          // Move the 'to' date to 1 second before midnight
          return array($from, $to + 86399);
        }
        break;

      case 'period':
        $from = strtotime('-' . str_replace('_', ' ', filter_xss($_GET['period'])));
        if (is_numeric($from)) {
          return array($from, time());
        }
    }
  }
  else {
    return array(strtotime('-1 month'), time());
  }

  return FALSE;
}
