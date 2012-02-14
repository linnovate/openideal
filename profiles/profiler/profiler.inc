<?php

/**
 * Init install profile functions on behalf of the caller.
 */
function profiler_v2($profile_name) {
  $code =
<<<EOT
if (!function_exists('{$profile_name}_profile_details')) {
  function {$profile_name}_profile_details() {
    return profiler_v2_load_config('{$profile_name}');
  }
}

if (!function_exists('{$profile_name}_profile_modules')) {
  function {$profile_name}_profile_modules() {
    include_once('profiler_api.inc');
    return profiler_profile_modules(profiler_v2_load_config('{$profile_name}'));
  }
}

if (!function_exists('{$profile_name}_profile_task_list')) {
  function {$profile_name}_profile_task_list() {
    include_once('profiler_api.inc');
    return profiler_profile_task_list(profiler_v2_load_config('{$profile_name}'));
  }
}

if (!function_exists('{$profile_name}_profile_tasks')) {
  function {$profile_name}_profile_tasks(&\$task, \$url) {
    include_once('profiler_api.inc');
    return profiler_profile_tasks(profiler_v2_load_config('{$profile_name}'), \$task, \$url);
  }
}

if (!function_exists('{$profile_name}_form_alter')) {
  function {$profile_name}_form_alter(&\$form, \$form_state, \$form_id) {
    include_once('profiler_api.inc');
    return profiler_form_alter(profiler_v2_load_config('{$profile_name}'), \$form, \$form_state, \$form_id);
  }
}
EOT;
  eval($code);
}

/**
 * Load the config for a particular Install profile.
 *
 * @param $profile_name
 *   The machine name of the Install profile.
 * @param $parsed
 *   Whether to return the config of the install profile as a string or as a
 *   parsed array. Used primarily as an internal flag for managing inheritance.
 * @return
 *   The Install profiles Profiler include configuration, as an array.
 */
function profiler_v2_load_config($profile_name, $parsed = TRUE) {
  $file = "./profiles/{$profile_name}/{$profile_name}.info";
  // Support legacy .profiler.inc files as well.
  $file = is_file($file) ? $file : "./profiles/{$profile_name}/{$profile_name}.profiler.inc";
  $data = is_file($file) ? file_get_contents($file) : '';
  if ($data && $info = profiler_v2_parse_info_file($data)) {
    if (!empty($info['base']) && is_string($info['base'])) {
      $data = profiler_v2_load_config($info['base'], FALSE) . "\n" . $data;
    }
  }
  return $parsed ? profiler_v2_parse_info_file($data) : $data;
}

/**
 * Copy of drupal_parse_info_file() but must be passed a string rather than a
 * file path to read from.
 */
function profiler_v2_parse_info_file($data) {
  if (!$data) {
    return FALSE;
  }

  $constants = get_defined_constants();
  if (preg_match_all('
    @^\s*                           # Start at the beginning of a line, ignoring leading whitespace
    ((?:
      [^=;\[\]]|                    # Key names cannot contain equal signs, semi-colons or square brackets,
      \[[^\[\]]*\]                  # unless they are balanced and not nested
    )+?)
    \s*=\s*                         # Key/value pairs are separated by equal signs (ignoring white-space)
    (?:
      ("(?:[^"]|(?<=\\\\)")*")|     # Double-quoted string, which may contain slash-escaped quotes/slashes
      (\'(?:[^\']|(?<=\\\\)\')*\')| # Single-quoted string, which may contain slash-escaped quotes/slashes
      ([^\r\n]*?)                   # Non-quoted string
    )\s*$                           # Stop at the next end of a line, ignoring trailing whitespace
    @msx', $data, $matches, PREG_SET_ORDER)) {
    $info = array();
    foreach ($matches as $match) {
      // Fetch the key and value string
      $i = 0;
      foreach (array('key', 'value1', 'value2', 'value3') as $var) {
        $$var = isset($match[++$i]) ? $match[$i] : '';
      }
      $value = stripslashes(substr($value1, 1, -1)) . stripslashes(substr($value2, 1, -1)) . $value3;

      // Parse array syntax
      $keys = preg_split('/\]?\[/', rtrim($key, ']'));
      $last = array_pop($keys);
      $parent = &$info;

      // Create nested arrays
      foreach ($keys as $key) {
        if ($key == '') {
          $key = count($parent);
        }
        if (!isset($parent[$key]) || !is_array($parent[$key])) {
          $parent[$key] = array();
        }
        $parent = &$parent[$key];
      }

      // Handle PHP constants
      if (defined($value)) {
        $value = constant($value);
      }

      // Insert actual value
      if ($last == '') {
        $last = count($parent);
      }
      $parent[$last] = $value;
    }
    return $info;
  }
  return FALSE;
}
