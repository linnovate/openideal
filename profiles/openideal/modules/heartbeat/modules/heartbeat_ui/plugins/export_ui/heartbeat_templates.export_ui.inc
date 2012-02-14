<?php

/**
 * @file
 * Contains ctools export_ui plugin for heartbeat templates.
 */

$plugin = array(

  'schema' => 'heartbeat_messages',
  'access' => 'admin heartbeat templates',

  'menu' => array(
    'menu prefix' => 'admin/structure/heartbeat', // default
    'menu item' => 'templates', // default
    'menu title' => 'Heartbeat templates',
    'menu description' => 'Administer heartbeat templates.',
  ),

  'title singular' => t('Heartbeat template'),
  'title singular proper' => t('Heartbeat template'),
  'title plural' => t('Heartbeat templates'),
  'title plural proper' => t('Heartbeat templates'),

  // $form_state['values'] to $form_state['item'].
  // All callbacks will accept &$form and &$form_state as arguments.
  'form' => array(
    'settings' => 'heartbeat_ui_ctools_export_ui_form',
    'validate' => 'heartbeat_ui_ctools_export_ui_form_validate',
    'submit' => 'heartbeat_ui_ctools_export_ui_form_submit',
  ),

  'handler' => array(
     'class' => 'ctools_export_ui_heartbeat_template',
     'parent' => 'ctools_export_ui',
   ),

);
