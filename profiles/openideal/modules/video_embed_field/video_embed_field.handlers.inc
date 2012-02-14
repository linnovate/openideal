<?php
/**
 *  Provide some handlers for video embed field
 *  Other modules can implement the hook_video_embed_handler_info to provide more handlers
 */
 

/**
 *  Implementation of hook_video_embed_handler_info
 */
function video_embed_field_video_embed_handler_info() {
  $handlers = array();
  
  $handlers['youtube'] = array(
    'title' => 'Youtube',
    'function' => 'video_embed_field_handle_youtube',
    'thumbnail_function' => 'video_embed_field_handle_youtube_thumbnail',
    'form' => 'video_embed_field_handler_youtube_form',
    'domains' => array(
      'youtube.com',
      'youtu.be'
    ),
    'defaults' => array(
      'width' => 640,
      'height' => 360,
      'autoplay' => 0,
      'hd' => 1,
      'rel' => 0,
      'autohide' => 2,
      'showinfo' => 1,
      'theme' => 'dark',
    ),
  );
  
  $handlers['vimeo'] = array(
    'title' => 'Vimeo',
    'function' => 'video_embed_field_handle_vimeo',
    'thumbnail_function' => 'video_embed_field_handle_vimeo_thumbnail',
    'form' => 'video_embed_field_handler_vimeo_form',
    'domains' => array(
      'vimeo.com'
    ),
    'defaults' => array(
      'width' => 640,
      'height' => 360,
      'color' => '00adef',
      'autoplay' => 0,
    ),
  );
  
  return $handlers;
}

/**
 *  Helper function to get the youtube video's id
 *  Returns false if it doesn't parse for wahtever reason
 */
function _video_embed_field_get_youtube_id($url){
  // Find the ID of the video they want to play from the url
  if (stristr($url, 'http://')) {
    $url = substr($url, 7);
  }
  else if (stristr($url, 'https://')) {
    $url = substr($url, 8);
  }
  
  $pos = strripos($url, 'v=');
  if ($pos!== FALSE) { 
    $pos += 2; 
    $pos2 = stripos($url, '&', $pos);
  }
  else { 
    $pos = strripos($url, '/');
    if ($pos !== FALSE) { 
      $pos++; 
      $pos2 = stripos($url, '?', $pos);
    }
  }
  
  if ($pos === FALSE) {
    return FALSE;
  }
  else {
    if ($pos2 > 0) {
      $id = substr($url, $pos, $pos2 - $pos);
    }
    else {
      $id = substr($url, $pos);
    }
  }
  return $id;
}

/**
 * Handler for Youtube videos.
 */
function video_embed_field_handle_youtube($url, $settings) {
  $output = array();
  
  $id = _video_embed_field_get_youtube_id($url);
  if (!$id) {
    // We can't decode the URL - just return the URL as a link
    $output['#markup'] = l($url, $url);
    return $output;
  }
  // Construct the embed code
  $settings['wmode'] = 'opaque';
  $settings_str = _video_embed_code_get_settings_str($settings);
  
  $output['#markup'] = '<iframe style="width:' . $settings['width'] . 'px; height:' . $settings['height'] . 'px;" src="http://www.youtube.com/embed/' . $id . '?' . $settings_str . '" frameborder="0" allowfullscreen></iframe>';
  
  return $output;
}

/**
 *  Get the thumbnail url for youtube videos
 */
function video_embed_field_handle_youtube_thumbnail($video_url){
  $info = array();
  
  $id = _video_embed_field_get_youtube_id($video_url);
  if ($id) {
    $info['id'] = $id;
    $info['url'] = 'http://img.youtube.com/vi/'.$id.'/0.jpg';
  }
  return $info;
}

/**
 * Defines the form elements for the Youtube configuration form.
 */
