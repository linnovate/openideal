<?php
// $Id$

/**
 * Implementation of hook_theme().
 */
function rubik_theme() {
  $items = array();

  // theme('filter_form') for nicer filter forms.
  $items['filter_form'] = array('arguments' => array('form' => array()));

  // theme('blocks') targeted override for content region.
  $items['blocks_content'] = array('arguments' => array('doit' => FALSE));

  // Content theming.
  $items['help'] =
  $items['node'] =
  $items['comment'] = array(
    'path' => drupal_get_path('theme', 'rubik') .'/templates',
    'template' => 'object',
  );
  $items['node']['template'] = 'node';

  // Help pages really need help. See preprocess_page().
  $items['help_page'] = array(
    'arguments' => array('content' => array()),
    'path' => drupal_get_path('theme', 'rubik') .'/templates',
    'template' => 'object',
  );

  // Form layout: simple.
  $items['filter_admin_overview'] =
  $items['user_admin_perm'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('theme', 'rubik') .'/templates',
    'template' => 'form-simple',
    'preprocess functions' => array(
      'rubik_preprocess_form_buttons',
      'rubik_preprocess_form_legacy'
    ),
  );

  // Form layout: default (2 column).
  $items['block_add_block_form'] =
  $items['block_admin_configure'] =
  $items['comment_form'] =
  $items['contact_admin_edit'] =
  $items['contact_mail_page'] =
  $items['contact_mail_user'] =
  $items['filter_admin_format_form'] =
  $items['forum_form'] =
  $items['locale_languages_edit_form'] =
  $items['locale_languages_configure_form'] =
  $items['menu_edit_menu'] =
  $items['menu_edit_item'] =
  $items['node_type_form'] =
  $items['path_admin_form'] =
  $items['system_settings_form'] =
  $items['system_themes_form'] =
  $items['system_modules'] =
  $items['system_actions_configure'] =
  $items['taxonomy_form_term'] =
  $items['taxonomy_form_vocabulary'] =
  $items['user_pass'] =
  $items['user_login'] =
  $items['user_register'] =
  $items['user_profile_form'] =
  $items['user_admin_access_add_form'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('theme', 'rubik') .'/templates',
    'template' => 'form-default',
    'preprocess functions' => array(
      'rubik_preprocess_form_buttons',
      'rubik_preprocess_form_legacy',
      'rubik_preprocess_form_filter',
    ),
  );

  // These forms require additional massaging.
  $items['confirm_form'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('theme', 'rubik') .'/templates',
    'template' => 'form-simple',
    'preprocess functions' => array(
      'rubik_preprocess_form_confirm'
    ),
  );
  $items['node_form'] = array(
    'arguments' => array('form' => array()),
    'path' => drupal_get_path('theme', 'rubik') .'/templates',
    'template' => 'form-default',
    'preprocess functions' => array(
      'rubik_preprocess_form_filter',
      'rubik_preprocess_form_buttons',
      'rubik_preprocess_form_node',
    ),
  );

  return $items;
}

/**
 * Preprocessor for theme('page').
 */
function rubik_preprocess_page(&$vars) {
  // Show a warning if base theme is not present.
  if (!function_exists('tao_theme') && user_access('administer site configuration')) {
    drupal_set_message(t('The Rubik theme requires the !tao base theme in order to work properly.', array('!tao' => l('Tao', 'http://code.developmentseed.org/tao'))), 'warning');
  }

  // Split page content & content blocks.
  $vars['content_region'] = theme('blocks_content', TRUE);

  // Set a page icon class.
  $vars['page_icon_class'] = ($item = menu_get_item()) ? _rubik_icon_classes($item['href']) : '';

  // Add body class for theme.
  $vars['attr']['class'] .= ' rubik';

  // Body class for admin module.
  $vars['attr']['class'] .= ' admin-static';

  // Help pages. They really do need help.
  if (strpos($_GET['q'], 'admin/help/') === 0) {
    $vars['content'] = theme('help_page', $vars['content']);
  }

  // Display user account links.
  $vars['user_links'] = _rubik_user_links();

  // Help text toggler link.
  $vars['help_toggler'] = l(t('Help'), $_GET['q'], array('attributes' => array('id' => 'help-toggler', 'class' => 'toggler'), 'fragment' => 'help-text'));

  // Clear out help text if empty.
  if (empty($vars['help']) || !(strip_tags($vars['help']))) {
    $vars['help'] = '';
  }
}

