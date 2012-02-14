<?php
// $Id: image_attach_views_handler_field_attached_images.inc,v 1.4.2.2 2010/08/03 17:43:00 sun Exp $

/**
 * Field handler to display the attached images on a node.
 *
 * This is a fake field: it does not add to the view query but runs its own
 * to get data. This is to avoid getting multiple rows for one node when
 * there are multiple attached image nodes.
 *
 * Inherit from image_handler_field_image_node_image so we get image sizes.
 */
class image_attach_views_handler_field_attached_images extends image_handler_field_image_node_image {

  /**
   * Constructor to provide additional fields to add.
   *
   * Add the nid field for our query later on.
   */
  function construct() {
    parent::construct();

    $this->additional_fields['image_attach_nid'] = array(
      'table' => 'node',
      'field' => 'nid',
    );
  }

  /**
   * Define default values for options.
   *
   * We inherit the image size option.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['as_link'] = array('default' => 'none');

    return $options;
  }

  /**
   * Extend the field's basic options with more image specific options.
   *
   * TODO: hide the 'link to node' option?
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['as_link'] = array(
      '#type' => 'select',
      '#title' => t('Link'),
      '#options' => array('none' => t('No link'), 'node' => t('Node'), 'image' => t('Image node')),
      '#default_value' => $this->options['as_link'],
    );
  }

  /**
   * We don't add to the query (see the parent class).
   * Instead, run our own query on pre-render. This is the same approach as CCK
   * multiple fields.
   */
  function pre_render(&$values) {
    $nids = array();
    foreach ($values as $v) {
      if (isset($v->{$this->aliases['image_attach_nid']})) {
        // Make the nid safe.
        $nid = intval($v->{$this->aliases['image_attach_nid']});
        // Use the nid as the key to make the values unique.
        $nids[$nid] = $nid;
      }
    }

    if (count($nids)) {
      $nids_string = implode(',', $nids);
      $result = db_query("SELECT nid, iid FROM {image_attach} WHERE nid IN (" . $nids_string . ") ORDER BY weight");
      while ($data = db_fetch_object($result)) {
        // Store all the attached image nids (iid) keyed by attaching nid.
        $attached_images[$data->nid][] = $data->iid;
      }

      // Place the data into the $values array for each result.
      foreach ($values as $id => $v) {
        if (isset($attached_images[$v->{$this->aliases['image_attach_nid']}])) {
          $values[$id]->image_attach_iids = $attached_images[$v->{$this->aliases['image_attach_nid']}];
        }
      }
    }
  }

  /**
   * Render the field.
   */
  function render($values) {
    if (isset($values->image_attach_iids)) {
      // Images need to loaded as full nodes to check node_access() for each of them.
      foreach ($values->image_attach_iids as $iid) {
        $image_nodes[$iid] = node_load($iid);
      }

      $options = array(
        'size' => $this->options['image_derivative'],
        'link' => $this->options['as_link'],
      );

      $output = theme('image_attach_attached_images', $values->{$this->aliases['image_attach_nid']}, $image_nodes, $options);

      return $output;
    }
  }
}

