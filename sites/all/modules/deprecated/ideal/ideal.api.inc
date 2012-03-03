<?php
//===IdeaL API Functions

function _get_vocabulary_by_machine_name($machine_name) {
  return db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE module = :module", array(':module' => ('%s'), '' => $machine_name))->fetchField();
}

function _get_time_format($timestamp) {
  $now = REQUEST_TIME;
  $t_delta = $now - $timestamp;

  if ( $t_delta < 86400) { // actual day activity : 3600*24 last 24 hours
    if ($t_delta  > 3600) {
      $output = t('Before ') . intval(date('G', $t_delta)) . t(' hours');
    }
    else {
      $output = t('Before ') . intval(date('i', $t_delta)) . t(' minutes');
    }
  }
  else {
    $output = date("d.m.y", $timestamp);
  }
  return $output;
}

function _ideal_head_to_head_chart($score1, $score2) {
  if ($score1 > $score2) {
    $h2 = round((100 * $score2) / $score1);
    $h1 = 100;
  }
  else if ($score1 < $score2) {
    $h1 = round((100 * $score1) / $score2);
    $h2 = 100;
  }
  else {
    $h2 = 100;
    $h1 = 100;
  }
  // TODO Please change this theme call to use an associative array for the $variables parameter.
  return theme('head_to_head_chart', $score1, $score2, $h1, $h2);
}
