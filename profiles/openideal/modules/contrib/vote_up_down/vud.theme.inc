<?php

/**
 * @file
 * Theme functions
 */

define('VUD_WIDGET_MESSAGE_ERROR', 0);
define('VUD_WIDGET_MESSAGE_DENIED', 1);

define('VUD_REQUIRED_CTOOLS_API', '2.0-alpha');

/**
 * Implements hook_ctools_plugin_type().
 */
function vud_ctools_plugin_type() {
  return array(
    'widgets' => array(
      'process' => 'vud_width_plugin_defaults',
        // Themes can offer this plugin.
      'load themes' => TRUE,
      'cache' => FALSE,
    ),
  );
}

/**
 * Provide defaults for widgets.
 */
function vud_width_plugin_defaults(&$plugin, $info) {
  $defaults = array(
    'render function' => 'theme_render_template',
    'extension' => '.tpl.php',
    'css' => $plugin['name'] . '.css',
  );

  $plugin += $defaults;
}

/**
 * Load metadata for a single widget without loading all widgets.
 */
function vud_widget_get($name) {
  ctools_include('plugins');
  return ctools_get_plugins('vud', 'widgets', $name);
}

/**
 * Load metadata for all widgets
 */
function vud_widget_get_all() {
  ctools_include('plugins');
  return ctools_get_plugins('vud', 'widgets');
}

/**
 * Load the names of all widgets for use in a select.
 *
 * This can be given directly to #options when choosing a widget.
 */
function vud_widget_get_names() {
  $names = array();
  foreach (vud_widget_get_all() as $name => $plugin) {
    $names[$name] = $plugin['title'];
  }

  asort($names);
  return $names;
}

/**
 * Implementation of hook_theme().
 */
function vud_theme() {
  return array(
    'vud_widget' => array(
      'function'  => 'vud_widget_proxy',
      'variables' => array('entity_id' => NULL, 'type' => NULL, 'tag' => NULL, 'widget_theme' => NULL, 'readonly' => NULL, 'widget_message_code' => NULL),
    ),
    'vud_votes' => array(
      'function'  => 'vud_votes_proxy',
      'variables' => array('entity_id' => NULL, 'type' => NULL, 'tag' => NULL, 'widget_theme' => NULL),
    ),
  );
}

/**
 * Mimic some theming workflow.
 *
 * We do this since we do not have standard theme overwrite because
 * is not possible to decide dinamically the path where the template is
 * located(in contrast with function names and template names).
 */
function vud_pseudo_theming($vote_entity, $template_type, $plugin, &$variables) {
  // let modules modify variables passed to the template
  $variables_type = 'vud_' . $template_type . '_template_variables';
  drupal_alter($variables_type, $variables);
  // let widget modify variables passed to the template
  if ($function = ctools_plugin_get_function($plugin, 'alter template variables')) {
    $function($template_type, $variables);
  }

  // and then provide template suggestions by hand
  $suggestions = module_invoke('vud_' . $vote_entity, 'template_suggestions', $template_type, $plugin, $variables['entity_id']);
  if (empty($suggestions)) {
    $template_file = $plugin['path'] . '/' . $plugin[$template_type . ' template'] . $plugin['extension'];
  }
  else {
    global $theme_key;
    $themes = list_themes();
    $current_theme = $themes[$theme_key];
    $paths = array(
      $plugin['path'],
      dirname($current_theme->filename),
    );
    // TODO: Fix this.
    //$template_file = drupal_find_theme_templates($paths, $suggestions, $plugin['extension']);
  }

  // TODO: Remove hardcode.
  $template_file = $plugin['path'] . '/' . $plugin['widget template'] . $plugin['extension'];

  return $template_file;
}

/**
 * Proxy widget function that hook_theme() calls.
 */
