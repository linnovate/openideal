<?php
/**
 * @file
 * fe_idea.radioactivity_decay_profile.inc
 */

/**
 * Implements hook_default_radioactivity_decay_profile().
 */
function fe_idea_default_radioactivity_decay_profile() {
  $export = array();

  $radioactivity_decay_profile = new stdClass;
  $radioactivity_decay_profile->disabled = FALSE; /* Edit this to true to make a default radioactivity_decay_profile disabled initially */
  $radioactivity_decay_profile->api_version = 1;
  $radioactivity_decay_profile->machine_name = 'idea';
  $radioactivity_decay_profile->name = 'Idea';
  $radioactivity_decay_profile->description = '';
  $radioactivity_decay_profile->enable_decay = 1;
  $radioactivity_decay_profile->granularity = '43200';
  $radioactivity_decay_profile->half_life = 302400;
  $radioactivity_decay_profile->cut_off = '0.1';
  $radioactivity_decay_profile->storage = 'Live';
  $export['idea'] = $radioactivity_decay_profile;

  return $export;
}
