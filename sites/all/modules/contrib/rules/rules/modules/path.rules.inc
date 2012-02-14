<?php
// $Id: path.rules.inc,v 1.1.2.7 2009/08/25 18:16:42 fago Exp $


/**
 * @file Rules integration for the path module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_condition_info().
 */
function path_rules_condition_info() {
  return array(
    'rules_condition_url_has_alias' => array(
      'label' => t('Path has alias(es)'),
      'module' => 'Path',
      'eval input' => array('src'),
    ),
    'rules_condition_alias_exists' => array(
      'label' => t('URL alias exists'),
      'module' => 'Path',
      'eval input' => array('dst'),
    ),
  );
}

/**
 * Condition implementation: Check if the path has an alias
 */
function rules_condition_url_has_alias($settings) {
  return (bool)drupal_lookup_path('alias', $settings['src'], $settings['language']);
}

/**
 * Condition implementation: Check if the URL alias already exists
 */
function rules_condition_alias_exists($settings) {
  $dst = rules_clean_path($settings['dst'], $settings['replace']);
  return (bool)drupal_lookup_path('source', $dst, $settings['language']);
}

/**
 * Implementation of hook_rules_action_info().
 */
function path_rules_action_info() {
  return array(
    'rules_action_path_alias' => array(
      'label' => t('Create or delete an URL alias'),
      'module' => 'Path',
      'eval input' => array('src', 'dst'),
    ),
    'rules_action_node_path_alias' => array(
      'label' => t("Create or delete a content's URL alias"),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Path',
      'eval input' => array('dst'),
    ),
  );
}

/**
 * Action Implementation
 */
function rules_action_path_alias($settings) {
  $dst = rules_clean_path($settings['dst'], $settings['replace']);
  // Only set the alias if the path alias isn't taken yet.
  if (!drupal_lookup_path('source', $dst)) {
    path_set_alias($settings['src'], $dst, NULL, $settings['language']);
  }
}

/**
 * Action Implementation
 */
function rules_action_node_path_alias($node, $settings) {
  $dst = rules_clean_path($settings['dst'], $settings['replace']);
  // Only set the alias if the path alias isn't taken yet.
  if (!drupal_lookup_path('source', $dst)) {
    $node->path = $dst;
    if (user_access('create url aliases') || user_access('administer url aliases')) {
      // Path module is going to save the alias on node save.
      return array('node' => $node);
    }
    else {
      path_set_alias('node/'. $node->nid, $node->path, isset($node->pid) ? $node->pid : NULL, isset($node->language) ? $node->language : '');
    }
  }
}

/**
 * Cleans the given path by replacing non ASCII characters with the given
 * $replace parameter
 *
 * Furthermore it allows customization by overriding the variable rules_clean_path in
 * the settings.php
 */
function rules_clean_path($path, $replace) {
  if ($replace) {
    $array = variable_get('rules_clean_path', array('/[^a-zA-Z0-9\/\-_]+/', $replace));
    $array[2] = $path;
    //replace it and remove trailing and leading replacement characters
    return trim(call_user_func_array('preg_replace', $array), $replace);
  }
  else {
    return $path;
  }
}

/**
 * @}
 */
