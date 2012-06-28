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

  $radioactivity_decay_profile = new stdClass();
  $radioactivity_decay_profile->disabled = FALSE; /* Edit this to true to make a default radioactivity_decay_profile disabled initially */
  $radioactivity_decay_profile->api_version = 1;
  $radioactivity_decay_profile->machine_name = 'blast_idea';
  $radioactivity_decay_profile->name = 'Blast Idea';
  $radioactivity_decay_profile->description = 'Blast Idea - Cut to half every 7 days, check every 24 hours';
  $radioactivity_decay_profile->enable_decay = 1;
  $radioactivity_decay_profile->granularity = '86400';
  $radioactivity_decay_profile->half_life = 604800;
  $radioactivity_decay_profile->cut_off = '100';
  $radioactivity_decay_profile->storage = 'Live';
  $export['blast_idea'] = $radioactivity_decay_profile;

  $radioactivity_decay_profile = new stdClass();
  $radioactivity_decay_profile->disabled = FALSE; /* Edit this to true to make a default radioactivity_decay_profile disabled initially */
  $radioactivity_decay_profile->api_version = 1;
  $radioactivity_decay_profile->machine_name = 'buzz_idea';
  $radioactivity_decay_profile->name = 'Buzz Idea';
  $radioactivity_decay_profile->description = 'Buzz Idea - Cut to half every 30 days, check every 72 hours';
  $radioactivity_decay_profile->enable_decay = 1;
  $radioactivity_decay_profile->granularity = '259200';
  $radioactivity_decay_profile->half_life = 2592000;
  $radioactivity_decay_profile->cut_off = '100';
  $radioactivity_decay_profile->storage = 'Live';
  $export['buzz_idea'] = $radioactivity_decay_profile;

  $radioactivity_decay_profile = new stdClass();
  $radioactivity_decay_profile->disabled = FALSE; /* Edit this to true to make a default radioactivity_decay_profile disabled initially */
  $radioactivity_decay_profile->api_version = 1;
  $radioactivity_decay_profile->machine_name = 'groove_idea';
  $radioactivity_decay_profile->name = 'Groove Idea';
  $radioactivity_decay_profile->description = 'Groove Idea - Cut to half every 90 days, check every week';
  $radioactivity_decay_profile->enable_decay = 1;
  $radioactivity_decay_profile->granularity = '604800';
  $radioactivity_decay_profile->half_life = 7776000;
  $radioactivity_decay_profile->cut_off = '100';
  $radioactivity_decay_profile->storage = 'Live';
  $export['groove_idea'] = $radioactivity_decay_profile;

  return $export;
}
