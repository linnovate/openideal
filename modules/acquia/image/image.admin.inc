<?php
// $Id: image.admin.inc,v 1.9.2.2 2010/08/03 17:43:00 sun Exp $

/**
 * Menu callback; Form builder function for image settings.
 */
function image_admin_settings() {
  $form['image_updated'] = array(
    '#type' => 'hidden',
    '#value' => variable_get('image_updated', time()),
  );

  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image file settings'),
  );
  $form['files']['image_default_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image path'),
    '#default_value' => variable_get('image_default_path', 'images'),
    '#description' => t('Subdirectory in the directory %dir where pictures will be stored. Do not include trailing slash.', array('%dir' => file_directory_path())),
  );

  $form['files']['image_max_upload_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size'),
    '#default_value' => variable_get('image_max_upload_size', 800),
    '#field_suffix' => t('KB'),
    '#size' => 12,
    '#description' => t('Maximum file size for image uploads. When a maximum image dimensions is specified for original images the size is checked after resizing.'),
  );

  $form['image_sizes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image sizes'),
    '#tree' => TRUE,
    '#theme' => 'image_settings_sizes_form',
    '#description' => '<p>'. t("The <em>Scale image</em> operation resizes images so that they fit with in the given dimensions. If only one dimension is specified the other dimension will be computed based on the image's aspect ratio. The <em>Scale and crop image</em> operation resizes images to be exactly the given dimensions. If only one dimension is specified the image will not be cropped, making this is equivalent to <em>Scale image</em>.") .'</p>'
      .'<p>'. t("Note: 'Original' dimensions will only be used to resize images when they are first uploaded. Existing originals will not be modified.") .'</p>',
  );

  $link_options = array(
    IMAGE_LINK_HIDDEN => t('Hidden'),
    IMAGE_LINK_SHOWN => t('Same window'),
    IMAGE_LINK_NEW => t('New window'),
  );

  $sizes = image_get_sizes();

  // Add some empty rows for user defined sizes.
  $num_sizes = count($sizes);
  for ($i = $num_sizes; $i < ($num_sizes + 3); $i++) {
    $sizes['new' . $i] = array(
      'label' => '',
      'operation' => 'scale',
      'width' => '',
      'height' => '',
      'link' => IMAGE_LINK_SHOWN,
      'new' => TRUE,
    );
  }

  foreach ($sizes as $key => $size) {
    $form['image_sizes'][$key]['label'] = array(
      '#type' => 'textfield',
      '#default_value' => $size['label'],
      '#size' => 25,
      '#maxlength' => 32,
    );

    // For required sizes, set the value and disable the field.
    if (_image_is_required_size($key)) {
      $form['image_sizes'][$key]['label']['#disabled'] = TRUE;
      $form['image_sizes'][$key]['label']['#value'] = $size['label'];
      $form['image_sizes'][$key]['label']['#required'] = TRUE;
    }
    $form['image_sizes'][$key]['operation'] = array(
      '#type' => 'select',
      '#default_value' => $size['operation'],
      '#options' => array('scale' => t('Scale image'), 'scale_crop' => t('Scale and crop image')),
    );
    $form['image_sizes'][$key]['width'] = array(
      '#type' => 'textfield',
      '#default_value' => $size['width'],
      '#size' => 5,
      '#maxlength' => 5,
    );
    $form['image_sizes'][$key]['height'] = array(
      '#type' => 'textfield',
      '#default_value' => $size['height'],
      '#size' => 5,
      '#maxlength' => 5,
    );
    $form['image_sizes'][$key]['link'] = array(
      '#type' => 'select',
      '#default_value' => $size['link'],
      '#options' => $link_options,
    );
  }

  // Make changes to the settings before passing them off to
  // system_settings_form_submit().
  $form['#submit'][] = 'image_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Form validation handler for image admin settings form.
 */
function image_admin_settings_validate($form, &$form_state) {
  // Check that the sizes provided have the required amount of information.
  $image_sizes = $form_state['values']['image_sizes'];
  foreach (element_children($image_sizes) as $key) {
    // If there's a label they must provide at either a height or width.
    if ($key != IMAGE_ORIGINAL && !empty($image_sizes[$key]['label'])) {
      if (empty($image_sizes[$key]['width']) && empty($image_sizes[$key]['height'])) {
        form_set_error("image_sizes][$key][width", t('You must specify width, height or both dimensions.'));
      }
    }
  }

  // Try to create directories and warn the user of errors.
  $image_default_path = $form_state['values']['image_default_path'];
  $image_path = file_create_path(file_directory_path() . '/' . $image_default_path);
  $temp_path = $image_path . '/temp';

  if (!file_check_directory($image_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS, 'image_default_path')) {
    form_set_error('image_default_path', t('You have specified an invalid directory.'));
  }
  if (!file_check_directory($temp_path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS, 'image_default_path')) {
    form_set_error('image_default_path', t('You have specified an invalid directory.'));
  }
}

/**
 * Form submit handler for image admin settings form.
 */
function image_admin_settings_submit($form, &$form_state) {
  // Ensure that 'image_default_path' variable contains no trailing slash.
  $form_state['values']['image_default_path'] = rtrim($form_state['values']['image_default_path'], '/');

  // Remove deleted sizes, and use the label as indexes for new sizes.
  $old_sizes = image_get_sizes();
  // If the size's operation, or dimensions change we need to rebuild.
  $rebuild = FALSE;

  foreach ($form_state['values']['image_sizes'] as $key => $size) {
    // Changed to the original setting only affect new images and they
    // shouldn't be able to add or remove it.
    if ($key == IMAGE_ORIGINAL) {
      continue;
    }

    // Remove sizes without labels.
    if (empty($size['label'])) {
      unset($form_state['values']['image_sizes'][$key]);
    }

    // Check if only one is set, indicating an addition or removal.
    if (isset($form_state['values']['image_sizes'][$key]) ^ isset($old_sizes[$key])) {
      $rebuild |= TRUE;

      // When adding a new size, we need to assign a key.
      if (isset($form_state['values']['image_sizes'][$key])) {
        unset($form_state['values']['image_sizes'][$key]);
        $new_key = drupal_strtolower(drupal_substr($size['label'], 0, 32));
        $form_state['values']['image_sizes'][$new_key] = $size;
      }
    }
    // Check for changes.
    else if (isset($form_state['values']['image_sizes'][$key]) && isset($old_sizes[$key])) {
      // Did the operation, height or width change?
      foreach (array('operation', 'height', 'width') as $field) {
        $rebuild |= ($form_state['values']['image_sizes'][$key][$field] != $old_sizes[$key][$field]);
      }
    }
  }

  // If we've changed anything update the image_update variable so the
  // derivative images are rebuilt.
  if ($rebuild) {
    drupal_set_message(t('Changes to the images sizes mean that the derivative images will need to be regenerated.'));
    $form_state['values']['image_updated'] = time();
  }
}

function theme_image_settings_sizes_form($form) {
  $header = array(t('Label'), t('Operation'), t('Width'), t('Height'), t('Link'));
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = drupal_render($form[$key]['label']);
    $row[] = drupal_render($form[$key]['operation']);
    $row[] = drupal_render($form[$key]['width']);
    $row[] = drupal_render($form[$key]['height']);
    $row[] = drupal_render($form[$key]['link']);
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}

