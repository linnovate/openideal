<?php

// Plugin definition.
$plugin = array(
  'title' => t('Tow column'),
  'category' => t('OpenIdeaL'),
  'icon' => 'tow_col.png',
  'theme' => 'tow_col',
  'css' => 'tow_col.css',
  'regions' => array(
    'region_first' => t('First region (tablets - top of left column)'),
    'region_second' => t('Second region (tablets - bottom of left column)'),
  ),
);
