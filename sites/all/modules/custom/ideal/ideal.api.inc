<?php
//===IdeaL API Functions

function _get_vocabulary_by_machine_name($machine_name) {
  return db_result(db_query("SELECT vid FROM {vocabulary} WHERE module = ('%s')", $machine_name));
}

function _get_time_format($timestamp){
	$now = time();	
	$t_delta = $now - $timestamp;
  
	if( $t_delta < 86400){// actual day activity : 3600*24 last 24 hours
		if($t_delta  > 3600) {
			$output = t('Before ') . intval(date('G', $t_delta)) . t(' hours');
		}
		else {
		  $output = t('Before ') . intval(date('i', $t_delta)) . t(' minutes');
			}
	}
	else{
	  $output= date("d.m.y", $timestamp);
	}
	return $output;
}

function _ideal_head_to_head_chart($score1, $score2) {
  if ($score1 > $score2) {
    $h2 = round((100 * $score2)/$score1);   
    $h1 = 100;
  }
  else if ($score1 < $score2) {
    $h1 = round((100 * $score1)/$score2);   
    $h2 = 100;
  }
  else {
    $h2 = 100;
    $h1 = 100;    
  }
  return theme('head_to_head_chart', $score1, $score2, $h1, $h2);
}
