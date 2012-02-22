<?php
/**
 * @file
 * Contains theme override functions and process & preprocess functions for sasson
 */

// Render SASS files
if (theme_get_setting('sasson_sass')) {
  require_once dirname(__FILE__) . '/includes/sass.inc';
  require_once dirname(__FILE__) . '/includes/sass_settings.inc';
  sasson_sass_render();
}


// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('sasson_clear_registry')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}


/**
 * Implements hook_css_alter().
 *
 * This function checks all CSS files currently added via drupal_add_css() and
 * and checks to see if a direction-specific file should be included.
 */
function sasson_css_alter(&$css) {
  global $language;

  foreach ($css as $data => $item) {
    // Only provide overrides for files.
    if ($item['type'] == 'file') {
      $path_parts = pathinfo($item['data']);
      $extens = ".{$path_parts['extension']}";
      // If the current language is LTR, add the file with the LTR overrides.
      if ($language->direction == LANGUAGE_LTR) {
        $dir_path = str_replace($extens, "-ltr{$extens}", $item['data']);
      }
      // If the current language is RTL, add the sass/scss file with the RTL overrides.
      // Core already takes care of RTL css files.
      elseif ($language->direction == LANGUAGE_RTL && ($extens == ".scss" || $extens == ".sass")) {
        $dir_path = str_replace($extens, "-rtl{$extens}", $item['data']);
      }
      // If the file is exists, add the file with the dir (LTR/RTL) overrides.
      if (isset($dir_path) && file_exists($dir_path) && !isset($css[$dir_path])) {
        // Replicate the same item, but with the dir (RTL/LTR) path and a little larger
        // weight so that it appears directly after the original CSS file.
        $item['data'] = $dir_path;
        $item['weight'] += 0.01;
        $css[$dir_path] = $item;
      }
    }
  }
}


/**
 * Build the theme tree from base theme to active theme.
 */
function sasson_theme_dynasty() {
  global $theme_key;
  $themes = list_themes();
  $dynasty = array();
  $dynasty[] = $obj = $themes[$theme_key];
  
  while (isset($obj->base_theme) && isset($themes[$obj->base_theme]) && !empty($themes[$obj->base_theme])) {
    $dynasty[] = $obj = $themes[$obj->base_theme];
  }
  
  return $dynasty;
}


/**
 * Includes all custom style sheets for the current theme.
 */
function sasson_css_include() {

  $dynasty = sasson_theme_dynasty();
  
  foreach ($dynasty as $theme) {
    $info = drupal_parse_info_file($theme->filename); 
  
    if (isset($info['styles']) && !empty($info['styles'])) {
      foreach ($info['styles'] as $file => $style) {
        if (file_exists($file = drupal_get_path('theme', $theme->name) . "/{$file}")) {
          drupal_add_css($file, $style['options']);
        }   
      }
    }
  }
}


/**
 * Implements template_html_head_alter();
 *
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function sasson_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}


/**
 * Implements template_preprocess_html().
 */
