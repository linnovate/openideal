<?php

// Plugin definition.
$plugin = array(
  'title' => t('One column stack'),
  'category' => t('OpenIdeaL'),
  'icon' => 'onecol_row_stack.png',
  'theme' => 'onecol_row_stack',
  'css' => 'onecol_row_stack.css',
  'regions' => array(
    'region_first' => t('First region (tablets - top of left column)'),
    'region_second' => t('Second region (tablets - bottom of left column)'),
    'region_third' => t('Third region (hidden on mobile)'),
    'region_fourth' => t('Fourth region (tablets - bottom of middle column)'),
  ),
);
