<?php
//$Id: nodequeue_handler_field_all_queues.inc,v 1.1 2008/09/06 15:42:05 ezrag Exp $
  /**
 * Field handler for all queues.
 *
 * @ingroup views_field_handlers
 */
class nodequeue_handler_field_all_queues extends views_handler_field_prerender_list {
  /**
   * Provide meaningful defaults
   */
  function options(&$options) {
    parent::options($options);
    $options['link_to_queue'] = TRUE;
    $options['limit'] = FALSE;
    $options['qids'] = array();
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_queue'] = array(
      '#title' => t('Link this field to queue arrange page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit to queues'),
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

  function pre_render($values) {
    $nids = array();
    foreach ($values as $result) {
      $nids[] = $result->{$this->field_alias};
    }

    if ($nids) {
      $queue = '';
      if (!empty($this->options['limit']) && !empty($this->options['qids'])) {
        $queue = " AND nn.qid IN (" . implode(', ', array_keys(array_filter($this->options['qids']))) . ")";
      }

      $result = db_query("SELECT nn.nid, nn.qid, nq.title FROM {nodequeue_nodes} nn INNER JOIN {nodequeue_queue} nq ON nq.qid = nn.qid WHERE nn.nid IN (" . implode(', ', $nids) . ")$queue ORDER BY nq.title");

      while ($queue = db_fetch_object($result)) {
        if (empty($this->options['link_to_queue'])) {
          $this->items[$queue->nid][$queue->qid] = check_plain($queue->title);
        }
        else {
          $this->items[$queue->nid][$queue->qid] = l($queue->title, "admin/content/nodequeue/$queue->qid");
        }
      }
    }
  }
}

