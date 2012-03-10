<?php
function gct_analytics_charts() {
//function gct_analytics_gct() {
  $period = _gct_main_get_time_from_url();
  $from = isset($period[0]) ? $period[0] : strtotime('-1 month');
  $to   = isset($period[1]) ? $period[1] : time();

  //==================================
  // Chart for ideas created per day.
  //==================================
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
  $ret[] = draw_chart($settings);
  
  //=====================================
  // Chart for comments created per day.
  //=====================================
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
  $ret[] = draw_chart($settings);
  
  //=====================================
  // Chart for votes placed per day.
  //=====================================
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
  $ret[] = draw_chart($settings);  
  
  //=====================================
  // Chart for user joind per day.
  //=====================================
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
      'title' => 'Users per day',
      'width' => 500,
      'height' => 300  
    )   
  );
  
  //Draw it.
  $ret[] = draw_chart($settings); 
  
  //=====================================
  // Chart for ideas per category.
  //=====================================
  $voc = taxonomy_vocabulary_machine_name_load('categories');
  $tree = taxonomy_get_tree($voc->vid);
  $header = array();
  foreach ($tree as $term) {
    $header[] = $term->name;
    $query = db_select('taxonomy_index', 'ti');
    $query->condition('ti.tid', $term->tid, '=')
          ->fields('ti', array('nid'));
    $terms[] = $query->countQuery()->execute()->fetchField();
    
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
  $ret[] = draw_chart($settings);  
  
  //=====================================
  // Chart for projects in progress.
  //=====================================
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
  $ret[] = draw_chart($settings);  
  
  //=========================================
  // Chart for ideas become popular per day.
  //=========================================
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
  $ret[] = draw_chart($settings);
  
  return $ret;
}