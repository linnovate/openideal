<?php

/**
 * Prep the block structures for further manipulation
 */
function install_init_blocks() {
  _block_rehash();
}

/**
 * Add a new plain block provided by block module.
 */
function install_add_block($module, $delta, $theme, $status, $weight = 0, $region = '', $visibility = 0, $pages = '', $custom = 0, $throttle = 0, $title = '') {
  if ($status) {
    db_query("INSERT INTO {blocks} (module, delta, theme, status, weight, region, visibility, pages, custom, throttle, title) 
       VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, '%s', %d, %d, '%s')", 
       $module, $delta, $theme, $status, $weight, $region, $visibility, $pages, $custom, $throttle, $title);
  }
  else {
    db_query("UPDATE {blocks} SET status = 0 WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme); 
  }
}

/**
 * Position an existing block inside a region of a theme.
 *
 * TIP: To identify the $module and $delta, go to an existing site and visit
 * the /admin/build/block page. Hover over the 'configure' links - the module
 * and delta are the last two parts of the target url.
 *
 * @param $theme
 *   A theme name, eg. 'garland'
 * @param $region
 *   Available region: usually one of 'header', 'footer', 'left', 'right', 'content'
 * @param $module
 *   The name of the module that provides the block
 * @param $delta
 *   The block id.
 * @param $weight
 *   Block order within the region.
 * @param $visibility
 *   Optionally, set block visibility type
 * @param $pages
 *   Optionally, set which pages the block appears on, or PHP code to control
 *   the page visibility (use $visibility = 2 for PHP)
 * @param $custom
 *   Optionally, set as custom block
 * @param $throttle
 *   Optionally, set the block throttle
 * @param $title
 *   Optionally, set a custom block title
 */
function install_set_block($module, $delta, $theme, $region, $weight = 0, $visibility = NULL, $pages = NULL, $custom = NULL, $throttle = NULL, $title = NULL) {
  // Require SET statements and arguments
  $set = array("region = '%s'", "status = 1", "weight = %d");
  $args = array($region, $weight);

  // Add optional SET statements and arguments
  foreach (array('visibility', 'pages', 'custom', 'throttle', 'title') as $arg) {
    // Beware that double dollar-sign, '$$', notation.
    if ($$arg !== NULL) {
      $set[] = "$arg = '%s'";
      $args[] = $$arg;
    }
  }

  // Add the WHERE arguments
  $args[] = $module;
  $args[] = $delta;
  $args[] = $theme;

  db_query("UPDATE {blocks} SET " . implode(', ', $set) . " WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $args);
}

/**
* Disable a block within a theme.
*/
function install_disable_block($module, $delta, $theme) {
  db_query("UPDATE {blocks} SET region = '', status = 0 WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme);
}

/**
 * Creates a new block role.
 */
function install_add_block_role($module, $delta, $rid) {
  db_query("INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)", $module, $delta, $rid);
}

/**
 * Create a custom block (box).
 *
 * @param $body
 *   The body of the custom block.
 * @param $description
 *   The description of the custom block (for admin/build/blocks).
 * @param $format
 *   The input format for the block's body.
 *
 * @return
 *   The new block's delta.
 *
 * @see block_add_block_form_submit()
 */
function install_create_custom_block($body, $description, $format = FILTER_FORMAT_DEFAULT) {
  db_query("INSERT INTO {boxes} (body, info, format) VALUES ('%s', '%s', %d)", $body, $description, $format);
  return db_last_insert_id('boxes', 'bid');
}

