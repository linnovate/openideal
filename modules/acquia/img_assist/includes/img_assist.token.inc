<?php
// $Id: img_assist.token.inc,v 1.1.2.1 2008/09/12 21:10:55 sun Exp $

/**
 * Implementation of hook_token_values().
 */
function img_assist_token_values($type, $object = NULL, $options = array()) {
  $values = array();
  switch ($type) {
    case 'node':
      $node = $object;
      $values['teaser'] = img_assist_sanitize($node->teaser);
      $values['body']   = img_assist_sanitize($node->body);
      break;
  }
  return $values;
}

/**
 * Implementation of hook_token_list().
 */
function img_assist_token_list($type = 'all') {
  if ($type == 'node' || $type == 'all') {
    $tokens['node']['teaser'] = t('Node teaser text (plain text)');
    $tokens['node']['body']   = t('Node body text (plain text)');
    return $tokens;
  }
}