/**
 * Preprocessor for theme('fieldset').
 */
function rubik_preprocess_fieldset(&$vars) {
  if (!empty($vars['element']['#collapsible'])) {
    $vars['title'] = "<span class='icon'></span>" . $vars['title'];
  }
}

/**
 * Attempts to render a non-template based form for template rendering.
 */
function rubik_preprocess_form_legacy(&$vars) {
  if (isset($vars['form']['#theme']) && function_exists("theme_{$vars['form']['#theme']}")) {
    $function = "theme_{$vars['form']['#theme']}";
    $vars['form'] = array(
      '#type' => 'markup',
      '#value' => $function($vars['form'])
    );
  }
}

/**
 * Preprocessor for handling form button for most forms.
 */
function rubik_preprocess_form_buttons(&$vars) {
  if (empty($vars['buttons']) || !element_children($vars['buttons'])) {
    if (isset($vars['form']['buttons']) && element_children($vars['form']['buttons'])) {
      $vars['buttons'] = $vars['form']['buttons'];
      unset($vars['form']['buttons']);
    }
    else {
      $vars['buttons'] = array();
      foreach (element_children($vars['form']) as $key) {
        if (isset($vars['form'][$key]['#type']) && in_array($vars['form'][$key]['#type'], array('submit', 'button'))) {
          $vars['buttons'][$key] = $vars['form'][$key];
          unset($vars['form'][$key]);
        }
      }
    }
  }
}

/**
 * Preprocessor for theme('confirm_form').
 */
function rubik_preprocess_form_confirm(&$vars) {
  // Move the title from the page title (usually too big and unwieldy)
  $title = filter_xss_admin(drupal_get_title());
  $vars['form']['description']['#type'] = 'item';
  $vars['form']['description']['#value'] = empty($vars['form']['description']['#value']) ?
    "<strong>{$title}</strong>" :
    "<strong>{$title}</strong><p>{$vars['form']['description']['#value']}</p>";
  drupal_set_title(t('Please confirm'));

  // Button setup
  $vars['buttons'] = $vars['form']['actions'];
  unset($vars['form']['actions']);
}

/**
 * Preprocessor for theme('node_form').
 */
function rubik_preprocess_form_node(&$vars) {
  $vars['sidebar'] = isset($vars['sidebar']) ? $vars['sidebar'] : array();
  // Support nodeformcols if present.
  if (module_exists('nodeformcols')) {
    $map = array(
      'nodeformcols_region_right' => 'sidebar',
      'nodeformcols_region_footer' => 'footer',
      'nodeformcols_region_main' => NULL,
    );
    foreach ($map as $region => $target) {
      if (isset($vars['form'][$region])) {
        if (isset($vars['form'][$region]['#prefix'], $vars['form'][$region]['#suffix'])) {
          unset($vars['form'][$region]['#prefix']);
          unset($vars['form'][$region]['#suffix']);
        }
        if (isset($vars['form'][$region]['buttons'], $vars['form'][$region]['buttons'])) {
          $vars['buttons'] = $vars['form'][$region]['buttons'];
          unset($vars['form'][$region]['buttons']);
        }
        if (isset($target)) {
          $vars[$target] = $vars['form'][$region];
          unset($vars['form'][$region]);
        }
      }
    }
  }
  // Default to showing taxonomy in sidebar if nodeformcols is not present.
  elseif (isset($vars['form']['taxonomy'])) {
    $vars['sidebar']['taxonomy'] = $vars['form']['taxonomy'];
    unset($vars['form']['taxonomy']);
  }
}

/**
 * Preprocessor for formatting input filter forms.
 */
function rubik_preprocess_form_filter(&$vars) {
  _rubik_filter_form_alter($vars['form']);
}

/**
 * Preprocessor for theme('form_element').
 */
