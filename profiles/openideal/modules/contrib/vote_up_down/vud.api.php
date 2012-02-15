<?php
/**
 * @file
 * This file contains module hooks for users of Vote Up/down.
 */

/**
 * @name constant for a new widget message code.
 * The suggested name is build as: VUD_<entity>_WIDGET_MESSAGE_<description>.
 */
define('VUD_NEWENTITY_WIDGET_MESSAGE_POSTPONED', 2);

/**
 * Allow modules to alter access to the voting operation.
 *
 * @param $perm
 *   A string containing the permission required to modify the vote.
 * @param $entity_type
 *   A string containing the type of content being voted on.
 * @param $entity_id
 *   An integer containing the unique ID of the content being voted on.
 * @param $value
 *   An integer containing the vote value, 1 for an up vote, -1 for a down vote.
 * @param $tag
 *   A string containing the voting API tag.
 * $param $account
 *   An object containing the user voting on the content, NULL for the current
 *   user.
 *
 * @return
 *   A boolean forcing access to the vote, pass NULL if the function should
 *   not modify the access restriction.
 */
function hook_vud_access($perm, $entity_type, $entity_id, $value, $tag, $account) {
  // Denies access for all users other than user 1.
  if ($account->uid != 1) {
    return FALSE;
  }
}

/**
 * Modify the array of know messages.
 *
 * For a real implementation take a look at
 * vud_node_vud_widget_message_codes_alter() and its implementation of
 * hook_nodeapi() on vud_node module.
 *
 * @param $widget_message_codes
 *   The array of know messages passed by reference to modify it.
 */
function hook_vud_widget_message_codes(&$widget_message_codes) {
  // Add a new message code with its description, take a look to the
  // constant definition for more information
  // This is a dummy message to notify voting is posponed. It make sense
  // on a new vud_<entity> module since we only can include new messages
  // while we are doing a real vote work.
  $widget_message_codes[VUD_NEWENTITY_WIDGET_MESSAGE_POSTPONED] = t('The voting on this is postponed, please wait a while. we will be open the voting soon');
}

/**
 * Modify the vote just before it is casted.
 *
 * @param $votes
 *   A votes array that is going to be passed to votingapi_set_votes()
 *   function.
 */
function hook_vud_votes(&$votes) {
  // let's add a new vote at the same time with an own vote tag
  $new_vote = $votes[0];
  $new_vote['tag'] = 'our_custom_tag';
  $votes[] = $new_vote;
}
