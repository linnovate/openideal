<?php
//$Id: nodequeue_handler_relationship_nodequeue.inc,v 1.2 2010/01/31 23:17:53 ezrag Exp $
/**
 * Specialized relationship handler to add nodequeues.
 */
class nodequeue_handler_relationship_nodequeue extends views_handler_relationship {
  function option_definition() {
    $options = parent::option_definition();

    $options['limit']['default'] = FALSE;
    $options['qids']['default'] = array();
    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $queues = nodequeue_load_queues(nodequeue_get_all_qids(NULL));

    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit to one or more queues (recommended)'),
      '#default_value'=> $this->options['limit'],
    );

    $options = array();
    $queues = nodequeue_load_queues(nodequeue_get_all_qids(NULL));
    foreach ($queues as $queue) {
      $options[$queue->qid] = $queue->title;
    }

    $form['qids'] = array(
      '#prefix' => '<div><div id="edit-options-qids">',
      '#suffix' => '</div></div>',
      '#type' => 'checkboxes',
      '#title' => t('Queues'),
      '#options' => $options,
      '#default_value' => $this->options['qids'],
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array('edit-options-limit' => array(TRUE)),
    );
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $join = new views_join();
    $join->definition = array(
      'table' => 'nodequeue_nodes',
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
    );

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    if (!empty($this->options['limit'])) {
      $join->definition['extra'] = array(array(
        'field' => 'qid',
        'value' => array_filter($this->options['qids']),
        'numeric' => TRUE,
      ));
    }

    $join->construct();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];

    $this->alias = $this->query->add_relationship($alias, $join, 'nodequeue_nodes', $this->relationship);
  }
}