function vud_widget_proxy($variables) {
  global $user;
  $plugin = vud_widget_get($variables['widget_theme']);
  if (empty($plugin) || empty($plugin['widget template'])) {
    return;
  }

  $variables += array(
    'widget_message_code' => VUD_WIDGET_MESSAGE_ERROR,
    'readonly' => FALSE,
    'plugin' => $plugin,
    'tag' => variable_get('vud_tag', 'vote'),
    'id' => 'widget-' . $variables['type'] . '-' . $variables['entity_id'],
    'link_class_up' => 'vud-link-up',
    'link_class_down' => 'vud-link-down',
    'link_class_reset' => 'vud-link-reset',

    'link_up' => '',
    'link_down' => '',

    'class_up' => 'up-active',
    'class_down' => 'down-active',
    'class_reset' => 'reset-inactive',

    'show_links' => FALSE,
    'show_reset' => FALSE,
    'show_up_as_link' => FALSE,
    'show_down_as_link' => FALSE,

    'reset_long_text' => t('Reset your vote'),
    'reset_short_text' => t('(reset)'),
  );

  $type = $variables['type'];
  $entity_id = $variables['entity_id'];
  $tag = $variables['tag'];
  $widget_theme = $variables['widget_theme'];
  $readonly = $variables['readonly'];

  drupal_add_library('system', 'drupal.ajax');
  ctools_add_js('ajax-responder');
  ctools_include('ajax');
  // TODO: Move this to vud-comment.
  if ($type == 'comment') {
    drupal_add_css(drupal_get_path('module', 'vud_comment') . '/vud_comment.css');
  }

  // Search and add media files.
  vud_add_files('css', $plugin);
  vud_add_files('js', $plugin);

  $user_vote =  0;

  // Define access per operation, for example when the vote up operation is
  // active this means the down operation should be allowed, as the up vote was
  // already.
  $up_access = $down_access = FALSE;
  if (user_access('use vote up/down')) {
    $criteria = array(
      'entity_type' => $type,
      'entity_id' => $entity_id,
      'tag' => $variables['tag'],
    ) + votingapi_current_user_identifier();
    $previous_vote = votingapi_select_single_vote_value($criteria);

    $user_vote = isset($previous_vote) ? $previous_vote : 0;

    if (!$readonly) {

      $up_access = $user_vote <= 0;
      $down_access = $user_vote >= 0;
      $reset_active = vud_access_callback('reset vote up/down votes', $type, $entity_id, NULL, $tag);

      if (!$up_access && !$down_access) {
        // Although implementing modules have requested a not readonly widget,
        // we prevent access.
        $readonly = $variables['readonly'] = TRUE;
      }
    }
  }

  // Prepare links only if it's not readonly, or if readonly but we need to show
  // a messgae on deny.
  $message_on_deny = variable_get('vud_message_on_deny', FALSE);
  if (!$readonly || ($readonly && $message_on_deny)) {
    // Widget needs to show links.
    if ($up_access) {
      $variables['class_up'] = 'up-inactive';
      $variables['show_up_as_link'] = TRUE;
    }
    if ($down_access) {
      $variables['class_down'] = 'down-inactive';
      $variables['show_down_as_link'] = TRUE;
    }
    if ($up_access || $down_access) {
      $variables['show_reset'] = TRUE;
      $variables['class_reset'] = 'reset-active';
    }
    $variables['show_links'] = TRUE;
  }

  if (!$readonly) {
    // Widget has link(s).
    if ($up_access) {
      $token_up = drupal_get_token("vote/$type/$entity_id/1/$tag/$widget_theme");
      $variables['link_up'] = url("vote/$type/$entity_id/1/$tag/$widget_theme/$token_up/nojs");
      $variables['link_class_up'] .= ' use-ajax';
    }
    if ($down_access) {
      $token_down = drupal_get_token("vote/$type/$entity_id/-1/$tag/$widget_theme");
      $variables['link_down'] = url("vote/$type/$entity_id/-1/$tag/$widget_theme/$token_down/nojs");
      $variables['link_class_down'] .= ' use-ajax';
    }
    if ($up_access || $down_access) {
      $token_reset = drupal_get_token("votereset/$type/$entity_id/$tag/$widget_theme");
      $variables['link_reset'] = url("votereset/$type/$entity_id/$tag/$widget_theme/$token_reset");
      $variables['link_class_reset'] .= ' use-ajax';
    }
  }
  elseif ($readonly && $message_on_deny) {
    // Readonly widget, but with a deny messgae.
    ctools_include('modal');
    ctools_modal_add_js();
    if ($up_access) {
      $variables['link_class_up'] .= ' denied ctools-use-modal';
    }
    else {
      $variables['link_class_down'] .= ' denied ctools-use-modal';
    }
    if ($up_access || $down_access) {
      $variables['link_class_reset'] .= ' denied ctools-use-modal';
    }

    // TODO: Get default $widget_message_code or one from $variables.
    $widget_message_code = VUD_WIDGET_MESSAGE_DENIED;
    $variables['link_up'] = url(sprintf('vud/nojs/denied/%d', $widget_message_code));
    $variables['link_down'] = url(sprintf('vud/nojs/denied/%d', $widget_message_code));
    $variables['link_reset'] = url(sprintf('vud/nojs/denied/%d', $widget_message_code));
  }
  else {
    // Readonly widget.
    $variables['link_up'] = '#';
    $variables['link_down'] = '#';
    $variables['link_reset'] = '#';
  }

  $result_criteria = array(
    'entity_type' => $type,
    'entity_id' => $entity_id,
    'value_type' => 'points',
    'tag' => $tag,
    'function' => 'sum'
  );
  $raw_points = votingapi_select_single_result_value($result_criteria);
  $variables['raw_points'] = $raw_points;
  $vote_result = (int)$raw_points;

  $criteria = array(
    'entity_type' => $type,
    'entity_id' => $entity_id,
    'value_type' => 'points',
    'tag' => $tag,
    'function' => 'count'
  );
  $vote_count = (int)votingapi_select_single_result_value($criteria);
  $variables['vote_count'] = $vote_count;

  $variables['unsigned_points'] = $vote_result;

  if ($vote_result > 0) {
    $variables['class'] = 'positive';
    $variables['points'] = '+'. $vote_result;
  }
  else {
    $variables['points'] = $vote_result;
    if ($vote_result < 0) {
      $variables['class'] = 'negative';
    }
    else {
      $variables['class'] = 'neutral';
    }
  }
  $variables['vote_label'] = format_plural(abs($vote_result), 'vote', 'votes');

  $template_file = vud_pseudo_theming($type, 'widget', $plugin, $variables);
  return $plugin['render function']($template_file, $variables);
}