function sasson_preprocess_html(&$vars) {
  
  $vars['doctype'] = _sasson_doctype();
  $vars['rdf'] = _sasson_rdf($vars);
  $vars['html_attributes'] = 'lang="' . $vars['language']->language . '" dir="' . $vars['language']->dir . '" ' . $vars['rdf']->version . $vars['rdf']->namespaces;

  // Custom fonts from Google web-fonts
  $font = str_replace(' ', '+', theme_get_setting('sasson_font'));
  if (theme_get_setting('sasson_font')) {
    drupal_add_css('http://fonts.googleapis.com/css?family=' . $font , array('type' => 'external', 'group' => CSS_THEME));
  }

  // Enable HTML5 elements in IE
  $vars['html5shiv'] = theme_get_setting('sasson_html5shiv') ? '<!--[if lt IE 9]><script src="'. base_path() . drupal_get_path('theme', 'sasson') .'/scripts/html5shiv.js"></script><![endif]-->' : '';

  // Force latest IE rendering engine (even in intranet) & Chrome Frame
  if (theme_get_setting('sasson_force_ie')) {
    $meta_force_ie = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'X-UA-Compatible',
        'content' =>  'IE=edge,chrome=1',
      )
    );  
    drupal_add_html_head($meta_force_ie, 'meta_force_ie');
  }

  // Prompt IE users to install Chrome Frame
  if (theme_get_setting('sasson_prompt_cf') != 'Disabled') {
    $vars['prompt_cf'] = "<!--[if lte " . theme_get_setting('sasson_prompt_cf') . " ]>
      <p class='chromeframe'>Your browser is <em>ancient!</em> <a href='http://browsehappy.com/'>Upgrade to a different browser</a> or <a href='http://www.google.com/chromeframe/?redirect=true'>install Google Chrome Frame</a> to experience this site.</p>
    <![endif]-->";
  } else {
    $vars['prompt_cf'] = '';
  }
  
  //  Mobile viewport optimized: h5bp.com/viewport
  if (theme_get_setting('sasson_responsive')) {
    $mobile_viewport = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'content' =>  'width=device-width',
        'name' => 'viewport',
      )
    );
    drupal_add_html_head($mobile_viewport, 'mobile_viewport');
  }

  // Load responsive menus if enabled in theme-settings
  if (theme_get_setting('sasson_responsive')) {
    $mobiledropdown_width = str_replace('px', '', theme_get_setting('sasson_responsive_menus_width'));
    if ($mobiledropdown_width > 0) {
      $mobiledropdown_selectors = theme_get_setting('sasson_responsive_menus_selectors');
      $inline_code = 'jQuery("' . $mobiledropdown_selectors . '").mobileSelect({
          deviceWidth: ' . $mobiledropdown_width . '
        });';
      drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/jquery.mobileselect.js');
      drupal_add_js($inline_code,
        array('type' => 'inline', 'scope' => 'footer')
      );
    }
  }
  
  // Since menu is rendered in preprocess_page we need to detect it here to add body classes
  $has_main_menu = theme_get_setting('toggle_main_menu');
  $has_secondary_menu = theme_get_setting('toggle_secondary_menu');

  /* Add extra classes to body for more flexible theming */

  if ($has_main_menu or $has_secondary_menu) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if ($has_secondary_menu) {
    $vars['classes_array'][] = 'with-subnav';
  }

  if (!empty($vars['page']['featured'])) {
    $vars['classes_array'][] = 'featured';
  }

  if ($vars['is_admin']) {
    $vars['classes_array'][] = 'admin';
  }
  
  if (theme_get_setting('sasson_show_grid')) {
    $vars['classes_array'][] = 'show-grid';
  }
  
  if (theme_get_setting('sasson_overlay') && theme_get_setting('sasson_overlay_url')) {
    $vars['classes_array'][] = 'show-overlay';
    drupal_add_library('system', 'ui');
    drupal_add_library('system', 'ui.widget');
    drupal_add_library('system', 'ui.mouse');
    drupal_add_library('system', 'ui.draggable');
    drupal_add_js(array('sasson' => array(
      'overlay_url' => theme_get_setting('sasson_overlay_url'),
      'overlay_opacity' => theme_get_setting('sasson_overlay_opacity'),
    )), 'setting');
  }
  
  $vars['classes_array'][] = 'dir-' . $vars['language']->dir;
  
  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $vars['classes_array'][] = drupal_html_id('page-' . $page_name);
    }

    $vars['classes_array'][] = drupal_html_id('section-' . $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-add'; // Add 'section-node-add'
      } elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-' . arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

  sasson_css_include();

}


/**
 * Implements template_preprocess_page().
 */
