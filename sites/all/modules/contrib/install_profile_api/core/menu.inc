<?php
// $Id: menu.inc,v 1.1.2.6.2.3 2009/03/26 17:31:19 jamesandres Exp $

/**
 * Create a new top-level menu.
 *
 * @param $title
 *   The title of the menu as seen by users.
 * @param $menu_name
 *   Optional. System menu name.
 * @param $desc
 *   Optional. The description of the menu.
 * 
 * @return 
 *   Returns either FALSE if there was an error, or the system menu-name as stored in the db.
 */
function install_menu_create_menu($title, $menu_name = '', $desc = '') {
  $return = FALSE;

  if (module_exists('menu')) {
    $title            = $title;
    $description      = $desc;
    $mn               = (empty($menu_name)) ? $title : $menu_name;
    $menu_name        = str_replace(array(' ', '_'), '-', strtolower($mn));
    $menu_name_custom = 'menu-' . $menu_name;

    // Check the db for an existing menu by the same name.
    $menu = db_result(db_query_range("SELECT menu_name FROM {menu_custom} WHERE menu_name = '%s' OR menu_name = '%s'", $menu_name, $menu_name_custom, 0, 1));

    // If a menu does not exists then we create it.
    // Appending 'menu-' to the beginning is standard for the menu module
    // so we do the same thing.
    if (!$menu) {
      $menu = $menu_name_custom;
      
      // Set up data for menu_link table.
      $path = 'admin/build/menu-customize/';
      $link = array(
        'link_title'  => $title,
        'link_path'   => $path . $menu,
        'router_path' => $path . '%',
        'module'      => 'menu',
        'plid'        => db_result(db_query("SELECT mlid FROM {menu_links} WHERE link_path = '%s' AND module = '%s'", 'admin/build/menu', 'system')),
      );
      // Save link to menu_link table.
      menu_link_save($link);
      
      // Save menu to menu_custom table.
      db_query("INSERT INTO {menu_custom} (menu_name, title, description) VALUES ('%s', '%s', '%s')", $menu_name_custom, $title, $description);
    }
    
    $return = $menu;
  } else {
    drupal_set_message('Custom menu was not created. Please be sure that the menu module is a dependancy.');
  }

  return $return;
}

/**
 * Return a menu item based on the mlid.
 *
 * Good for testing the existence of a value or just getting the row data.
 *
 * @param $mlid
 *   The mlid requested.
 * @return
 *   A menu item if found, boolean FALSE otherwise.
 */
function install_menu_get_item($mlid) {
  return db_fetch_array(db_query_range("SELECT * FROM {menu_links} WHERE mlid = %d", $mlid));
}

/**
 * Get menu item information based on path or other information.
 * 
 * @param $path
 *   Search on path information string.
 * @param $menu_link_title
 *   Search on $meny_link_title string.
 * @param $plid
 *   Search on parent menu mlid.
 * @param $menu_name
 *   Search on the base menu name.
 *
 * @return
 *   An array of menu items matching the specified criteria.
 */
function install_menu_get_items($path = NULL, $menu_link_title = NULL, $plid = NULL, $menu_name = NULL) {
  $return = array();
  $query  = "SELECT mlid FROM {menu_links} WHERE ";

  $and = FALSE;
  $args = array();

  if (!is_null($path)) { 
    $query .= "link_path = '%s'";
    $args[] = $path;
    $and = TRUE;
  }
  if (!is_null($menu_link_title)) {
    $query .= ($and) ? " AND " : "";
    $query .= "link_title = '%s'";
    $args[] = $menu_link_title;
    $and = TRUE;
  }
  if (!is_null($plid)) {
    $query .= ($and) ? " AND " : "";
    $query .= "plid = %d";
    $args[] = $plid;
    $and = TRUE;
  }
  if (!is_null($menu_name)) {
    $query .= ($and) ? " AND " : "";
    $query .= "menu_name = '%s'";
    $args[] = $menu_name;
  }

  $result = db_query($query, $args);
  while ($row = db_fetch_array($result)) {
    $return[] = $row;
  }

  return $return;
}

/**
 * Set an existing menu ID to a new parent.
 *
 * @param $mlid
 *   The menu link id that is to be reassigned.
 * @param $plid
 *   The parent link id to assign to the mlid value.
 * @param $weight
 *   The weight desired to set this menu item to.
 */
function install_menu_set_menu($mlid, $plid = NULL, $weight = NULL) {
  $return = FALSE;
  $update = FALSE;

  $result = db_query("SELECT * FROM {menu_links} WHERE mlid = %d", $mlid);
  while ($row = db_fetch_array($result)) {
    $curr_menu = $row;
  }

  // See if we need to update the parent ID.
  if (!is_null($plid) && $plid !== $curr_menu['plid']) {
      drupal_set_message($curr_menu['plid']);
      $curr_menu['plid'] = $plid;
      $update = TRUE;
      $weight = (!is_null($weight)) ? $weight : 0 ;
  }

  // See if we need to update the weight
  if (!is_null($weight) && $curr_menu['weight']) {
    $curr_menu['weight'] = $weight;
    $update = TRUE;
  }

  // Update only if needed.
  if ($update) {
    $return = menu_link_save($curr_menu);
  }

  return $return;
}

