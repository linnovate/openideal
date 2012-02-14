<?php
// $Id: image_attach.views.inc,v 1.6.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Implementation of hook_views_data().
 */
function image_attach_views_data() {
  // Basic table information.
  $data = array();

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['image_attach']['table']['group'] = t('Image attach');

  // For other base tables, explain how we join
  // LEFT is the default, but let's be explicit
  $data['image_attach']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
      'type' => 'LEFT',
    ),
  );

  // Attached images relationship: gets us the attached nodes.
  // {node} --> {image_attach} --> {node}.
  // In other words, given the node, what attached images does it have?
  $data['image_attach']['iid'] = array(
    // The item it appears as on the UI,
    'title' => t('Attached images'),
    'help' => t('The images attached to a node.'),
    'relationship' => array(
      'title' => t('Attached image nodes'),
      'label' => t('Attached images'),
      'help' => t('The image nodes attached to the given node. May produce multiple rows. To filter out nodes with no attached images, set this relationship to be required.'),
      'base' => 'node',
      'base field' => 'nid',
      'relationship table' => 'image_attach',
      'relationship field' => 'iid',
      'handler' => 'views_handler_relationship',
    ),
  );

  // 'Reverse' relationship for attached images.
  // This goes from attached image nodes to image attach and thence to nodes,
  // but joining first on iid, and only then on nid.
  // In other words, given the image, what node(s) have it as an attached image?
  $data['image_attach_reverse']['table']['group'] = t('Image attach');

  $data['image_attach_reverse']['table']['join'] = array(
    'node' => array(
      'table' => 'image_attach',
      'left_field' => 'nid',
      'field' => 'iid',
      'type' => 'LEFT',
    ),
  );
  $data['image_attach_reverse']['nid'] = array(
    'title' => t('Attaching nodes'),
    'help' => t('The nodes that have attached images.'),
    'relationship' => array(
      'title' => t('Attaching nodes'),
      'label' => t('Attaching nodes'),
      'help' => t("The nodes that have attached images. This relationship should be used on a view of image nodes, to get to their attaching 'parent' nodes."),
      'base' => 'node',
      'base field' => 'nid',
      'relationship table' => 'image_attach_reverse',
      'relationship field' => 'nid',
      'handler' => 'views_handler_relationship',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function image_attach_views_data_alter(&$data) {
  // {node} table, prefixed with 'image_attach' to avoid potential clashes.
  // The images for attached image nodes.
  $data['node']['image_attach_images'] = array(
    'group' => t('Image attach'),
    'field' => array(
      'title' => t('Attached images'),
      'help' => t('The attached images, shown at a chosen size. This field can be added without a relationship.'),
      'handler' => 'image_attach_views_handler_field_attached_images',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function image_attach_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'image_attach'),
    ),
    'handlers' => array(
      'image_attach_views_handler_field_attached_images' => array(
        'parent' => 'image_handler_field_image_node_image',
      ),
    ),
  );
}