function rubik_preprocess_form_element(&$vars) {
  if (!empty($vars['element']['#rubik_filter_form'])) {
    $vars['attr']['class'] .= ' form-item-filter';
  }
}

/**
 * Preprocessor for theme('help').
 */
function rubik_preprocess_help(&$vars) {
  $vars['hook'] = 'help';
  $vars['attr']['id'] = 'help-text';
  $class = 'path-admin-help clear-block toggleable';
  $vars['attr']['class'] = isset($vars['attr']['class']) ? "{$vars['attr']['class']} $class" : $class;
  $help = menu_get_active_help();
  if (($test = strip_tags($help)) && !empty($help)) {
    // Thankfully this is static cached.
    $vars['attr']['class'] .= menu_secondary_local_tasks() ? ' with-tabs' : '';

    $vars['is_prose'] = TRUE;
    $vars['layout'] = TRUE;
    $vars['content'] = "<span class='icon'></span>" . $help;

    // Link to help section.
    $item = menu_get_item('admin/help');
    if ($item && $item['path'] === 'admin/help' && $item['access']) {
      $vars['links'] = l(t('More help topics'), 'admin/help');
    }
  }
}

/**
 * Preprocessor for theme('help_page').
 */
function rubik_preprocess_help_page(&$vars) {
  $vars['hook'] = 'help-page';
  $vars['is_prose'] = TRUE;
  $vars['layout'] = TRUE;
  $vars['attr'] = array('class' => 'help-page clear-block');

  // Truly hackish way to navigate help pages.
  $module_info = module_rebuild_cache();
  $modules = array();
  foreach (module_implements('help', TRUE) as $module) {
    if (module_invoke($module, 'help', "admin/help#$module", NULL)) {
      $modules[$module] = $module_info[$module]->info['name'];
    }
  }
  asort($modules);
  $links = array();
  foreach ($modules as $module => $name) {
    $links[] = array('title' => $name, 'href' => "admin/help/{$module}");
  }
  $vars['links'] = theme('links', $links);
}

/**
 * Preprocessor for theme('node').
 */
function rubik_preprocess_node(&$vars) {
  $vars['layout'] = TRUE;
  $vars['title'] = menu_get_object() === $vars['node'] ? '' : $vars['title'];
  $vars['attr']['class'] .= ' clear-block';

  // Clear out template file suggestions if we are the active theme.
  // Other subthemes will need to manage template suggestions on their own.
  global $theme_key;
  if (in_array($theme_key, array('rubik', 'cube'), TRUE)) {
    $vars['template_files'] = array();
  }
}

/**
 * Preprocessor for theme('comment').
 */
function rubik_preprocess_comment(&$vars) {
  $vars['layout'] = TRUE;
  $vars['attr']['class'] .= ' clear-block';
}

/**
 * Preprocessor for theme('comment_wrapper').
 */
function rubik_preprocess_comment_wrapper(&$vars) {
  $vars['hook'] = 'box';
  $vars['title'] = t('Comments');

  $vars['attr']['id'] = 'comments';
  if (!isset($vars['attr']['class'])) {
    $vars['attr']['class'] = ' clear-block';
  }
  else {
    $vars['attr']['class'] .= ' clear-block';
  }
}

/**
 * Override of theme_blocks() for content region. Allows content blocks
 * to be split away from page content in page template. See tao_blocks()
 * for how this function is called.
 */
function rubik_blocks_content($doit = FALSE) {
  static $blocks;
  if (!isset($blocks)) {
    $blocks = module_exists('context') && function_exists('context_blocks') ? context_blocks('content') : theme_blocks('content');
  }
  return $doit ? $blocks : '';
}

/**
 * Override of theme('breadcrumb').
 */
