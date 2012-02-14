<?php
// $Id: image.imagemagick.inc,v 1.24.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Return information about the imagemagick toolkit.
 */
function image_imagemagick_info() {
  return array('name' => 'imagemagick', 'title' => 'ImageMagick Toolkit.');
}

/**
 * Validate and return toolkit specific settings.
 */
function image_imagemagick_settings() {
  $form['#after_build'] = array('_image_imagemagick_build_version');

  $form['imagemagick_binary'] = array(
    '#type' => 'fieldset',
    '#title' => t('ImageMagick binary'),
    '#collapsible' => FALSE,
    '#description' => t('ImageMagick is a standalone program used to manipulate images. To use it, it must be installed on your server and you need to know where it is located. If you are unsure of the exact path consult your ISP or server administrator.'),
  );
  $form['imagemagick_binary']['image_imagemagick_convert'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to the "convert" binary'),
    '#default_value' => variable_get('image_imagemagick_convert', '/usr/bin/convert'),
    '#required' => TRUE,
    '#description' => t('Specify the complete path to the ImageMagic <kbd>convert</kbd> binary. For example: <kbd>/usr/bin/convert</kbd> or <kbd>C:\Program Files\ImageMagick-6.3.4-Q16\convert.exe</kbd>.'),
  );
  $form['imagemagick_binary']['image_imagemagick_debugging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display debugging information'),
    '#default_value' => variable_get('image_imagemagick_debugging', 0),
    '#description' => t('Checking this option will display the ImageMagick commands and ouput to users with the <em>administer site configuration</em> permission.'),
  );
  return $form;
}

function _image_imagemagick_build_version($form, $form_element) {
  // Don't check the path when another toolkit is being selected.
  if (!isset($form['#post']['image_toolkit']) || $form['#post']['image_toolkit'] == 'imagemagick') {
    $valid = _image_imagemagick_check_path($form_element['values']['image_imagemagick_convert'], 'image_imagemagick_convert');
    if ($valid) {
      _image_imagemagick_convert_exec('-version', $output, $errors);
      $form['imagemagick_binary']['version'] = array(
        '#type' => 'item',
        '#value' => '<pre>' . check_plain(trim($output)) . '</pre>',
      );
    }
  }
  return $form;
}

function _image_imagemagick_check_path($path, $attach_error_to = FALSE) {
  if (file_exists($path)) {
    return TRUE;
  }
  if ($attach_error_to) {
    if ($open_basedir = ini_get('open_basedir')) {
      form_set_error($attach_error_to, t("No file %file could be found. PHP's <a href='@open_basedir'>open_basedir</a> security restriction is set to %open_basedir, which may be interfering with the attempts to locate ImageMagick.", array('%file' => $path, '%open_basedir' => $open_basedir, '@open_basedir' => 'http://php.net/features.safe-mode#ini.open-basedir')));
    }
    else {
      form_set_error($attach_error_to, t('The specified ImageMagic path %file does not exist.', array('%file' => $path)));
    }
  }
  return FALSE;
}

/**
 * Invoke hook_imagemagick_alter().
 *
 * Implementors of hook_imagemagick_alter() should accept three parameters: $op,
 * $filepath and &$args (passed by reference), which are described below.
 *
 * @param $op
 *   String with the operation: 'resize', 'crop', 'rotate'.
 * @param $filepath
 *   String containing the path to the image that is being processed.
 * @param $args
 *   Array containing ImageMagick options.
 *
 * @return
 *   Array of modified arguments.
 */
function _image_imagemagick_alter_invoke($op, $filepath, $args) {
  foreach (module_implements('imagemagick_alter') as $module) {
    $function = $module . '_imagemagick_alter';
    $function($op, $filepath, $args);
  }
  return $args;
}

/**
 * Resize an image to the given width and height.
 */
function image_imagemagick_resize($source, $dest, $width, $height) {
  $args = array('resize' => '-resize ' . (int) $width . 'x' . (int) $height . '!');
  $args = _image_imagemagick_alter_invoke('resize', $source, $args);
  return _image_imagemagick_convert($source, $dest, $args);
}

/**
 * Rotate an image.
 */
function image_imagemagick_rotate($source, $dest, $degrees, $bg_color = 0x000000) {
  $args = array(
    'rotate' => '-rotate ' . (float) $degrees,
    'background' => '-background #' . str_pad(dechex($bg_color), 6, 0),
  );
  $args = _image_imagemagick_alter_invoke('rotate', $source, $args);
  return _image_imagemagick_convert($source, $dest, $args);
}

/**
 * Crop an image to the specified dimensions.
 */
function image_imagemagick_crop($source, $dest, $x, $y, $width, $height) {
  $args = array('crop' => '-crop ' . (int) $width . 'x' . (int) $height . '+' . (int) $x . '+' . (int) $y . '!');
  $args = _image_imagemagick_alter_invoke('crop', $source, $args);
  return _image_imagemagick_convert($source, $dest, $args);
}

/**
 * Calls the convert executable with the specified filter.
 */
function _image_imagemagick_convert($source, $dest, $args) {
  $command = implode(' ', array(
    preg_replace("/[^A-Za-z0-9\!\.\-\+\_\/\040]/", '', implode(' ', $args)),
    escapeshellarg($source),
    escapeshellarg($dest),
  ));

  if (0 != _image_imagemagick_convert_exec($command, $output, $errors)) {
    return FALSE;
  }
  return file_exists($dest);
}

function _image_imagemagick_convert_exec($command_args, &$output, &$errors) {
  $convert_path = variable_get('image_imagemagick_convert', '/usr/bin/convert');
  if (!_image_imagemagick_check_path($convert_path)) {
    drupal_set_message(t("ImageMagick could not be found. The admin will need to set the path on the <a href='@image-toolkit-settings'>image toolkit page</a>.", array('@image-toolkit-settings' => url('admin/settings/image-toolkit'))), 'error');
    return FALSE;
  }

  if (strstr($_SERVER['SERVER_SOFTWARE'], 'Win32') || strstr($_SERVER['SERVER_SOFTWARE'], 'IIS')) {
    // Use Window's start command to avoid the "black window" from showing up:
    // http://us3.php.net/manual/en/function.exec.php#56599
    // Use /D to run the command from PHP's current working directory so the
    // file paths don't have to be absolute.
    $convert_path = 'start "window title" /D' . escapeshellarg(getcwd()) . ' /B ' . escapeshellarg($convert_path);
  }

  $descriptors = array(
    0 => array('pipe', 'r'), // stdin
    1 => array('pipe', 'w'), // stdout
    2 => array('pipe', 'w'), // stderr
  );
  if ($h = proc_open($convert_path . ' ' . $command_args, $descriptors, $pipes)) {
    $output = '';
    while (!feof($pipes[1])) {
      $output .= fgets($pipes[1]);
    }

    $errors = '';
    while (!feof($pipes[2])) {
      $errors .= fgets($pipes[2]);
    }

    // Display debugging information to authorized users.
    if (variable_get('image_imagemagick_debugging', FALSE) && user_access('administer site configuration')) {
      drupal_set_message(t('ImageMagick command: @command', array('@command' => $convert_path . ' ' . $command_args)));
      drupal_set_message(t('ImageMagick output: @output', array('@output' => $output)));
    }

    if ($errors) {
      drupal_set_message(t('ImageMagick reported an error: %error.', array('%error' => $errors)), 'error');
    }

    fclose($pipes[0]);
    fclose($pipes[1]);
    fclose($pipes[2]);
    return proc_close($h);
  }
  return FALSE;
}

