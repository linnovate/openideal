<?php 
/**
 *  Creating graphs for the last 10 days of node creating, comments and votes.
 */
function ideal_dashboard() { 
  // Chart for ideas, comments, votes and users created per day.
  $header = $nodes = $comments = $votes = array();
  
  for ($i = 29; $i >= 0; $i--) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', strtotime("-{$i} day"));
    // The number of nodes created each day.
    $nodes[$i]     = db_query("SELECT COUNT(*) FROM {node} WHERE type = :type AND FROM_UNIXTIME(created,'%d.%m.%y') = :dete", array(':dete' => $header[$i], ':type' => 'idea'))->fetchField();
    // The number of comments created each day.
    $comments[$i]  = db_query("SELECT COUNT(*) FROM {comment} WHERE FROM_UNIXTIME(created,'%d.%m.%y') = :dete", array(':dete' => $header[$i]))->fetchField();
    // The number of voted placed each day.
    $votes[$i]     = db_query("SELECT COUNT(*) FROM {votingapi_vote} WHERE  FROM_UNIXTIME(timestamp,'%d.%m.%y') = :dete", array(':dete' => $header[$i]))->fetchField();
    // The number of users join each day.
    $users[$i]     = db_query("SELECT COUNT(*) FROM {users} WHERE  FROM_UNIXTIME(created,'%d.%m.%y') = :dete", array(':dete' => $header[$i]))->fetchField();
  }
  // Building the rows, array of the data point arrays.
  $rows = array($nodes, $comments, $votes, $users);
  // The labels for the rows.
  $columns = array('No. Of Ideas', 'No. Of Comments', 'No. Of Votes', 'No. Of Users');
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartOne'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart1',
    'options' => array( // Optionals.
      'curveType' => "function",
      'forceIFrame' => FALSE,  
      'title' => 'Ideas / Comments / Votes / Users / per day',
      'width' => 800,
      'height' => 400  
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
  $settings['chart']['chartTwo'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'PieChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart2',
    'options' => array( // Optionals.
      'curveType' => "function", 
      'is3D' => TRUE,  
      'forceIFrame' => FALSE, 
      'title' => 'Ideas per category',
      'width' => 800,
      'height' => 400  
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
  $settings['chart']['chartThree'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'PieChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart3',
    'options' => array( // Optionals.
      'curveType' => "function", 
      'is3D' => TRUE,  
      'forceIFrame' => FALSE, 
      'title' => 'projects in Progress',
      'width' => 800,
      'height' => 400  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 
  
   // Chart for ideas, comments, votes and users created per day.
  $header = $nodes = array();
  $fid = db_query("SELECT fid FROM {flags} WHERE name = :name", array(':name' => 'popular_idea'))->fetchField();
  for ($i = 29; $i >= 0; $i--) {
    // Building the header - list of date from today backward.
    $header[$i]   = date('d.m.y', strtotime("-{$i} day"));
    // The number of nodes become popular each day.
    $nodes[$i]  = db_query("SELECT COUNT(*) FROM {flag_content} WHERE fid = :fid AND FROM_UNIXTIME(timestamp,'%d.%m.%y') = :dete", array(':fid' => $fid, ':dete' => $header[$i]))->fetchField();
  }
  // Building the rows, array of the data point arrays.
  $rows = array($nodes);
  // The labels for the rows.
  $columns = array('No. Of Ideas become populars');
  
  // Put all the data into the settings array, 
  // which will be send to draw.
 
  // Must empty the array first.
  $settings = array();
  $settings['chart']['chartFour'] = array(  
    'header' => $header,
    'rows' => $rows,
    'columns' => $columns,
    'chartType' => 'LineChart',// LineChart, PieChart, ,ColumnChart, ,AreaChart, Gauge, BarChart, etc....
    'containerId' =>  'chart4',
    'options' => array( // Optionals.
//      'curveType' => "function",
      'forceIFrame' => FALSE,  
      'title' => 'Ideas become populars',
      'width' => 800,
      'height' => 400  
    )   
  );
  
  //Draw it.
  ideal_chart_draw_chart($settings); 

  return '<div id="chart1"></div><div id="chart2"></div><div id="chart3"></div><div id="chart4"></div>';
}