function sasson_preprocess_page(&$vars) {
  
  if (isset($vars['node_title'])) {
    $vars['title'] = $vars['node_title'];
  }
  
  // Site navigation links.
  $vars['main_menu_links'] = '';
  if (isset($vars['main_menu'])) {
    $vars['main_menu_links'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('inline', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
  $vars['secondary_menu_links'] = '';
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu_links'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'id'    => 'secondary-menu-links',
        'class' => array('inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($vars['title_suffix']['add_or_remove_shortcut']) && $vars['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $vars['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $vars['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $vars['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
  
  if(!theme_get_setting('sasson_feed_icons')) {
    $vars['feed_icons'] = '';
  }
}


/**
 * Implements template_preprocess_node().
 *
 * Adds extra classes to node container for advanced theming
 */
function sasson_preprocess_node(&$vars) {
  // Striping class
  $vars['classes_array'][] = 'node-' . $vars['zebra'];

  // Node is published
  $vars['classes_array'][] = ($vars['status']) ? 'published' : 'unpublished';

  // Node has comments?
  $vars['classes_array'][] = ($vars['comment']) ? 'with-comments' : 'no-comments';

  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky'; // Node is sticky
  }

  if ($vars['promote']) {
    $vars['classes_array'][] = 'promote'; // Node is promoted to front page
  }

  if ($vars['teaser']) {
    $vars['classes_array'][] = 'node-teaser'; // Node is displayed as teaser.
  }

  if ($vars['uid'] && $vars['uid'] === $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  
  $vars['submitted'] = t('Submitted by !username on ', array('!username' => $vars['name']));
  $vars['submitted_date'] = t('!datetime', array('!datetime' => $vars['date']));
  $vars['submitted_pubdate'] = format_date($vars['created'], 'custom', 'Y-m-d\TH:i:s');
  
  if ($vars['view_mode'] == 'full' && node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'node-full';
  }
}


/**
 * Implements template_preprocess_block().
 */
function sasson_preprocess_block(&$vars, $hook) {
  // Add a striping class.
  $vars['classes_array'][] = 'block-' . $vars['zebra'];

  // In the header region visually hide block titles.
  if ($vars['block']->region == 'header') {
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
}


/**
 * Implements template_proprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function sasson_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}


/**
 * Implements theme_menu_tree().
 */
function sasson_menu_tree($vars) {
  return '<ul class="menu clearfix">' . $vars['tree'] . '</ul>';
}


/**
 * Implements theme_field__field_type().
 */
function sasson_field__taxonomy_term_reference($vars) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<h3 class="field-label">' . $vars['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ( $vars['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($vars['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $vars['classes'] . (!in_array('clearfix', $vars['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}


/**
 *  Return a themed breadcrumb trail
 */
function sasson_breadcrumb($vars) {
  
  $breadcrumb = isset($vars['breadcrumb']) ? $vars['breadcrumb'] : array();
  
  if (theme_get_setting('sasson_breadcrumb_hideonlyfront')) {
    $condition = count($breadcrumb) > 1;
  } else {
    $condition = !empty($breadcrumb);
  }
  
  if(theme_get_setting('sasson_breadcrumb_showtitle')) {
    $title = drupal_get_title();
    if(!empty($title)) {
      $condition = true;
      $breadcrumb[] = $title;
    }
  }
  
  $separator = theme_get_setting('sasson_breadcrumb_separator');

  if (!$separator) {
    $separator = '»';
  }
  
  if ($condition) {
    return implode(" {$separator} ", $breadcrumb);
  }
}


/**
 * Generate doctype for templates
 */
function _sasson_doctype() {
  return (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"' . "\n" . '"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">' : '<!DOCTYPE html>' . "\n";
}


/**
 * Generate RDF object for templates
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 *
 * @param array $vars
 */
function _sasson_rdf($vars) {
  $rdf = new stdClass();

  if (module_exists('rdf')) {
    $rdf->version = 'version="HTML+RDFa 1.1"';
    $rdf->namespaces = $vars['rdf_namespaces'];
    $rdf->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $rdf->version = '';
    $rdf->namespaces = '';
    $rdf->profile = '';
  }

  return $rdf;
}


/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function sasson_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant)
  $element['#attributes']['class'][] = drupal_html_id($element['#title']);
  // Adding a class depending on the ID of the link (constant)
  $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 * Override or insert variables into theme_menu_local_task().
 */
function sasson_preprocess_menu_local_task(&$vars) {
  $link = & $vars['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}


/**
 *  Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function sasson_menu_local_tasks(&$vars) {
  $output = '';

  if (!empty($vars['primary'])) {
    $vars['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $vars['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $vars['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['primary']);
  }

  if (!empty($vars['secondary'])) {
    $vars['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $vars['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $vars['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['secondary']);
  }

  return $output;
}
