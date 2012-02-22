<?php
//===rules actions

/**
 * Implements hook_rules_action_info().
().
 * @ingroup rules
 */
//function ideal_rules_action_info() {
//  return array(
//    'ideal_action_close_comments' => array(
//      'label' => t('close node from adding new comments'),
//      'module' => 'Ideal',
//      'arguments' => array(
//        'node' => array(
//          'type' => 'node',
//          'label' => t('Content'),
//        ),
//      ),
//    ),
//    'ideal_action_send_brand_email' => array(
//      'label' => t('Send Email to brand email'),
//      'module' => 'Ideal',
//      'arguments' => array(
//        'node' => array(
//          'type' => 'node',
//          'label' => t('Email'),
//        ),
//      ),
//      'eval input' => array('subject', 'message', 'from'),
//    ),
//  );
//}
//
///**
// * Action Implementation: Send Email to brand email.
// */
//function ideal_action_send_brand_email($node, $settings) {
//  foreach ($node->taxonomy as $term) {
//    if ($term->vid == 2) {
//      //working with taxonomy_node module
////      $nid = db_result(db_query("SELECT nid FROM {taxonomynode} WHERE tid = %d", $term->tid));
////      
////      $email_ref_db_info = content_database_info(content_fields('field_brand_email_value', 'category'));      
////      $email_ref_field = $ideas_ref_db_info['columns']['value']['column'];
////      $email_ref_table = $ideas_ref_db_info['table'];
////      
////      $to = db_result(db_query("SELECT $email_ref_field FROM {$email_ref_table} WHERE nid = %d", $nid));
////      $from = ($settings['from']) ? str_replace(array("\r", "\n"), '', $settings['from']) : NULL;
//
//      //working with term_fields module
//      $to = db_query("SELECT brand_email  FROM {term_fields_term} WHERE tid = :tid", array(':tid' => $term->tid))->fetchField();
//      $from = ($settings['from']) ? str_replace(array("\r", "\n"), '', $settings['from']) : NULL;
//
//
//      $message = drupal_mail('rules', 'rules_action_mail', $to, language_default(), $settings, $from);
//      if ($message['result']) {
//        watchdog('rules', 'Successfully sent email to %recipient', array('%recipient' => $to));
//      }
//    }
//  }
//
//}
//
///**
// * Action Implementation: close node from adding new comments.
// */
//function ideal_action_close_comments($node) {
//  $node->comment = 1;
//  return array('node' => $node);
//}
//
////===rules conditions
//
///**
// * Implements hook_rules_condition_info().
//().
// */
//function ideal_rules_condition_info() {
//  return array(
//    'ideal_condition_content_has_term' => array(
//      'label' => t('Content has term'),
//      'arguments' => array(
//        'node' => array(
//          'type' => 'node',
//          'label' => t('Content'),
//        ),
//      ),
//      'help' => t('Check whether the node has a specified term.'),
//      'module' => 'Ideal',
//    ), $info['ideal_rules_term_changed'] = array(
//      'label' => t('Term has changed'),
//      'arguments' => array(
//        'node' => array(
//          'type' => 'node',
//          'label' => t('Content containing changes'),
//        ),
//        'node_unchanged' => array(
//          'type' => 'node',
//          'label' => t('Content not containing changes'),
//        ),
//      ),
//      'help' => t('You should make sure that the used vocabulary exists in the given content type.'),
//      'module' => 'Ideal',
//    ),
//  );
//}
//
///**
// * Condition Implementation: Check if term exists on node.
// */
//function ideal_condition_content_has_term($node, $settings) {
//  if (!is_numeric($settings['term'])) {
//    $terms = taxonomy_get_term_by_name($settings['term']);
//    foreach ($terms as $tr) {
//      foreach ($node->taxonomy as $term) {
//        if (is_numeric($term)) {
//          $term = taxonomy_term_load($term);
//        }
//        if ($term->tid == $tr->tid) {
//          return TRUE;
//        }
//      }
//    }
//  }
//
//  foreach ($node->taxonomy as $term) {
//    if (is_numeric($term)) {
//      $term = taxonomy_term_load($term);
//    }
//    if ($term->tid == $settings['term']) {
//      return TRUE;
//    }
//  }
//  return FALSE;
//}
//
///**
// * Condition: Check if the field has changed.
// */
//function ideal_rules_term_changed($node1, $node2, $settings) {
//  // Get information about the term.
//  $term = $settings['taxonomy_vocabulary'];
//  //dpm(_term_has_value($node1->$settings['taxonomy_vocabulary'], $node2->$settings['taxonomy_vocabulary']));
//  return !empty($term) && !_term_has_value($node1->$settings['taxonomy_vocabulary'], $node2->$settings['taxonomy_vocabulary']);
//}
//
///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function ideal_rules_term_changed_form($settings, &$form, &$form_state) {
//  $settings += array('taxonomy_vocabulary' => '');
//  $form['settings']['taxonomy_vocabulary'] = array(
//    '#type' => 'select',
//    '#title' => t('taxonomy_vocabulary'),
//    '#options' => _get_vocas_names(),
//    '#default_value' => $settings['taxonomy_vocabulary'],
//    '#description' => t('Select the name of the vocabulary to look at.'),
//    '#required' => TRUE,
//  );
//}
//
//function _get_vocas_names() {
//  $vocas = taxonomy_get_vocabularies();
//  foreach ($vocas as $voca) {
//    $r[$voca->vid] = $voca->name;
//  }
//  return $r;
//}
//
//function _term_has_value($node_value, $value) {
//  if (count($value) != count($node_value)) {
//    return FALSE;
//  }
//  // Loop over multiple fields
//  foreach ($value as $delta => $sub_value) {
//    // Check if all properties of the value are there in the node value too
//    if (is_array($sub_value) && is_array($node_value[$delta])) {
//      if (count(array_diff_assoc($sub_value, $node_value[$delta])) != 0) {
//        return FALSE;
//      }
//    }
//    elseif ($sub_value !== $node_value[$delta]) {
//      return FALSE;
//    }
//  }
//  return TRUE;
//}
//
///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function ideal_rules_term_changed_label($settings, $argument_labels) {
//  return t("@node's field '@field' has been changed", array('@field' => $settings['field_name']) + $argument_labels);
//}
