<?php
// $Id: template.php,v 1.3.4.5 2010/10/04 16:11:15 thecrow Exp $

/**
 * @file
 * Various examples for overwrite the theme's settings.
 *
 * For a simply replacement of your link's list, just put the row:
 * <?php print $service_links_rendered; ?>
 * in your file 'node.tpl.php' and disable the other visualization options 
 * under the configuration's page.
 *
 * If you need other transformation add 'template.php' under your theme folder
 * either integrate the functions below in your 'template.php'.
 *
 * WARNING: instead of 'themename' put the name of your theme and don't forget
 * to clean the cache if some change are not well updated.
 */

/**
 * Example 1: Create the variable $service_links_rendered for your 'page.tpl.php'.
 */
function themename_preprocess_page(&$vars) {
  if (module_exists('service_links')) {
    // Work also for not-node pages
    if (user_access('access service links') && service_links_show($vars['node'])) {
      $vars['service_links_rendered'] = theme('links', service_links_render($vars['node'], TRUE));
    }
  }
}

/**
 * Example 2: Add extra variable for your 'node.tpl.php' (b.e. $twitter).
 */
function themename_preprocess_node(&$vars) {
  if (module_exists('service_links')) {
    if (user_access('access service links') && service_links_show($vars['node'])) {
      $vars['twitter'] = theme('links', array($vars['node']->service_links['service-links-twitter']));
    }
  }  
}

/**
 * If something doesn't work well try this.
 */
function themename_preprocess(&$vars, $hook) {
  switch ($hook) {
    case 'node':
      if module_exists('service_links') {
        if (user_access('access service links') && service_links_show($vars['node'])) {
          $vars['twitter'] = theme('links', array($vars['node']->service_links['service-links-twitter']));
        }
      }
      break;
    case 'page':
      if module_exists('service_links') {
        if (user_access('access service links') && service_links_show($vars['node'])) {
          $vars['service_links'] = theme('links', service_links_render($vars['node'], TRUE));
        }
      }
      break;
  }
}