/**
 * Create a new menu item.
 *
 * TODO: inspect the $path argument. The menu system will only register menu
 * items whose path is already in the DB.
 *
 * @param  $path
 *   Path of the new menu item
 * @param  $title
 *   Title of the menu item (visible label for menu).
 * @param  $description
 *   Description of the menu item (tooltip).
 * @param  $menu
 *   Menu to associate this link with.
 * @param  $plid
 *   Parent link ID.
 * @param  $weight
 *   Weight for positioning.
 * @param  $module
 *   Optional. Specify a specific module responsible for this menu item.
 * 
 * @return  integer
 *   The database MLID of the new menu item.
 */
function install_menu_create_menu_item($path, $title, $description = '', $menu = 'navigation', $plid = 0, $weight = 0, $module = 'menu', $hidden = 0, $has_children = 0, $expanded = 0, $customized = 0, $updated = 0) {
  // If menu exists then we should insure that the menu has been created.
  $menu = install_menu_create_menu($menu);

  $option_arr = array();
  // If a description is present.
  if (!empty($description)) {
    $option_arr['attributes'] = array('title' => $description);
  }

  $link = array(
    'menu_name'     => $menu,
    'weight'        => $weight,
    'link_title'    => $title,
    'options'       => $option_arr,
    'module'        => $module,
    'link_path'     => drupal_get_normal_path($path),
    'hidden'        => $hidden,
    'has_children'  => $has_children,
    'expanded'      => $expanded,
    'customized'    => $customized,
    'updated'       => $updated,
  );

  // Since menu_save_link will do a query if any plid is passed.
  // Omit it if this is a top level link.
  if ($plid) {
    $link['plid'] = $plid;
  }

  $return = menu_link_save($link);

  // If no $return link, then inform the installer that this path in not valid.
  if (!$return) {
    drupal_set_message('The menu path: <em>"' . $path . '"</em> is not a valid path. Please insure that the necessary modules are set as dependents.');
  }

  return $return;
}

/**
 * Creates a batch of menu items.
 *
 * @param $items
 *   An array containing menu items (as arrays).
 * @param $plid
 *   Parent Link ID for which menu the items are being added.
 */
function install_menu_create_menu_items($items, $plid) {
  foreach ($items as $id => $item) {
    // Format the data to send.
    $path = ($item['path']) ? $item['path'] : FALSE;
    $title = ($item['title']) ? $item['title'] : FALSE;
    $desc  = ($item['description']) ? $item['description'] : '';
    $menu  = ($item['menu']) ? $item['menu'] : 'navigation';
    $plid  = ($plid) ? $plid : 0;
    $weight = ($item['weight']) ? $item['weight'] : 0;
    $module = ($item['module']) ? $item['module'] : 'menu';
    $children = (isset($item['children'])) ? $item['children'] : FALSE;

    // Create the menu link.
    $mlid = install_menu_create_menu_item($path, $title, $desc, $menu, $plid, $weight, $module);

    // If there are children do them too.
    if ($children) {
      install_menu_create_menu_items($children, $mlid);
    }
  }
}

/**
 * Update a menu item.
 *
 * Use in conjunction with install_menu_get_menu_item().
 *
 * @param $menu
 *   The updated menu item array.
 * @return
 *   The mlid if successful, otherwise boolean FALSE.
 */
function install_menu_update_menu_item($menu) {
  $mlid = menu_link_save($menu);
  $return = ($mlid) ? $mlid : FALSE;
  return $return;
}


/**
 * Disable a specified menu item by mlid.
 *
 * @param $mlid
 *   The mlid of the menu item to disable.
 *
 */
function install_menu_disable_item($mlid, $unhide = FALSE) {
  $link = menu_link_load($mlid);
  $status = ($unhide) ? 0 : 1;
  // If the mlid exists then we can disable it.
  if ($link['mlid']) {
    $link['hidden'] = $status;
    db_query("UPDATE {menu_links} SET hidden = %d WHERE mlid = %d", $status, $mlid);
  }
}

/**
 * Add a URL alias.
 *
 * @param $src
 *   The source location of the url eg: node/1
 * @param $dst
 *   The destination location of the url.
 * @param $language
 *   The language to associate this with.
 *
 * @return
 *   The pid of the new row if successful, boolean FLASE otherwise.
 */
function install_menu_create_url_alias($src = '', $dst = '', $language = '') {
  $return = FALSE;
  
  // Only proceed if we are sent a src and a dst
  if (!empty($src) && !empty($dst)) {
    db_query("INSERT INTO {url_alias} (src, dst, language) VALUES ('%s', '%s', '%s')", $src, $dst, $language);
    $return = db_last_insert_id('url_alias', 'pid');
  }
  
  return $return;
}
