<?php
// $Id: nodequeue.actions.inc,v 1.4 2009/01/06 15:23:01 ezrag Exp $

/**
 * @file nodequeue.actions.inc
 * Provides actions integration for nodequeues.
 */

/**
 * Implementation of hook_action_info().
 */
function nodequeue_action_info() {
  return array(
    'nodequeue_add_action' => array(
      'type' => 'node',
      'description' => t('Add to Nodequeues'),
      'configurable' => TRUE,
      'hooks' => array(
        'nodeapi' => array('insert', 'update'),
      ),
    ),
    'nodequeue_remove_action' => array(
      'type' => 'node',
      'description' => t('Remove from Nodequeues'),
      'configurable' => TRUE,
      'hooks' => array(
        'nodeapi' => array('insert', 'update'),
      ),
    ),
  );
}

/**
 * Configuration form for Add to Nodequeues action.
 */
function nodequeue_add_action_form($context) {
  // Default values for form.
  if (!isset($context['qids'])) {
    $context['qids'] = '';
  }

  $queues = nodequeue_load_queues(nodequeue_get_all_qids(500, TRUE), TRUE);
  foreach ($queues as $qid => $queue) {
    $options[$qid] = $queue->title;
  }
  $form = array();
  if (count($options)) {
    // Add form components.
    $form['qids'] = array(
      '#type' => 'select',
      '#title' => t("Queue"),
      '#default_value' => $context['qids'],
      '#multiple' => TRUE,
      '#options' => $options,
      '#required' => TRUE,
      '#description' => t('Specify the queues into which the node should be submitted. If the queue is a smartqueue, the node shall be placed into every subqueue for which it is eligible.')
    );
  }
  else {
    drupal_set_message(t('Please <a href="!url">create</a> a nodequeue first.', array('!url' => url('admin/content/nodequeue'))));
  }
  return $form;
}

/**
 * Submit handler for Add to Nodequeues action configuration.
 */
function nodequeue_add_action_submit($form, &$form_state) {
  return array(
    'qids' => $form_state['values']['qids'],
  );
}

/**
 * Action to add a node to a queue.
 */
function nodequeue_add_action($node, $context) {
  $queues = nodequeue_load_queues($context['qids'], TRUE);
  // Filter out queues by node type. We choose not to use nodequeue_get_qids() because it checks for access control which only matters if we administering a queue.
  $eligible_queues = array();
  foreach ($queues as $queue) {
    if (in_array($node->type, $queue->types)) {
      $eligible_queues[$queue->qid] = $queue;
    }
  }

  if (!empty($eligible_queues)) {
    // Remove the node from the eligible queues (if needed).
    nodequeue_remove_action($node, array('qids' => array_keys($eligible_queues)));

    // Use API to get the eligible subqueues
    $eligible_subqueues = nodequeue_get_subqueues_by_node($eligible_queues, $node);

    // Add node to each subqueue.
    foreach ($eligible_subqueues as $subqueue) {
      nodequeue_subqueue_add($queues[$subqueue->qid], $subqueue, $node->nid);
    }
  }
}

/**
 * Old-style action to add a node to a queue.
 */
function action_nodequeue_add($op, $edit = array(), $node) {
  switch($op) {
    case 'metadata':
      return array(
        'description' => t('Add to Nodequeues'),
        'type' => t('node'),
        'batchable' => true,
        'configurable' => true,
      );
      break;

    case 'do':
      $queues = nodequeue_load_queues($edit['qids'], TRUE);
      // Filter out queues by node type. We choose not to use nodequeue_get_qids() because it checks for access control which only matters if we administering a queue.
      $eligible_queues = array();
      foreach ($queues as $queue) {
        if (in_array($node->type, $queue->types)) {
          $eligible_queues[$queue->qid] = $queue;
        }
      }

      if (!empty($eligible_queues)) {
        // Remove the node from the eligible queues (if needed).
        action_nodequeue_remove('do', array('qids' => array_keys($eligible_queues)), $node);

        // Use API to get the eligible subqueues
        $eligible_subqueues = nodequeue_get_subqueues_by_node($eligible_queues, $node);

        // Add node to each subqueue.
        foreach ($eligible_subqueues as $subqueue) {
          nodequeue_subqueue_add($queues[$subqueue->qid], $subqueue, $node->nid);
        }
      }
      break;

    // return an HTML config form for the action
    case 'form':
      // default values for form
      if (!isset($edit['qids'])) $edit['qids'] = '';
      $queues = nodequeue_load_queues(nodequeue_get_all_qids(500, TRUE), TRUE);
      foreach ($queues as $qid => $queue) {
        $options[$qid] = $queue->title;
      }
      $form = array();
      if (count($options)) {
        // add form components
        $form['qids'] = array(
          '#type' => 'select',
          '#title' => t("Queue"),
          '#default_value' => $edit['qids'],
          '#multiple' => TRUE,
          '#options' => $options,
          '#required' => TRUE,
          '#description' => t('Specify the queues into which the node should be submitted. If the queue is a smartqueue, the node shall be placed into every subqueue for which it is eligible.')
          );
      }
      else {
        drupal_set_message(t('Please <a href="!url">create</a> a nodequeue first.', array('!url' => url('admin/content/nodequeue'))));
      }
      return $form;

     // validate the HTML form

    // process the HTML form to store configuration
    case 'submit':
      $params = array(
        'qids' => $edit['qids']
      );
      return $params;
      break;
  }
}