function video_embed_field_handler_youtube_form($defaults) {
  $form = array();
  
  $form['width'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#title' => t('Player Width'),
    '#description' => t('The width of the youtube player, in pixels.  Only enter the number e.g. 560'),
    '#default_value' => $defaults['width'],
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#title' => t('Player Height'),
    '#description' => t('The height of the youtube player, in pixels.  Only enter the number e.g. 349'),
    '#default_value' => $defaults['height'],
  );
  $form['theme'] = array(
    '#type' => 'select',
    '#options' => array(
      'dark' => t('Dark'),
      'light' => t('Light'),
    ), 
    '#title' => t('Player theme'),
    '#default_value' => $defaults['theme'],
  );
  $form['autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#description' => t('Play the video immediately.'),
    '#default_value' => $defaults['autoplay'],
  );
  $form['hd'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use HD'),
    '#description' => t('Attempt to play the video in HD if available.'),
    '#default_value' => $defaults['hd'],
  );
  $form['rel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show related videos'),
    '#description' => t('Show related videos after the video is finished playing.'),
    '#default_value' => $defaults['rel'],
  );
  $form['showinfo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show info'),
    '#description' => t('Display information like the video title and rating before the video starts playing.'),
    '#default_value' => $defaults['showinfo'],
  );
  $form['autohide'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('The video progress bar and player controls will be visible throughout the video.'),
      1 => t('Automatically slide the video progress bar and the player controls out of view a couple of seconds after the video starts playing. They will only reappear if the user moves her mouse over the video player or presses a keyboard key.'),
      2 => t('The video progress bar will fade out but the player controls (play button, volume control, etc.) remain visible.'),
    ), 
    '#title' => t('Autohide progress bar and the player controls'),
    '#description' => t('Controls the autohide behavior of the youtube player controls.'),
    '#default_value' => $defaults['autohide'],
  );

  return $form;
}

/**
 * Handler for Vimeo videos.
 */
function video_embed_field_handle_vimeo($url, $settings) {
  $output = array();
  // Find the ID of the video they want to play from the url
  $pos = strripos($url, '/');
  if ($pos === FALSE) {
    // We can't decode the URL - just return the URL as a link
    $output['#markup'] = l($url, $url);
    return $output;
  }
  else {
    $pos+=1;
    $id = substr($url, $pos);
  }
  
  // Construct the embed code
  $settings['portrait'] = 0;
  $settings_str = _video_embed_code_get_settings_str($settings);
  
  $output['#markup'] = '<iframe style="width:' . $settings['width'] . 'px; height:' . $settings['height'] . 'px;" src="http://player.vimeo.com/video/' . $id . '?' . $settings_str . '" frameborder="0" allowfullscreen></iframe>';
  
  return $output;
}

/**
 *  Get the thumbnail url for youtube videos
 */
function video_embed_field_handle_vimeo_thumbnail($url){
  $pos = strripos($url, '/');
  if($pos === FALSE){
    $id = FALSE;
  } else {
    $pos+=1;
    $id = substr($url, $pos);
  }
  $info = array(
    'id' => $id,
  );
  
  $return = file_get_contents('http://vimeo.com/api/v2/video/'.$id.'.php');
  $return = unserialize($return);
  $video = current($return);
  
  $image_url = $video['thumbnail_large'];

  $info['url'] = $image_url;
  
  return $info;
}

/**
 * Defines the form elements for the Vimeo configuration form.
 */
function video_embed_field_handler_vimeo_form($defaults) {
  $form = array();
  
  $form['width'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#title' => t('Player Width'),
    '#description' => t('The width of the vimeo player, in pixels.  Only enter the number e.g. 560'),
    '#default_value' => $defaults['width'],
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#title' => t('Player Height'),
    '#description' => t('The height of the vimeo player, in pixels.  Only enter the number e.g. 315'),
    '#default_value' => $defaults['height'],
  );
  $form['color'] = array(
    '#type' => 'select',
    '#options' => array(
      '00adef' => t('Blue'),
      'ff9933' => t('Orange'),
      'c9ff23' => t('Lime'),
      'ff0179' => t('Fuschia'),
      'ffffff' => t('White') 
    ), 
    '#title' => t('Player Color'),
    '#description' => t('The color to use on the vimeo player.'),
    '#default_value' => $defaults['color'],
  );
  $form['autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#description' => t('Play the video immediately.'),
    '#default_value' => $defaults['autoplay'],
  );
  
  return $form;
}
