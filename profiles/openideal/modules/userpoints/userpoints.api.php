<?php

/**
 * @file
 * API documentation for userpoints.module
 */

/**
 * Return information about registered operations.
 *
 * Modules can register operation strings
 *
 * @return
 *   An array whose keys are operation strings used in
 *   userpoints_userpoints_api() and the which has the following properties:
 *   - description: A string that is used as a reason when transactions are
 *     displayed. Either this or a callback (see below) is required.
 *   - description callback: If the reason is dynamic, because he for example
 *     includes the title of a node, a callback function can be given which
 *     receives the transaction object and (if existing) and entity object as
 *     arguments.
 *   - admin description: A description which is searched for and displayed in
 *     the operation autocomplete field in the add points form.
 *
 */
function hook_userpoints_info() {
  return array(
    'expiry' => array(
      'description' => t('!Points have expired.', userpoints_translation()),
      'admin description' => t('Expire an existing transaction'),
    )
  );
}

/**
 * Allows to customize the output of a the users by points page.
 *
 * @param $output
 *   Render array with the content.
 *
 * @see userpoints_list_users().
 */
function hook_userpoints_list_alter(&$output) {

}

/**
 * Allows to customize the output of a the my userpoints page.
 *
 * @param $output
 *   Render array with the content.
 *
 * @see userpoints_list_transactions().
 */
function hook_userpoints_list_transactions_alter(&$output) {

}