/**
 * Configuration form for Remove from Nodequeues action.
 */
function nodequeue_remove_action_form($context) {
  // Default values for form.
  if (!isset($context['qids'])) {
    $context['qids'] = array();
  }

  $queues = nodequeue_load_queues(nodequeue_get_all_qids(500, TRUE), TRUE);
  foreach ($queues as $qid => $queue) {
    $options[$qid] = $queue->title;
  }

  // Add form components.
  $form['qids'] = array(
    '#type' => 'select',
    '#title' => t("Queues"),
    '#default_value' => $context['qids'],
    '#multiple' => TRUE,
    '#decription' => t('Specify the queues from which the node should be removed. If the queue is a smartqueue, the node shall be removed from all subqueues.'),
    '#required' => TRUE,
    '#options' => $options,
  );
  return $form;
}

/**
 * Submit handler for Remove from Nodequeues action configuration.
 */
function nodequeue_remove_action_submit($form, &$form_state) {
  return array(
    'qids' => $form_state['values']['qids'],
  );
}

function nodequeue_remove_action($node, $context) {
  $qids = $context['qids'];
  // If a node is being deleted, ensure it's also removed from any queues.
  $placeholders = implode(',', array_fill(0, count($qids), '%d'));
  $args = $qids;
  $args[] = $node->nid;
  $result = db_query("SELECT * FROM {nodequeue_nodes} WHERE qid IN ($placeholders) AND nid = %d", $args);
  while ($obj = db_fetch_object($result)) {
    // This removes by nid, not position, because if we happen to have a
    // node in a queue twice, the 2nd position would be wrong.
    nodequeue_subqueue_remove_node($obj->sqid, $node->nid);
  }
}

/**
 * Old-style action to remove a node from a queue.
 */
function action_nodequeue_remove($op, $edit = array(), $node) {
  switch($op) {
    case 'metadata':
      return array(
        'description' => t('Remove from Nodequeues'),
        'type' => t('node'),
        'batchable' => true,
        'configurable' => true,
      );
      break;

    case 'do':
      $qids = $edit['qids'];
      // If a node is being deleted, ensure it's also removed from any queues.
      $placeholders = implode(',', array_fill(0, count($qids), '%d'));
      $args = $qids;
      $args[] = $node->nid;
      $result = db_query("SELECT * FROM {nodequeue_nodes} WHERE qid IN ($placeholders) AND nid = %d", $args);
      while ($obj = db_fetch_object($result)) {
        // This removes by nid, not position, because if we happen to have a
        // node in a queue twice, the 2nd position would be wrong.
        nodequeue_subqueue_remove_node($obj->sqid, $node->nid);
      }
      break;

    // return an HTML config form for the action
    case 'form':
      // default values for form
      if (!isset($edit['qids'])) $edit['qids'] = array();

      $queues = nodequeue_load_queues(nodequeue_get_all_qids(500, TRUE), TRUE);
      foreach ($queues as $qid => $queue) {
        $options[$qid] = $queue->title;
      }

      // add form components
      $form['qids'] = array(
        '#type' => 'select',
        '#title' => t("Queues"),
        '#default_value' => $edit['qids'],
        '#multiple' => TRUE,
        '#decription' => t('Specify the queues from which the node should be removed. If the queue is a smartqueue, the node shall be removed from all subqueues.'),
        '#required' => TRUE,
        '#options' => $options,
      );
      return $form;
      break;

     // validate the HTML form

    // process the HTML form to store configuration
    case 'submit':
      $params = array(
        'qids' => $edit['qids']
      );
      return $params;
      break;
  }
}
