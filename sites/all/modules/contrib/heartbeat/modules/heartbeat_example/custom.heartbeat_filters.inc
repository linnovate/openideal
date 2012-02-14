<?php
// $Id: custom.heartbeat_filters.inc,v 1.1.2.3 2010/08/19 17:00:02 stalski Exp $

/**
 * @file
 *   custom.heartbeat_filters.inc
 */

/*
 * Created on 10-okt-09
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */


/**
 * Heartbeat filter implementation for custom filters.
 */
function heartbeat_filter_stories(&$messages, HeartbeatAccess $stream) {
  $filtered_messages = array();

  foreach ($messages as $key => $message) {
  	if ($message->nid > 0) {
  		$node = node_load($message->nid);
  		if ($node->type == 'story') {
  			$filtered_messages[$key] = $message;
  		}
  	}
  }

  return $filtered_messages;
}