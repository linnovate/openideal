<?php
// $Id: image_gallery.admin.inc,v 1.2.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * @file
 * Contains menu callbacks for image_gallery admin pages.
 */

/**
 * Menu callback for settings page.
 */
function image_gallery_admin_settings() {
  _image_check_settings();

  // Show various messages if Views module is enabled.
  if (module_exists('views')) {
    // Test the status of the view:
    // It's enabled by default, so either:
    // - not set: view enabled
    // - FALSE:   view enabled
    // - TRUE:    view disabled
    // @see views_get_all_views().
    $status = variable_get('views_defaults', array());
    if (isset($status['image_gallery']) && $status['image_gallery']) {
      // The view is disabled: tell the user that more interesting things can be done.
      $form['info']['#value'] = t('Enabling the <a href="!views-link">image_gallery view</a> will give you many more ways to customize your galleries.', array('!views-link' => url('admin/build/views')));
    }
    else {
      // The view is enabled: explain why there are no settings here and leave.
      $form['info'] = array(
        '#type' => 'item',
        '#title' => t('Image galleries are being made with the Views module'),
      );
      if (module_exists('views_ui')) {
        $form['info']['#value'] = t('To change the way galleries are displayed, edit the image_gallery view on the <a href="!views-link">Views administration page</a>.', array('!views-link' => url('admin/build/views')));
      }
      else {
        $form['info']['#value'] = t('To change the way galleries are displayed, enable the <strong>Views UI</strong> module on the <a href="!modules-link">Modules administration page</a>, then override the default image_gallery view.', array('!modules-link' => url('admin/build/modules')));
      }
      return $form;
    }
  }

  $form['gallery'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery settings'),
  );
  $form['gallery']['image_images_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Images per page'),
    '#default_value' => variable_get('image_images_per_page', 6),
    '#size' => 3,
    '#description' => t('Sets the number of images to be displayed in a gallery page.'),
  );
  $form['gallery']['image_gallery_node_info'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display node info'),
    '#default_value' => variable_get('image_gallery_node_info', 0),
    '#description' => t("Checking this will display the \"Posted by\" node information on the gallery pages."),
  );
  $form['gallery']['image_gallery_sort_order'] = array(
    '#type' => 'radios',
    '#title' => t('Image display sort order'),
    '#default_value' => variable_get('image_gallery_sort_order', IMAGE_GALLERY_SORT_CREATE_DESC),
    '#options' => array(
      IMAGE_GALLERY_SORT_CREATE_DESC => t('Create date, newest first'),
      IMAGE_GALLERY_SORT_CREATE_ASC  => t('Create date, oldest first'),
      IMAGE_GALLERY_SORT_FILENAME    => t('File name'),
      IMAGE_GALLERY_SORT_TITLE       => t('Image title'),
    ),
  );

  return system_settings_form($form);
}

/**
 * Menu callback for gallery admin.
 */
function image_gallery_admin() {
  _image_check_settings();

  $tree = taxonomy_get_tree(_image_gallery_get_vid());
  if ($tree) {
    $header = array(t('Name'), t('Operations'));
    foreach ($tree as $term) {
      $rows[] = array(str_repeat(' -- ', $term->depth) . ' ' . l($term->name, "image/tid/$term->tid"), l(t('edit gallery'), "admin/content/image/edit/$term->tid"));
    }
    return theme('table', $header, $rows);
  }
  else {
    return t('No galleries available');
  }
}

/**
 * Menu callback for editing or adding a gallery.
 */
function image_gallery_admin_edit($tid = NULL) {
  if ((isset($_POST['op']) && $_POST['op'] == t('Delete')) || isset($_POST['confirm'])) {
    return drupal_get_form('image_gallery_confirm_delete_form', $tid);
  }

  if (is_numeric($tid)) {
    $edit = (array) taxonomy_get_term($tid);
  }
  else {
    $edit = array(
      'name' => '',
      'description' => '',
      'vid' => _image_gallery_get_vid(),
      'tid' => NULL,
      'weight' => 0,
    );
  }
  return drupal_get_form('image_gallery_admin_form', $edit);
}

/**
 * Form for editing or adding a gallery.
 */
function image_gallery_admin_form(&$form_state, $edit = array()) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Gallery name'),
    '#default_value' => $edit['name'],
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('The name is used to identify the gallery.'),
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $edit['description'],
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('The description can be used to provide more information about the image gallery.'),
  );
  $form['parent']['#tree'] = TRUE;
  $form['parent'][0] = _image_gallery_parent_select($edit['tid'], t('Parent'), 'forum');
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'],
    '#delta' => 10,
    '#description' => t('When listing galleries, those with with light (small) weights get listed before containers with heavier (larger) weights. Galleries with equal weights are sorted alphabetically.'),
  );
  $form['vid'] = array(
    '#type' => 'hidden',
    '#value' => _image_gallery_get_vid(),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($edit['tid']) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
    $form['tid'] = array(
      '#type' => 'hidden',
      '#value' => $edit['tid'],
    );
  }
  $form['#submit'][] = 'image_gallery_admin_submit';
  $form['#theme'][] = 'image_gallery_admin';
  return $form;
}

function _image_gallery_parent_select($tid, $title) {
  $parents = taxonomy_get_parents($tid);
  if ($parents) {
    $parent = array_shift($parents);
    $parent = $parent->tid;
  }
  else {
    $parent = 0;
  }

  $children = taxonomy_get_tree(_image_gallery_get_vid(), $tid);

  // A term can't be the child of itself, nor of its children.
  foreach ($children as $child) {
    $exclude[] = $child->tid;
  }
  $exclude[] = $tid;

  $tree = taxonomy_get_tree(_image_gallery_get_vid());
  $options[0] = '<' . t('root') . '>';
  if ($tree) {
    foreach ($tree as $term) {
      if (!in_array($term->tid, $exclude)) {
        $options[$term->tid] = str_repeat(' -- ', $term->depth) . ' ' . $term->name;
      }
    }
  }

  return array(
    '#type' => 'select',
    '#title' => $title,
    '#default_value' => $parent,
    '#options' => $options,
    '#description' => t('Image galleries may be nested below other galleries.'),
    '#required' => TRUE,
  );
}

/**
 * Form submit handler.
 */
function image_gallery_admin_submit($form, &$form_state) {
  $status = taxonomy_save_term($form_state['values']);
  switch ($status) {
    case SAVED_NEW:
      drupal_set_message(t('Created new gallery %term.', array('%term' => $form_state['values']['name'])));
      break;

    case SAVED_UPDATED:
      drupal_set_message(t('The gallery %term has been updated.', array('%term' => $form_state['values']['name'])));
      break;

    case SAVED_DELETED:
      drupal_set_message(t('The gallery %term has been deleted.', array('%term' => $form_state['values']['name'])));
      break;
  }
  return 'admin/content/image';
}

/**
 * Form for confirmation for deleting a gallery.
 */
function image_gallery_confirm_delete_form(&$form_state, $tid) {
  $term = taxonomy_get_term($tid);

  $form['tid'] = array('#type' => 'value', '#value' => $tid);
  $form['name'] = array('#type' => 'value', '#value' => $term->name);

  return confirm_form($form, t('Are you sure you want to delete the image gallery %name?', array('%name' => $term->name)), 'admin/content/image', t('Deleting an image gallery will delete all sub-galleries. This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form submit handler.
 */
function image_gallery_confirm_delete_form_submit($form, &$form_state) {
  taxonomy_del_term($form_state['values']['tid']);
  drupal_set_message(t('The image gallery %term and all sub-galleries have been deleted.', array('%term' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/content/image';
}

