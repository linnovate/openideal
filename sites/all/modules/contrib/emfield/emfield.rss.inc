<?php
// $Id: emfield.rss.inc,v 1.1.2.4 2008/10/08 17:39:09 alexua Exp $

/**
 * @file
 * This is the emfield.module's RSS settings file.
 */

function _emfield_nodeapi_rss(&$node, $op, $teaser = NULL, $page = NULL) {
  $files = array();
  $type = content_types($node->type);

  foreach ($type['fields'] as $field) {
    if (module_hook($field['type'], 'emfield_info')) {
      $items = (array)$node->{$field['field_name']};
      $rss_encl = module_invoke($field['type'], 'emfield_rss', $node, $items, $teaser);
      // about here let extras change the data
      // allow modules (such as emthumb, or a future emmetadata) to alter our data.
      foreach (module_implements('emfield_field_extra') as $module) {
        $args = array($op, &$node, $field, &$items, $teaser, $page, $module);
        $ret = call_user_func_array($module .'_emfield_field_extra', $args);
        if (is_array($rss_encl) && is_array($ret)) {
          while (list($delta, ) = each($rss_encl)) {
            if (is_array($ret[$delta]) && is_array($rss_encl[$delta])) {
              $rss_encl[$delta] = $ret[$delta] + $rss_encl[$delta];
            }
          }
        }
      }

      if (is_array($rss_encl)) {
        // by now the delta doesn't matter we have enough data in the individual arrays
        $files = array_merge($files, $rss_encl);
      }
    }
  }

  $enclosure = array();
  $rss = array();
  
  $rss[] = array('namespace' => array('xmlns:media' => 'http://search.yahoo.com/mrss/'));

  foreach ($files as $file) {
    // RRS2 enclosure http://cyber.law.harvard.edu/rss/rss.html#ltenclosuregtSubelementOfLtitemgt
    if (
      count($enclosure) < 1 && // only one <enclosure> per <item>
      isset($file['filepath']) && isset($file['filesize']) && isset($file['filemime']) &&
      $file['filepath'] != '' && $file['filesize'] > 0 && $file['filemime'] != ''
    ) {
      $enclosure = array(
        'key' => 'enclosure',
        'attributes' => array(
          'url' => check_url($file['filepath']),
          'length' => (int) $file['filesize'],
          'type' => check_plain($file['filemime']),
        )
      );
    }

    // MRSS media:content http://search.yahoo.com/mrss
    $media = array();
    if (isset($file['filepath']) && $file['filepath'] != '') {
      // actually optional if media:player were to be specified
      $media['url'] = check_url($file['filepath']);
      // the rest of these are optional
      if (isset($file['filesize']) && $file['filesize'] > 1) {
        $media['fileSize'] = (int) $file['filesize'];
      }
      if ($file['filemime']) {
        $media['type'] = check_plain($file['filemime']);
      }
      if (isset($file['medium']) && $file['medium'] != '') {
        $media['medium'] = check_plain($file['medium']);
      }
      // media:isDefault, may be good for multiple CCK fields - ignored for now
      if (isset($file['expression']) && $file['expression'] != '') {
        $media['expression'] = check_plain($file['expression']);
      }
      if (isset($file['bitrate']) && $file['bitrate'] > 0) {
        $media['bitrate'] = (int) $file['bitrate'];
      }
      if (isset($file['framerate']) && $file['framerate'] > 0) {
        $media['framerate'] = (int) $file['framerate'];
      }
      if (isset($file['sampling_rate']) && $file['sampling_rate'] > 0) {
        $media['samplingrate'] = (int) $file['sampling_rate'];
      }
      if (isset($file['channels']) && $file['channels'] > 0) {
        $media['channels'] = check_plain($file['channels']);
      }
      if (isset($file['duration']) && $file['duration'] > 0) {
        $media['duration'] = (int) $file['duration'];
      }
      if (
        isset($file['width']) && isset($file['height']) &&
        $file['width'] > 0 && $file['height'] > 0
      ) {
        $media['width'] = (int) $file['width'];
        $media['height'] = (int) $file['height'];
      }
      // media:lang will be interesting

      $mrss_thumbnail = array();
      if (is_array($file['thumbnail']) && $file['thumbnail']['filepath'] != '') {
        $thumbnail = array();
        $thumbnail['url'] = check_url($file['thumbnail']['filepath']);
        if (isset($file['thumbnail']['height']) && $file['thumbnail']['height'] > 0) {
          $thumbnail['height'] = (int) $file['thumbnail']['height'];
        }
        if (isset($file['thumbnail']['width']) && $file['thumbnail']['width'] > 0) {
          $thumbnail['width'] = (int) $file['thumbnail']['width'];
        }
        if (isset($file['thumbnail']['time']) && $file['thumbnail']['time'] > 0) {
          $thumbnail['time'] = (int) $file['thumbnail']['time'];
        }
        $mrss_thumbnail = array('key' => 'media:thumbnail', 'attributes' => $thumbnail);
      }
      $rss[] = array('key' => 'media:content', 'attributes' => $media, 'value' => array($mrss_thumbnail));
    }
  }

  $rss[] = $enclosure;

  return $rss;
}
