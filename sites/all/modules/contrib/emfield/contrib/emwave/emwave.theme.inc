<?php
// $Id: emwave.theme.inc,v 1.1.2.1 2009/10/13 13:26:44 aaron Exp $

function theme_emwave_wave($field, $item, $formatter, $node, $code, $width, $height, $bg_color, $color, $font, $font_size, $title = '', $link = NULL) {
  $url = module_invoke('emfield', 'include_invoke', 'emwave', $item['provider'], 'wave_url', $code, $width, $height, $formatter, $field, $item, $node);
  $attributes = array();
  if ($width) {
    $attributes['width'] = $width;
  }
  if ($height) {
    $attributes['height'] = $height;
  }
  if (!$width || !$height) {
    // Find out the size of the actual image file, and scale accordingly
    if ( $item['data']['width'] && $item['data']['height'] ) {
      // The waves's width & height are known
      $scale_width = $item['data']['width'] / ($width ? $width : 1);
      $scale_height = $item['data']['height'] / ($height ? $height : 1);
      if ($scale_width > $scale_height) {
        $attributes['width'] = $width;
      }
  else {
        $attributes['height'] = $height;
      }
    }
    else {
      // We don't know the size of the image, so just make it fill the space available.
      // It will probably be stretched in one direction, making it look odd.
      $attributes['width'] = $width ? $width : NULL;
      $attributes['height'] = $height ? $height : NULL;
    }
	}
	// Setup an array of JavaScript settings.
  $js_settings = array(
    'wave_id' => $code,
    'bg_color' => $bg_color,
    'color' => $color,
    'font' => $font,
    'font_size' => $font_size . 'px',
  );
  static $added = FALSE;
  static $i = 0;
  if ($i >= 1) {
    $i++;
  }
  if ($added == FALSE ) {
    drupal_set_html_head('    <script src="http://wave-api.appspot.com/public/embed.js" type="text/javascript"></script>');
    $added = TRUE;
    $i++;
  }
  // Add the JavaScript settings.
  drupal_add_js(array('emwave-' . $code => $js_settings), "setting");
  $output .= <<<EOD
    <script type="text/javascript">
      Drupal.behaviors.emWave$i = function (context) {
        var wavePanel$i = new WavePanel('http://wave.google.com/a/wavesandbox.com/');
        wavePanel$i.loadWave('wavesandbox.com!w+$code');
        wavePanel$i.setUIConfig('$bg_color','$color','$font','$font_size');
        wavePanel$i.init(document.getElementById('waveframe$i'));
      }
    </script>
EOD;
  $output .= "\n" . '<div id="waveframe' . $i . '" style="width:';
  $output .= $attributes['width'] . 'px; height:' . $attributes['height'] . 'px;" ></div>';
  // Apparently this stuff is not implemented yet.
  //$output .= "\n" . '  <div id="add-reply-' . $i . '">';
  //$output .= "\n" . '    <button type="button" name="add-reply-';
  //$output .= $i . '" value="' . t('Reply to this wave') . '" onclick="wavePanel' . $i . '.addReply()" >';
  //$output .= 'Add Reply</button></div>';
  //$output .= "\n" . '  <div id="add-participant-' . $i . '">';
  //$output .= "\n" . '    <button type="button" name="add-participant-';
  //$output .= $i . '" value="' . t('Join this wave') . '" onclick="wavePanel' . $i . '.addParticipant()" >';
  //$output .= 'Add Participant</button></div>';
  
  // If being output as a link, theme a link version.
  if ($link) {
    $output = l($output, $link, array('html' => true));
  }
  return $output;
}

function theme_emwave_wave_full($field, $item, $formatter, $node) {
  if ($item['value'] && $item['provider']) {
    $code = $item['value'];
    $width = $field['widget']['full_width'];
    $height = $field['widget']['full_height'];
    $bg_color = $field['widget']['bg_color'];
    $color = $field['widget']['color'];
    $font = $field['widget']['font'];
    $font_size = $field['widget']['font_size'];
    $link = $field['widget']['full_link'];
    if ($link == emwave_LINK_CONTENT) {
      $link = 'node/'. $node->nid;
    }
    else if ($link == emwave_LINK_PROVIDER) {
      $link = module_invoke('emfield', 'include_invoke', 'emwave', $item['provider'], 'embedded_link', $code, $item['data']);
    }
    else {
      $link = NULL;
    }
    $title = module_invoke('emfield', 'include_invoke', 'emwave', $item['provider'], 'wave_title', $code, $item['data']);
    $output = theme('emwave_wave', $field, $item, $formatter, $node, $code, $width, $height, $bg_color, $color, $font, $font_size, $title, $link);
  }
  return $output;
}

function theme_emwave_default($field, $item, $formatter, $node) {
  return theme('emwave_wave_full', $field, $item, $formatter, $node);
}

function theme_emwave_wave_preview($field, $item, $formatter, $node) {
  if ($item['value'] && $item['provider']) {
    $code = $item['value'];
    $width = $field['widget']['preview_width'];
    $height = $field['widget']['preview_height'];
    $bg_color = $field['widget']['bg_color'];
    $color = $field['widget']['color'];
    $font = $field['widget']['font'];
    $font_size = $field['widget']['font_size'];
    $link = $field['widget']['preview_link'];
    if ($link == emwave_LINK_CONTENT) {
      $link = 'node/' . $node->nid;
    }
    else if ($link == emwave_LINK_PROVIDER) {
      $link = module_invoke('emfield', 'include_invoke', 'emwave', $item['provider'], 'embedded_link', $code, $item['data']);
    }
    else {
      $link = NULL;
    }
    $title = module_invoke('emfield', 'include_invoke', 'emwave', $item['provider'], 'wave_title', $code, $item['data']);
    $output = theme('emwave_wave', $field, $item, $formatter, $node, $code, $width, $height, $bg_color, $color, $font, $font_size, $title, $link);
  }
  return $output;
}

function theme_emwave_formatter_wave_embed($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emwave', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emwave_formatter_default($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emwave', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emwave_formatter_wave_full($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emwave', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emwave_formatter_wave_preview($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emwave', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emwave_wave_embed($field, $item, $formatter, $node) {
  if ($item['value'] && $item['provider']) {
    $output = drupal_get_form('emwave_embed_form', $field, $item, $formatter, $node);
  }
  return $output;
}