function rubik_breadcrumb($breadcrumb, $prepend = TRUE) {
  $output = '';

  // Add current page onto the end.
  if (!drupal_is_front_page()) {
    $item = menu_get_item();
    $end = end($breadcrumb);
    if ($end && strip_tags($end) !== $item['title']) {
      $breadcrumb[] = "<strong>". check_plain($item['title']) ."</strong>";
    }
  }

  // Remove the home link.
  foreach ($breadcrumb as $key => $link) {
    if (strip_tags($link) === t('Home')) {
      unset($breadcrumb[$key]);
      break;
    }
  }

  // Optional: Add the site name to the front of the stack.
  if ($prepend) {
    $site_name = empty($breadcrumb) ? "<strong>". check_plain(variable_get('site_name', '')) ."</strong>" : l(variable_get('site_name', ''), '<front>', array('purl' => array('disabled' => TRUE)));
    array_unshift($breadcrumb, $site_name);
  }

  foreach ($breadcrumb as $link) {
    $output .= "<span class='breadcrumb-link'>{$link}</span>";
  }
  return $output;
}

/**
 * Display the list of available node types for node creation.
 */
function rubik_node_add_list($content) {
  $output = "<ul class='admin-list'>";
  if ($content) {
    foreach ($content as $item) {
      $item['title'] = "<span class='icon'></span>" . filter_xss_admin($item['title']);
      if (isset($item['localized_options']['attributes']['class'])) {
        $item['localized_options']['attributes']['class'] .= ' '. _rubik_icon_classes($item['href']);
      }
      else {
        $item['localized_options']['attributes']['class'] = _rubik_icon_classes($item['href']);
      }
      $item['localized_options']['html'] = TRUE;
      $output .= "<li>";
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      $output .= '<div class="description">'. filter_xss_admin($item['description']) .'</div>';
      $output .= "</li>";
    }
  }
  $output .= "</ul>";
  return $output;
}

/**
 * Override of theme_admin_block_content().
 */
function rubik_admin_block_content($content, $get_runstate = FALSE) {
  static $has_run = FALSE;
  if ($get_runstate) {
    return $has_run;
  }
  $has_run = TRUE;
  $output = '';
  if (!empty($content)) {
    foreach ($content as $k => $item) {
      $content[$k]['title'] = "<span class='icon'></span>" . filter_xss_admin($item['title']);
      $content[$k]['localized_options']['html'] = TRUE;
      if (!empty($content[$k]['localized_options']['attributes']['class'])) {
        $content[$k]['localized_options']['attributes']['class'] .= _rubik_icon_classes($item['href']);
      }
      else {
        $content[$k]['localized_options']['attributes']['class'] = _rubik_icon_classes($item['href']);
      }
    }
    $output = system_admin_compact_mode() ? '<ul class="admin-list admin-list-compact">' : '<ul class="admin-list">';
    foreach ($content as $item) {
      $output .= '<li class="leaf">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (!system_admin_compact_mode()) {
        $output .= "<div class='description'>{$item['description']}</div>";
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Override of theme('admin_drilldown_menu_item_link').
 */
function rubik_admin_drilldown_menu_item_link($link) {
  $link['localized_options'] = empty($link['localized_options']) ? array() : $link['localized_options'];
  $link['localized_options']['html'] = TRUE;
  if (!isset($link['localized_options']['attributes']['class'])) {
    $link['localized_options']['attributes']['class'] = _rubik_icon_classes($link['href']);
  }
  else {
    $link['localized_options']['attributes']['class'] .= ' '. _rubik_icon_classes($link['href']);
  }
  $link['description'] = check_plain(truncate_utf8(strip_tags($link['description']), 150, TRUE, TRUE));
  $link['description'] = "<span class='icon'></span>" . $link['description'];
  $link['title'] .= !empty($link['description']) ? "<span class='menu-description'>{$link['description']}</span>" : '';
  $link['title'] = filter_xss_admin($link['title']);
  return l($link['title'], $link['href'], $link['localized_options']);
}

/**
 * Override of theme('textfield').
 */
function rubik_textfield($element) {
  if ($element['#size'] >= 30) {
    $element['#size'] = '';
    $element['#attributes']['class'] = isset($element['#attributes']['class']) ? "{$element['#attributes']['class']} fluid" : "fluid";
  }
  return theme_textfield($element);
}

/**
 * Override of theme('password').
 */
function rubik_password($element) {
  if ($element['#size'] >= 30 || $element['#maxlength'] >= 30) {
    $element['#size'] = '';
    $element['#attributes']['class'] = isset($element['#attributes']['class']) ? "{$element['#attributes']['class']} fluid" : "fluid";
  }
  return theme_password($element);
}

/**
 * Override of theme('node_submitted').
 */
function rubik_node_submitted($node) {
  return _rubik_submitted($node);
}

/**
 * Override of theme('comment_submitted').
 */
function rubik_comment_submitted($comment) {
  $comment->created = $comment->timestamp;
  return _rubik_submitted($comment);
}

/**
 * Override of theme('filter_tips_more_info').
 */
function rubik_filter_tips_more_info() {
  return '<div class="filter-help">'. l(t('Formatting help'), 'filter/tips', array('attributes' => array('target' => '_blank'))) .'</div>';
}

/**
 * Theme a filter form element
 */
function rubik_filter_form($form) {
  if (isset($form['#title'])) {
    unset($form['#title']);
  }
  $select = '';
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#type']) && $form[$key]['#type'] === 'radio') {
      $select .= drupal_render($form[$key]);
    }
  }
  $help = theme('filter_tips_more_info');
  $output = "<div class='filter-options clear-block'>{$select}{$help}</div>";
  return $output;
}

/**
 * Helper function for cloning and drupal_render()'ing elements.
 */
function rubik_render_clone($elements) {
  static $instance;
  if (!isset($instance)) {
    $instance = 1;
  }
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]['#id'])) {
      $elements[$key]['#id'] = "{$elements[$key]['#id']}-{$instance}";
    }
  }
  $instance++;
  return drupal_render($elements);
}