/**
 * Proxy votes display function, that hook_theme() calls.
 */
function vud_votes_proxy($variables) {
  $plugin = vud_widget_get($widget_theme);
  if (empty($plugin) || empty($plugin['votes template'])) {
    return;
  }

  $template_file = $plugin['path'] . '/' . $plugin['votes template'] . $plugin['extension'];
  $variables = array(
    'plugin' => $plugin,
    'id' => 'votes-' . $type . '-' . $entity_id,
  );

  $criteria = array(
    'entity_type' => $type,
    'entity_id' => $entity_id,
    'value_type' => 'points',
    'tag' => $tag,
    'function' => 'sum'
  );

  $vote_result = (int) votingapi_select_single_result_value($criteria);
  $variables['unsigned_points'] = $vote_result;

  if ($vote_result > 0) {
    $variables['class'] = 'positive';
    $variables['points'] = '+'. $vote_result;
  }
  else {
    $variables['points'] = $vote_result;
    if ($vote_result < 0) {
      $variables['class'] = 'negative';
    }
    else {
      $variables['class'] = 'neutral';
    }
  }
  $variables['vote_label'] = format_plural(abs($vote_result), 'vote', 'votes');

  vud_add_files('css', $plugin);  // Search and add the CSS files.
  vud_add_files('js', $plugin);   // Search and add the JS files.

  $template_file = vud_pseudo_theming($type, 'votes', $plugin, $variables);

  // not all widget use votes.tpl.php
  if (function_exists($plugin['render function'])) {
    return $plugin['render function']($template_file, $variables);
  }

  return '';
}

/**
 * Read and load all CSS or JS files in the selected widget directory.
 */
function vud_add_files($type, $plugin) {
  $function = 'drupal_add_' . $type;
  if (empty($plugin[$type])) {
    return;
  }

  if (is_string($plugin[$type])) {
    $css = array($plugin[$type]);
  }

  if (is_array($plugin[$type])) {
    $css = $plugin[$type];
  }

  if (!empty($css)) {
    foreach ($css as $file) {
      $function($plugin['path'] . '/' . $file);
    }
  }
}

/**
 * Function for the main voting handler with Ajax support.
 */
