<?php
// $Id: image.views.inc,v 1.4.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * @file
 * Views integration for Image module.
 */

/**
 * Implementation of hook_views_data().
 */
function image_views_data() {
  $data = array();

  /**
   * {image} table, using {node} as base table.
   *
   * This is needed for the image_file alias to work. Possibly a bug in views?
   * @see http://drupal.org/node/425334
   */
  $data['image']['table']['group'] = t('Image');
  $data['image']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'files' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );

  /**
   * {image} table alias, using {files} as base table.
   */
  $data['image_file']['table']['group'] = t('Image');
  $data['image_file']['table']['join'] = array(
    'files' => array(
      'table' => 'image',
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );
  // The preset size of the image.
  $data['image_file']['image_size'] = array(
    'title' => t('Image preset size'),
    'help' => t('The preset image size of an image, e.g. <em>Original</em>, <em>Thumbnail</em>, etc.'),
    'field' => array(
      'handler' => 'image_handler_field_image_size',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'image_handler_argument_image_size',
      'parent' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'image_handler_filter_image_size',
    ),
  );
  // Relationship to the node base.
  // Allows us to go {files} --> {image} --> {node}.
  $data['image_file']['nid'] = array(
    'relationship' => array(
      'title' => t('Node'),
      'label' => t('Image node'),
      'help' => t('A relationship to gain access to the corresponding node of an image file uploaded via Image module.'),
      'base' => 'node',
      'field' => 'nid',
      'relationship table' => 'image',
      'relationship field' => 'nid',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
    ),
  );

  /**
   * {image} table alias, using {node} as base table.
   */
  $data['image_node']['table']['group'] = t('Image');
  $data['image_node']['table']['join'] = array(
    'node' => array(
      'table' => 'image',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Relationship to the files base.
  // Allows us to go {node} --> {image} --> {files}.
  $data['image_node']['nid'] = array(
    'relationship' => array(
      'title' => t('File'),
      'label' => t('Image file'),
      'help' => t('A relationship to gain access to the corresponding file(s) of an image node.'),
      'base' => 'files',
      'field' => 'fid',
      'relationship table' => 'image',
      'relationship field' => 'fid',
      'base field' => 'fid',
      'handler' => 'image_handler_relationship_node_image_file',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function image_views_data_alter(&$data) {
  // {node} table, prefixed with 'image' to avoid potential clashes.
  // The image for an image node. This could technically be #global, but adding
  // on {node} allows this field to be used through relationships if needed
  // (e.g. through a CCK nodereference field).
  $data['node']['image_image'] = array(
    'group' => t('Image'),
    'field' => array(
      'title' => t('Image'),
      'help' => t('The rendered image of an Image node, shown at a chosen size. This field can be added without a relationship.'),
      'handler' => 'image_handler_field_image_node_image',
    ),
    'argument' => array(
      'title' => t('Image size'),
      'help' => t('Allows the size of the Image node image field to be set with the argument.'),
      'handler' => 'image_handler_argument_image_node_image_size',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function image_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'image') . '/views',
    ),
    'handlers' => array(
      'image_handler_field_image_node_image' => array(
        'parent' => 'views_handler_field_node',
      ),
      'image_handler_field_image_size' => array(
        'parent' => 'views_handler_field',
      ),
      'image_handler_argument_image_size' => array(
        'parent' => 'views_handler_argument_string',
      ),
      'image_handler_argument_image_node_image_size' => array(
        'parent' => 'views_handler_argument_string',
      ),
      'image_handler_filter_image_size' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'image_handler_relationship_node_image_file' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function image_views_plugins() {
  return array(
    'style' => array(
      /**
       * A fluid grid view style for gallery items.
       */
      'image_gallery' => array(
        'title' => 'Gallery',
        'help' => t('Displays items in a fluid grid.'),
        'parent' => 'list',
        'handler' => 'views_plugin_style_list',
        'theme path' => drupal_get_path('module', 'image') . '/views/theme',
        'theme file' => 'theme.inc',
        'theme' => 'image_view_image_gallery',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-list',
      ),
    ),
    'argument validator' => array(
      'image_size' => array(
        'title' => t('Image size'),
        'handler' => 'image_plugin_argument_validate_image_size',
        'path' => drupal_get_path('module', 'image') . '/views',
      ),
    ),
    'argument default' => array(
      'image_size' => array(
        'title' => t('Image size'),
        'handler' => 'image_plugin_argument_default_image_size',
        'path' => drupal_get_path('module', 'image') . '/views',
        // Include parent class.
        'parent' => 'fixed',
      ),
    ),
  );
}