/**
 * Helper function to submitted info theming functions.
 */
function _rubik_submitted($node) {
  $byline = t('Posted by !username', array('!username' => theme('username', $node)));
  $date = format_date($node->created, 'small');
  return "<div class='byline'>{$byline}</div><div class='date'>$date</div>";
}

/**
 * User/account related links.
 */
function _rubik_user_links() {
  // Add user-specific links
  global $user;
  $user_links = array();
  if (empty($user->uid)) {
    $user_links['login'] = array('title' => t('Login'), 'href' => 'user');
    // Do not display register link if registration is not allowed.
    if (variable_get('user_register', 1)) {
      $user_links['register'] = array('title' => t('Register'), 'href' => 'user/register');
    }
  }
  else {
    $user_links['account'] = array('title' => t('Hello @username', array('@username' => $user->name)), 'href' => 'user', 'html' => TRUE);
    $user_links['logout'] = array('title' => t('Logout'), 'href' => "logout");
  }
  return $user_links;
}

/**
 * Generate an icon class from a path.
 */
function _rubik_icon_classes($path) {
  $classes = array();
  $args = explode('/', $path);
  if ($args[0] === 'admin' || (count($args) > 1 && $args[0] === 'node' && $args[1] === 'add')) {
    while (count($args)) {
      $classes[] = 'path-'. str_replace('/', '-', implode('/', $args));
      array_pop($args);
    }
    return implode(' ', $classes);
  }
  return '';
}

/**
 * Recurses through forms for input filter fieldsets and alters them.
 */
function _rubik_filter_form_alter(&$form) {
  $found = FALSE;
  foreach (element_children($form) as $id) {
    // Filter form element found
    if (
      isset($form[$id]['#element_validate']) &&
      is_array($form[$id]['#element_validate']) &&
      in_array('filter_form_validate', $form[$id]['#element_validate'])
    ) {
      $form[$id]['#type'] = 'markup';
      $form[$id]['#theme'] = 'filter_form';
      $found = TRUE;
    }
    // Formatting guidelines element found
    elseif ($id == 'format' && !empty($form[$id]['format']['guidelines'])) {
      $form[$id]['#theme'] = 'filter_form';
      $found = TRUE;
    }
    // Recurse down other elements
    else {
      _rubik_filter_form_alter($form[$id]);
    }
  }
  // If filter elements found, adjust parent element.
  if ($found) {
    foreach (element_children($form) as $element) {
      $form[$element]['#rubik_filter_form'] = TRUE;
    }
    $form = array(
      '#type' => 'item',
      '#weight' => isset($form['#weight']) ? $form['#weight'] : 0,
      $form
    );
  }
}