function vud_vote($type, $entity_id, $value, $tag, $widget, $token, $ajax = 'ajax') {
  // If the user is anonymous we don't need to check for a token.
  if (!is_numeric($value) || !drupal_valid_token($token, "vote/$type/$entity_id/$value/$tag/$widget", TRUE)) {
    return MENU_ACCESS_DENIED;
  }

  $vote = array();
  $casted_vote_criteria = array(
    'entity_type' => $type,
    'entity_id' => $entity_id,
    'tag' => $tag,
  ) + votingapi_current_user_identifier();
  $casted_vote = votingapi_select_single_vote_value($casted_vote_criteria);

  // Sanity-check the incoming values.
  if ($value > 0) {
    $value = 1;
  }
  elseif ($value < 0) {
    $value = -1;
  }
  else {
    // Invalid value.
    watchdog('vud', 'Invalid vote on @type @content_id, with value @value, tag @tag and token @token',
      array('@type' => $type, '@content_id' => $content_id, '@value' => $value, '@tag' => $tag, '@token' => $token));
    return;
  }

  $vote['value'] = $value;
  $vote['value_type'] = 'points';
  $tag = $tag ? $tag : variable_get('vud_tag', 'vote');
  $vote['tag'] = $tag;
  $vote['entity_id'] = $entity_id;
  $vote['entity_type'] = $type;
  $votes = array(0 => $vote);
  drupal_alter('vud_votes', $votes);

  // Do not allow to vote with the same value.
  if ($casted_vote == $votes[0]['value']) {
    return;
  }
  votingapi_set_votes($votes);

  if ($ajax == 'ajax') {
    $plugin = vud_widget_get($widget);
    $commands = array();

    if ($function = ctools_plugin_get_function($plugin, 'ajax render')) {
      $commands = $function($type, $entity_id, $value, $tag, $token, $widget);
    }
    else {
      $variables = array(
        'entity_id' =>$entity_id,
        'type' => $type,
        'tag' => $tag,
        'widget_theme' => $widget,
      );
      if (!empty($plugin['widget template'])) {
        $commands[] = ajax_command_replace("#widget-$type-$entity_id", theme('vud_widget', $variables));
      }

      if (!empty($plugin['votes template'])) {
        $commands[] = ajax_command_replace("#votes-$type-$entity_id", theme('vud_votes', $variables));
      }
    }

    print ajax_render($commands);
    exit;
  }
  else {
    drupal_goto($_SERVER['HTTP_REFERER']);
  }
}

/**
 * Function for the reset handler with Ajax support.
 */
function vud_reset($type, $entity_id, $tag, $widget, $token, $ajax = 'ajax') {
  if (drupal_valid_token($token, "votereset/$type/$entity_id/$tag/$widget", TRUE)) {
    $criteria = array(
      'entity_type' => $type,
      'entity_id' => $entity_id,
      'tag' => $tag,
    ) + votingapi_current_user_identifier();
    votingapi_delete_votes(votingapi_select_votes($criteria));
    votingapi_recalculate_results($type, $entity_id);
  }
  else {
    watchdog('vud',
      'Could not reset votes on @type @entity_id, tag @tag and token @token',
      array('@type' => $type, '@entity_id' => $entity_id, '@tag' => $tag, '@token' => $token));
    drupal_set_message(t('Oops! There was an error resetting your vote!'), 'warning');
  }

  if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && $_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest') {
    if ($ajax == 'ajax') {
      $plugin = vud_widget_get($widget);
      $commands = array();

      if ($function = ctools_plugin_get_function($plugin, 'ajax render')) {
        $commands = $function($type, $entity_id, $value, $tag, $token, $widget);
      }
      else {
        $variables = array(
          'entity_id' =>$entity_id,
          'type' => $type,
          'tag' => $tag,
          'widget_theme' => $widget,
        );
        if (!empty($plugin['widget template'])) {
          $commands[] = ajax_command_replace("#widget-$type-$entity_id", theme('vud_widget', $variables));
        }

        if (!empty($plugin['votes template'])) {
          $commands[] = ajax_command_replace("#votes-$type-$entity_id", theme('vud_votes', $variables));
        }
      }

      print ajax_render($commands);
      exit;
    }
  }
  else {
    drupal_goto($_SERVER['HTTP_REFERER']);
  }
}
