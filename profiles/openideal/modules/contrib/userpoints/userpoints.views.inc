<?php


/**
 *  @file
 *  This defines views hooks for the Userpoints module. It will be loaded automatically as needed by the Views module.
 */

/**
 * Implements hook_views_data().
 */
function userpoints_views_data() {
  // ----------------------------------------------------------------
  // userpoints table
  // Describe the userpoints table.
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['userpoints']['table']['group'] = t('Userpoints');

  $data['userpoints']['table']['base'] = array(
      'field' => 'uid',
      'title' => t('Userpoints'),
      'help' => t('Points by category accumulated by users on your site.', userpoints_translation()),
  );

  $data['userpoints']['table']['join'] = array(
      'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
      ),
      'node' => array(
          'left_field' => 'uid',
          'field' => 'uid',
      ),
      'taxonomy_term_data' => array(
          'left_field' => 'tid',
          'field' => 'tid',
      ),
      // This goes to the node so that we have consistent authorship.
      'node_revisions' => array(
          'left_table' => 'node',
          'left_field' => 'uid',
          'field' => 'uid',
      ),
  );

  // Describe the points column of the userpoints table.
  $data['userpoints']['points'] = array(
      'title' => t('Current !points in category', userpoints_translation()),
      'help' => t("A User's current !points in a single category.", userpoints_translation()), // The help that appears on the UI,
      'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
          'name field' => 'points', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Describe the tid column of the userpoints table.
  $data['userpoints']['tid'] = array(
      'title' => t('Points category'),
      'help' => t('The categories (terms) of userpoints used'), // The help that appears on the UI,
      'field' => array(
          'handler' => 'userpoints_views_handler_field_category',
      ),
      'argument' => array(
          'handler' => 'userpoints_views_handler_argument_category',
          'numeric' => TRUE,
          'name field' => 'category', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'userpoints_views_handler_filter_category',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Describe the max_points column of the userpoints table.
  $data['userpoints']['max_points'] = array(
      'title' => t('Max !points in category', userpoints_translation()),
      'help' => t("A User's max !points in a single category.", userpoints_translation()), // The help that appears on the UI,
      'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
          'name field' => 'max_points', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Describe the last_update column of the userpoints table.
  $data['userpoints']['last_update'] = array(
      'title' => t('Last update in category'),
      'help' => t("The last update timestamp for a User's current !points in a single category.", userpoints_translation()),
      'field' => array(
          'handler' => 'views_handler_field_date',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
      ),
  );

  // Add relationship to user table.
  $data['userpoints']['uid'] = array(
      'title' => t('User'),
      'help' => t('Relate the userpoints table to the user table.'),
      'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'label' => t('Users'),
          'handler' => 'views_handler_relationship',
      ),
  );

  // ----------------------------------------------------------------
  // userpoints_total table
  // Describe the userpoints_total table.
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['userpoints_total']['table']['group'] = t('Userpoints total');

  $data['userpoints_total']['table']['base'] = array(
      'field' => 'uid',
      'title' => t('Userpoints Total'),
      'help' => t('Total !points over all categories accumulated by users on your site.', userpoints_translation()),
  );

  $data['userpoints_total']['table']['join'] = array(
      'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
      ),
      'node' => array(
          'left_field' => 'uid',
          'field' => 'uid',
      ),
      'taxonomy_term_data' => array(
          'left_field' => 'tid',
          'field' => 'tid',
      ),
      // This goes to the node so that we have consistent authorship.
      'node_revisions' => array(
          'left_table' => 'node',
          'left_field' => 'uid',
          'field' => 'uid',
      ),
  );

  // Describe the points column of the userpoints_total table.
  $data['userpoints_total']['points'] = array(
      'title' => t('Current total !points', userpoints_translation()),
      'help' => t("A User's current !points across all categories.", userpoints_translation()), // The help that appears on the UI,
      'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
          'name field' => 'points', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Describe the max_points column of the userpoints_total table.
  $data['userpoints_total']['max_points'] = array(
      'title' => t('Max total !points', userpoints_translation()),
      'help' => t("A user's max !points across all categories.", userpoints_translation()), // The help that appears on the UI,
      'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
          'name field' => 'max_points', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Describe the last_update column of the userpoints_total table.
  $data['userpoints_total']['last_update'] = array(
      'title' => t('Last update of total !points', userpoints_translation()),
      'help' => t("The last update timestamp for a user's !points across all categories.", userpoints_translation()),
      'field' => array(
          'handler' => 'views_handler_field_date',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
      ),
  );

  // Add relationship to user table.
  $data['userpoints_total']['uid'] = array(
      'title' => t('User'),
      'help' => t('Relate the userpoints total table to the user table.'),
      'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'label' => t('Users'),
          'handler' => 'views_handler_relationship',
      ),
  );

  // ----------------------------------------------------------------
  // userpoints_txn table
  // Describe the userpoints_txn table.
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['userpoints_txn']['table']['group'] = t('Userpoints Transactions');

  $data['userpoints_txn']['table']['base'] = array(
      'field' => 'txn_id',
      'title' => t('Userpoints Transactions'),
      'help' => t('Points transactions accumulated by users on your site.', userpoints_translation()),
  );

  $data['userpoints_txn']['table']['join'] = array(
      'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
      ),
      'taxonomy_term_data' => array(
          'left_field' => 'tid',
          'field' => 'tid',
      ),
      // This goes to the node so that we have consistent authorship.
      'node_revisions' => array(
          'left_table' => 'node',
          'left_field' => 'uid',
          'field' => 'uid',
      ),
  );

  // Describe the points column of the userpoints table.
  $data['userpoints_txn']['points'] = array(
      'title' => t('Points', userpoints_translation()),
      'help' => t("A User's !points for this transaction.", userpoints_translation()), // The help that appears on the UI,
      'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
          'name field' => 'points', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Describe the tid column of the userpoints table.
  $data['userpoints_txn']['tid'] = array(
      'title' => t('Category'),
      'help' => t('The categories (terms) of userpoints used for this transaction'), // The help that appears on the UI,
      'field' => array(
          'handler' => 'userpoints_views_handler_field_category',
      ),
      'argument' => array(
          'handler' => 'userpoints_views_handler_argument_category',
          'numeric' => TRUE,
          'name field' => 'category', // display this field in the summary
      ),
      'filter' => array(
          'handler' => 'userpoints_views_handler_filter_category',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Add relationship to user table.
  $data['userpoints_txn']['uid'] = array(
      'title' => t('User'),
      'help' => t('Relate the userpoints table to the user table.'),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
      ),
      'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'label' => t('Users'),
          'handler' => 'views_handler_relationship',
      ),
  );

  $data['userpoints_txn']['time_stamp'] = array(
      'title' => t('Timestamp'),
      'help' => t('The created timestamp for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field_date',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
      ),
  );

  $data['userpoints_txn']['changed'] = array(
      'title' => t('Changed'),
      'help' => t('The changed timestamp for the transaction, for when the transaction is updated.'),
      'field' => array(
          'handler' => 'views_handler_field_date',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
      ),
  );

  $data['userpoints_txn']['status'] = array(
      'title' => t('Status'),
      'help' => t('The status of the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field_numeric',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  $data['userpoints_txn']['description'] = array(
      'title' => t('Description'),
      'help' => t('The description for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field',
      ),
  );

  $data['userpoints_txn']['reference'] = array(
      'title' => t('Reference'),
      'help' => t('The reference for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  $data['userpoints_txn']['expirydate'] = array(
      'title' => t('Expiry date'),
      'help' => t('The expiration date for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field_date',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_date',
      ),
  );

  $data['userpoints_txn']['expired'] = array(
      'title' => t('Expired'),
      'help' => t('The expiry status for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field_numeric',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  $data['userpoints_txn']['entity_id'] = array(
      'title' => t('Entity ID'),
      'help' => t('The entity_id field. Used to relate to the node table.'),
      'field' => array(
          'handler' => 'views_handler_field',
      ),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
      ),
      'relationship' => array(
          'base' => 'node',
          'field' => 'nid',
          'label' => t('Entity'),
          'handler' => 'views_handler_relationship',
      ),
  );

  $data['userpoints_txn']['entity_type'] = array(
      'title' => t('Entity type'),
      'help' => t('The entity type for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  $data['userpoints_txn']['operation'] = array(
      'title' => t('Operation'),
      'help' => t('The operation for the transaction.'),
      'field' => array(
          'handler' => 'views_handler_field',
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
          'handler' => 'views_handler_sort',
      ),
  );

  // Reverse join information. This should not be necessary.
  $data['taxonomy_term_data']['table']['join'] = array(
    'userpoints' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
    'userpoints_txn' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
    'users' => array(
      'left_field' => 'tid',
      'left_table' => 'userpoints',
      'field' => 'tid',
    )
  );

  return $data;
}
