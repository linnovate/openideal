<?php

/**
 * @file
 * Implement a video field.
 */

/**
 * Implements of hook_field_info().
 */
function video_embed_field_field_info() {
  return array(
    'video_embed_field' => array(
      'label' => 'Video Embed',
      'description' => 'Embed videos from youtube or vimeo',
      'settings' => array(),
      'instance_settings' => array(
        'description_field' => 0,
      ),
      'default_widget' => 'video_embed_field_video',
      'default_formatter' => 'video_embed_field',
    )
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function video_embed_field_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  
  $form['description_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Description</em> field'),
    '#default_value' => isset($settings['description_field']) ? $settings['description_field'] : '',
    '#description' => t('The description field allows users to enter a description about the video.'),
    '#parents' => array('instance', 'settings', 'description_field'),
    '#weight' => 11,
  );
  
  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function video_embed_field_field_widget_info() {
  return array(
    'video_embed_field_video' => array(
      'label' => 'Video',
      'description' => 'Provides a video embed field',
      'field types' => array('video_embed_field'),
      'settings' => array(),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function video_embed_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Don't need to check the type right now because we're only defining one
  $element += array(
    '#type' => 'fieldset'
  );
  
  $element['video_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Video URL'),
    '#description' => $element['#description'],
    '#attributes' => array('class' => array('video_embed_url')),
    '#attached' => array( 
      'css' => array( 
        drupal_get_path('module', 'video_embed_field') .'/video_embed_field.form.css'
      )
    ),
    '#default_value' => isset($items[$delta]['video_url'])?$items[$delta]['video_url']:'',
    '#required' => $element['#required']
  );
  
  // Add the description field if enabled.
  if (!empty($instance['settings']['description_field'])) {
    $element['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => isset($items[$delta]['description']) ? $items[$delta]['description'] : '',
      '#description' => t('The description which may be used as a label.'),
    );
  }

  return $element;
}

/**
 * Validates video URL.
 */
function video_embed_field_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['video_url'])) {
      $item['video_url'] = trim($item['video_url']);
      if (stristr($item['video_url'], '.') && !stristr($item['video_url'], 'http://') && !stristr($item['video_url'], 'https://')) {
        $item['video_url'] = 'http://' . $item['video_url'];
      }
      
      $parts = parse_url($item['video_url']);
      if (!$parts || !isset($parts['host'])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => t('Invalid Url'), 
          'message' => t('Video: Invalid Video URL.', array('%name' => $instance['label'])),
        );
      }
      else {
        $host = $parts['host'];
        if (stripos($host, 'www.') > -1) {
          $host = substr($host, 4);
        }
        $domains = _video_embed_field_get_provider_domains();
        if (!array_key_exists($host, $domains)) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => t('Unsupported Video Provider'), 
            'message' => t('%name: This video provider is not currently supported.', array('%name' => $instance['label'])),
          );
        }
      }
    }
  }
}

/**
 *  Implementation of hook_field_prepare_view().
 *
 *  Download and save the thumbnail if it hasn't already been stored
 */
function video_embed_field_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach($items as $delta => $item){
  
    $info = video_embed_field_thumbnail_url($item['video_url']);

    if (isset($info['url']) && $info['url']) {
      $thumb_url = $info['url'];
      
      $local_path = 'public://video_embed_field_thumbnails/' . $info['handler'] . '/' . $info['id'] . '.jpg';
      if (!file_exists($local_path)) {
        $dirname = drupal_dirname($local_path);
        file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
        @copy($thumb_url, $local_path);
      }
      
      $items[$delta]['thumbnail_path'] = $local_path;
    } //couldn't get the thumbnail for whatever reason
    else {
      $items[$delta]['thumbnail_path'] = ''; 
    }
  
  }
}
/**
 * Implements of hook_field_is_empty().
 */
function video_embed_field_field_is_empty($item, $field){
  return empty($item) || empty($item['video_url']) || $item['video_url'] === '';
}

/**
 * Implements hook_field_formatter_info().
 */
function video_embed_field_field_formatter_info() {
  return array(
    'video_embed_field' => array(
      'label' => t('Video Player'),
      'field types' => array('video_embed_field'),
      'settings' => array('video_style' => 'normal', 'description' => 1),
    ),
    'video_embed_field_thumbnail' => array(
      'label' => t('Thumbnail Preview'),
      'field types' => array('video_embed_field'),
      'settings' => array('image_style' => 'none', 'description' => 1),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function video_embed_field_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();
  
  if ($display['type'] == 'video_embed_field') {
    $video_styles = video_embed_field_video_style_options(FALSE);
    $element['video_style'] = array(
      '#title' => t('Video style'),
      '#type' => 'select',
      '#default_value' => $settings['video_style'],
      '#options' => $video_styles,
    );
  } 
  else if ($display['type'] == 'video_embed_field_thumbnail') {
    $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#options' => image_style_options(FALSE),
      '#default_value' => $settings['image_style'],
      '#empty_option' => t('None (original image)'),
    ); 
  }
  
  if ($instance['settings']['description_field']) {
    $element['description'] = array(
      '#title' => t('Show description'),
      '#type' => 'select',
      '#default_value' => $settings['description'],
      '#options' => array(1 => t('Yes'), 0 => t('No')),
    );
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function video_embed_field_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();
  if($display['type'] == 'video_embed_field'){
    $video_styles = video_embed_field_video_style_options(FALSE);
    // Styles could be lost because of enabled/disabled modules that defines
    // their styles in code.
    if (isset($video_styles[$settings['video_style']])) {
      $summary[] = t('Video style: @style', array('@style' => $video_styles[$settings['video_style']]));
    }
  } 
  else if ($display['type'] == 'video_embed_field_thumbnail') {
    $image_styles = image_style_options(FALSE);
    if (isset($image_styles[$settings['image_style']])) {
      $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    } //No Image style (original image)  
    else {
      $summary[] = t('Original Image.');
    }
  }
  
  if ($settings['description'] && $instance['settings']['description_field']) {
    $summary[] = t('Show description');
  }
  
  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function video_embed_field_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display){
  $element = array();
  
  foreach ($items as $delta => $item) {
    //create the render array for the description
    if (!empty($item['description']) && $display['settings']['description'] && $instance['settings']['description_field']) {
      $description = array(
        '#prefix' => '<div class="video-embed-description">',
        '#markup' => $item['description'],
        '#suffix' => '</div>',
      );
    }
    else {
      $description = array();
    }
    //Render the field
    if ($display['type'] == 'video_embed_field') {
      $element[$delta] = array(
        array(
          '#theme' => 'video_embed_field_embed_code',
          '#url' => $item['video_url'],
          '#style' => $display['settings']['video_style'],
        ),
        $description,
      );
    }
    else if ($display['type'] == 'video_embed_field_thumbnail') {
      $image_style = $display['settings']['image_style'];
      if (isset($item['thumbnail_path']) && ($image_style == 'none' || $image_style == '')) {
        $element[$delta] = array(
          array(
            '#theme' => 'image',
            '#path' => $item['thumbnail_path'],
          ),
          $description,
        );  
      }
      else if (isset($item['thumbnail_path'])) {
        $element[$delta] = array(
          array(
            '#theme' => 'image_style',
            '#path' => $item['thumbnail_path'],
            '#style_name' => $image_style,
          ),
          $description,
        );
      } //incase no thumbnail was downloaded / provider doesn't support thumbnails
      else {
        $element[$delta] = array(
          $description,
        );    
      }
    }
  }

  return $element;
}