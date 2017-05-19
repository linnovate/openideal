<?php

/**
 * @file
 * template.php
 */

/**
 * Returns HTML for a date element formatted as a range.
 */
function confucius_date_display_range($variables) {
  $date1 = $variables['date1'];
  $date2 = $variables['date2'];
  $timezone = $variables['timezone'];
  $attributes_start = $variables['attributes_start'];
  $attributes_end = $variables['attributes_end'];

  $start_date = '<span class="date-display-start"' . drupal_attributes($attributes_start) . '>' . $date1 . '</span>';
  $end_date = '<span class="date-display-end"' . drupal_attributes($attributes_end) . '>' . $date2 . $timezone . '</span>';

  // If microdata attributes for the start date property have been passed in,
  // add the microdata in meta tags.
  if (!empty($variables['add_microdata'])) {
    $start_date .= '<meta' . drupal_attributes($variables['microdata']['value']['#attributes']) . '/>';
    $end_date .= '<meta' . drupal_attributes($variables['microdata']['value2']['#attributes']) . '/>';
  }

  // Wrap the result with the attributes.
  return t('!start-date - !end-date', array(
    '!start-date' => $start_date,
    '!end-date' => $end_date,
  ));
}

function sheatufim_preprocess_page(&$variables) {
  global $user;
  if((arg(0) == 'node' && arg(1) == 'add' && arg(2) =='idea')|| arg(0) == 'community') {    
    $variables['page']['show_title'] = true;
  }

  $toboggan_denied = in_array('page__toboggan__denied', $variables['theme_hook_suggestions']);


  if ($toboggan_denied && $user->uid == 0) {
    $variables['theme_hook_suggestions'][] = 'page__user__login';
  }

}

function sheatufim_preprocess_node(&$variables) {
  if ($variables['node']->type == 'header_site') {
    if (isset ($variables['node']->field_banner_image['und'][0]['uri'])) {
      $url = file_create_url($variables['node']->field_banner_image['und'][0]['uri']);  
      $variables['bg_image'] = $url;
    }
  }
}

