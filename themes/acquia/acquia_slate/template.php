<?php
// $Id: template.php,v 1.1.4.4 2009/06/13 10:51:09 jwolf Exp $

include_once('theme-settings.php');

/**
 * Initialize theme settings if needed
 */
acquia_slate_initialize_theme_settings('acquia_slate');


/**
 * Modify theme variables
 */
function acquia_slate_preprocess(&$vars) {
  global $user;                                            // Get the current user
  $vars['is_admin'] = in_array('admin', $user->roles);     // Check for Admin, logged in
  $vars['logged_in'] = ($user->uid > 0) ? TRUE : FALSE;
}


function acquia_slate_preprocess_page(&$vars) {
  global $language;
  // Remove sidebars if disabled e.g., for Panels
  if (!$vars['show_blocks']) {
    $vars['sidebar_first'] = '';
    $vars['sidebar_last'] = '';
  }
  // Build array of helpful body classes
  $body_classes = array();
  $body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';                                 // Page user is logged in
  $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front';                                          // Page is front page
  if (isset($vars['node'])) {
    $body_classes[] = ($vars['node']) ? 'full-node' : '';                                                   // Page is one full node
    $body_classes[] = (($vars['node']->type == 'forum') || (arg(0) == 'forum')) ? 'forum' : '';             // Page is Forum page
    $body_classes[] = ($vars['node']->type) ? 'node-type-'. $vars['node']->type : '';                       // Page has node-type-x, e.g., node-type-page
  }
  else {
    $body_classes[] = (arg(0) == 'forum') ? 'forum' : '';                                                   // Page is Forum page
  }
  $body_classes[] = (module_exists('panels_page') && (panels_page_get_current())) ? 'panels' : '';        // Page is Panels page
  $body_classes[] = 'layout-'. (($vars['sidebar_first'] || $vars['secondary_links']) ? 'first-main' : 'main') . (($vars['sidebar_last']) ? '-last' : '');  // Page sidebars are active
  if (!(empty($vars['preface_first']) && empty($vars['preface_middle']) && empty($vars['preface_last']))) { // Preface regions are active
    $preface_regions = 'preface';
    $preface_regions .= ($vars['preface_first']) ? '-first' : '';
    $preface_regions .= ($vars['preface_middle']) ? '-middle' : '';
    $preface_regions .= ($vars['preface_last']) ? '-last' : '';
    $body_classes[] = $preface_regions;
  }
  if ($vars['postscript_first'] || $vars['postscript_middle'] || $vars['postscript_last']) {              // Postscript regions are active
    $postscript_regions = 'postscript';
    $postscript_regions .= ($vars['postscript_first']) ? '-first' : '';
    $postscript_regions .= ($vars['postscript_middle']) ? '-middle' : '';
    $postscript_regions .= ($vars['postscript_last']) ? '-last' : '';
    $body_classes[] = $postscript_regions;
  }
  $body_classes = array_filter($body_classes);                                                            // Remove empty elements
  $vars['body_classes'] = implode(' ', $body_classes);                                                    // Create class list separated by spaces

  // Add preface & postscript classes with number of active sub-regions
  $region_list = array(
    'prefaces' => array('preface_first', 'preface_middle', 'preface_last'), 
    'postscripts' => array('postscript_first', 'postscript_middle', 'postscript_last')
  );
  foreach ($region_list as $sub_region_key => $sub_region_list) {
    $active_regions = array();
    foreach ($sub_region_list as $region_item) {
      if (!empty($vars[$region_item])) {
        $active_regions[] = $region_item;
      }
    }
    $vars[$sub_region_key] = $sub_region_key .'-'. strval(count($active_regions));
  }
  
  // Generate menu tree from source of primary links
  $vars['primary_links_tree'] = menu_tree(variable_get('menu_primary_links_source', 'primary-links'));

  // TNT THEME SETTINGS SECTION

  // Hide breadcrumb on all pages
  if (theme_get_setting('breadcrumb_display') == 0) {
    $vars['breadcrumb'] = '';  
  }
  
  // Set site title, slogan, mission, page title & separator
  if (!module_exists('page_title')) {
    $title = t(variable_get('site_name', ''));
    $slogan = t(variable_get('site_slogan', ''));
    $mission = t(variable_get('site_mission', ''));
    $page_title = t(drupal_get_title());
    $title_separator = theme_get_setting('configurable_separator');
    if (drupal_is_front_page()) {                                                // Front page title settings
      switch (theme_get_setting('front_page_title_display')) {
        case 'title_slogan':
          $vars['head_title'] = drupal_set_title($title . $title_separator . $slogan);
          break;
        case 'slogan_title':
          $vars['head_title'] = drupal_set_title($slogan . $title_separator . $title);
          break;
        case 'title_mission':
          $vars['head_title'] = drupal_set_title($title . $title_separator . $mission);
          break;
        case 'custom':
          if (theme_get_setting('page_title_display_custom') !== '') {
            $vars['head_title'] = drupal_set_title(t(theme_get_setting('page_title_display_custom')));
          }
      }
    }
    else {                                                                       // Non-front page title settings
      switch (theme_get_setting('other_page_title_display')) {
        case 'ptitle_slogan':
          $vars['head_title'] = drupal_set_title($page_title . $title_separator . $slogan);
          break;
        case 'ptitle_stitle':
          $vars['head_title'] = drupal_set_title($page_title . $title_separator . $title);
          break;
        case 'ptitle_smission':
          $vars['head_title'] = drupal_set_title($page_title . $title_separator . $mission);
          break;
        case 'ptitle_custom':
          if (theme_get_setting('other_page_title_display_custom') !== '') {
            $vars['head_title'] = drupal_set_title($page_title . $title_separator . t(theme_get_setting('other_page_title_display_custom')));
          }
          break;
        case 'custom':
          if (theme_get_setting('other_page_title_display_custom') !== '') {
            $vars['head_title'] = drupal_set_title(t(theme_get_setting('other_page_title_display_custom')));
          }
      }
    }
    $vars['head_title'] = strip_tags($vars['head_title']);                                        // Remove any potential html tags
  }
  
  // Set meta keywords and description (unless using Meta tags module)
  if (!module_exists('nodewords')) {
    if (theme_get_setting('meta_keywords') !== '') {
      $keywords = '<meta name="keywords" content="'. theme_get_setting('meta_keywords') .'" />';
      $vars['head'] .= $keywords ."\n";
    } 
    if (theme_get_setting('meta_description') !== '') {
      $keywords = '<meta name="description" content="'. theme_get_setting('meta_description') .'" />';
      $vars['head'] .= $keywords ."\n";
    } 
  }

  // Add custom theme settings
  $theme_settings_path = path_to_theme() . '/theme_settings/';
  drupal_add_css($theme_settings_path . theme_get_setting('theme_width') . '.css', 'theme');
  drupal_add_css($theme_settings_path . theme_get_setting('theme_color') . '.css', 'theme');
  drupal_add_css($theme_settings_path . theme_get_setting('theme_fonts') . '.css', 'theme');
  $banner_file = theme_get_setting('theme_banner');
  $vars['banner_image'] = ($banner_file == 'none') ? '' : 'style="background: url('. base_path() . $theme_settings_path .'banners/'. $banner_file .') no-repeat;"';

  // Set IE6 & IE7 stylesheets, plus right-to-left versions
  $theme_path = base_path() . path_to_theme();
  $vars['ie6_styles'] = '<link type="text/css" rel="stylesheet" media="all" href="' . $theme_path . '/ie6-fixes.css" />' . "\n";
  $vars['ie7_styles'] = '<link type="text/css" rel="stylesheet" media="all" href="' . $theme_path . '/ie7-fixes.css" />' . "\n";
  if (defined('LANGUAGE_RTL') && $language->direction == LANGUAGE_RTL) {
    $vars['ie6_styles'] .= '    <link type="text/css" rel="stylesheet" media="all" href="' . $theme_path . '/ie6-fixes-rtl.css" />' . "\n";
    $vars['ie7_styles'] .= '    <link type="text/css" rel="stylesheet" media="all" href="' . $theme_path . '/ie7-fixes-rtl.css" />' . "\n";
  }

  if (file_exists(path_to_theme() . '/local.css')) {                    // Add local css file if present
    $theme_path = base_path() . path_to_theme() . '/local.css';
    $vars['local_styles'] = '<link type="text/css" rel="stylesheet" media="all" href="' . $theme_path . '" />' . "\n";
  }
  
  // Use grouped import technique for more than 30 un-aggregated stylesheets (css limit fix for IE)
  $css = drupal_add_css();
  if (theme_get_setting('fix_css_limit') && !variable_get('preprocess_css', FALSE) && acquia_slate_css_count($css) > 26) {
    $styles = '';
    $suffix = "\n".'</style>'."\n";
    foreach ($css as $media => $types) {
      $prefix = '<style type="text/css" media="'. $media .'">'."\n";
      $imports = array();
      foreach ($types as $files) {
        foreach ($files as $file => $preprocess) {
          $imports[] = '@import "'. base_path() . $file .'";';
          if (count($imports) == 30) {
            $styles .= $prefix . implode("\n", $imports) . $suffix;
            $imports = array();
          }
        }
      }
      $styles .= (count($imports) > 0) ? ($prefix . implode("\n", $imports) . $suffix) : '';
    }
    $vars['styles'] = $styles;
  }
  else {
    $vars['styles'] = drupal_get_css();                                   // Use normal link technique
  }
  if (drupal_is_front_page()) {
    $vars['closure'] .= '<div id="legal-notice">Theme provided by <a href="http://www.acquia.com">Acquia, Inc.</a> under GPL license from TopNotchThemes <a href="http://www.topnotchthemes.com">Drupal themes</a></div>';
  }
}


function acquia_slate_preprocess_block(&$vars) {
  // Add regions with rounded blocks (e.g., sidebar_first, sidebar_last) to $rounded_regions array
  $rounded_regions = array('sidebar_first');
  $vars['rounded_block'] = (in_array($vars['block']->region, $rounded_regions)) ? TRUE : FALSE;
}


function acquia_slate_preprocess_node(&$vars) {
  // Build array of handy node classes
  $node_classes = array();
  $node_classes[] = $vars['zebra'];                                      // Node is odd or even
  $node_classes[] = (!$vars['node']->status) ? 'node-unpublished' : '';  // Node is unpublished
  $node_classes[] = ($vars['sticky']) ? 'sticky' : '';                   // Node is sticky
  $node_classes[] = (isset($vars['node']->teaser)) ? 'teaser' : 'full-node';    // Node is teaser or full-node
  $node_classes[] = 'node-type-'. $vars['node']->type;                   // Node is type-x, e.g., node-type-page
  $node_classes = array_filter($node_classes);                           // Remove empty elements
  $vars['node_classes'] = implode(' ', $node_classes);                   // Implode class list with spaces
  
  // Add node_bottom region content
  $vars['node_bottom'] = theme('blocks', 'node_bottom');

  // Node Theme Settings
  
  // Date & author
  if (!module_exists('submitted_by')) {
    $date = t('Posted ') . format_date($vars['node']->created, 'medium');                 // Format date as small, medium, or large
    $author = theme('username', $vars['node']);
    $author_only_separator = t('Posted by ');
    $author_date_separator = t(' by ');
    $submitted_by_content_type = (theme_get_setting('submitted_by_enable_content_type') == 1) ? $vars['node']->type : 'default';
    $date_setting = (theme_get_setting('submitted_by_date_'. $submitted_by_content_type) == 1);
    $author_setting = (theme_get_setting('submitted_by_author_'. $submitted_by_content_type) == 1);
    $author_separator = ($date_setting) ? $author_date_separator : $author_only_separator;
    $date_author = ($date_setting) ? $date : '';
    $date_author .= ($author_setting) ? $author_separator . $author : '';
    $vars['submitted'] = $date_author;
  }

  // Taxonomy
  $taxonomy_content_type = (theme_get_setting('taxonomy_enable_content_type') == 1) ? $vars['node']->type : 'default';
  $taxonomy_display = theme_get_setting('taxonomy_display_'. $taxonomy_content_type);
  $taxonomy_format = theme_get_setting('taxonomy_format_'. $taxonomy_content_type);
  if ((module_exists('taxonomy')) && ($taxonomy_display == 'all' || ($taxonomy_display == 'only' && $vars['page']))) {
    $vocabularies = taxonomy_get_vocabularies($vars['node']->type);
    $output = '';
    $term_delimiter = ', ';
    foreach ($vocabularies as $vocabulary) {
      if (theme_get_setting('taxonomy_vocab_hide_'. $taxonomy_content_type .'_'. $vocabulary->vid) != 1) {
        $terms = taxonomy_node_get_terms_by_vocabulary($vars['node'], $vocabulary->vid);
        if ($terms) {
          $term_items = '';
          foreach ($terms as $term) {                        // Build vocabulary term items
            $term_link = l($term->name, taxonomy_term_path($term), array('attributes' => array('rel' => 'tag', 'title' => strip_tags($term->description))));
            $term_items .= '<li class="vocab-term">'. $term_link . $term_delimiter .'</li>';
          }
          if ($taxonomy_format == 'vocab') {                 // Add vocabulary labels if separate
            $output .= '<li class="vocab vocab-'. $vocabulary->vid .'"><span class="vocab-name">'. $vocabulary->name .':</span> <ul class="vocab-list">';
            $output .= substr_replace($term_items, '</li>', -(strlen($term_delimiter) + 5)) .'</ul></li>';
          }
          else {
            $output .= $term_items;
          }
        }
      }
    }
    if ($output != '') {
      $output = ($taxonomy_format == 'list') ? substr_replace($output, '</li>', -(strlen($term_delimiter) + 5)) : $output;
      $output = '<ul class="taxonomy">'. $output .'</ul>';
    }
    $vars['terms'] = $output;
  }
  else {
    $vars['terms'] = '';
  }
  
  // Node Links
  if (isset($vars['node']->links['node_read_more'])) {
    $node_content_type = (theme_get_setting('readmore_enable_content_type') == 1) ? $vars['node']->type : 'default';
    $vars['node']->links['node_read_more'] = array(
      'title' => acquia_slate_themesettings_link(
        theme_get_setting('readmore_prefix_'. $node_content_type),
        theme_get_setting('readmore_suffix_'. $node_content_type),
        t(theme_get_setting('readmore_'. $node_content_type)),
        'node/'. $vars['node']->nid,
        array(
          'attributes' => array('title' => t(theme_get_setting('readmore_title_'. $node_content_type))), 
          'query' => NULL, 'fragment' => NULL, 'absolute' => FALSE, 'html' => TRUE
        )
      ),
      'attributes' => array('class' => 'readmore-item'),
      'html' => TRUE,
    );
  }
  if (isset($vars['node']->links['comment_add'])) {
    $node_content_type = (theme_get_setting('comment_enable_content_type') == 1) ? $vars['node']->type : 'default';
    if ($vars['teaser']) {
      $vars['node']->links['comment_add'] = array(
        'title' => acquia_slate_themesettings_link(
          theme_get_setting('comment_add_prefix_'. $node_content_type),
          theme_get_setting('comment_add_suffix_'. $node_content_type),
          t(theme_get_setting('comment_add_'. $node_content_type)),
          "comment/reply/".$vars['node']->nid,
          array(
            'attributes' => array('title' => t(theme_get_setting('comment_add_title_'. $node_content_type))), 
            'query' => NULL, 'fragment' => 'comment-form', 'absolute' => FALSE, 'html' => TRUE
          )
        ),
        'attributes' => array('class' => 'comment-add-item'),
        'html' => TRUE,
      );
    }
    else {
      $vars['node']->links['comment_add'] = array(
        'title' => acquia_slate_themesettings_link(
          theme_get_setting('comment_node_prefix_'. $node_content_type),
          theme_get_setting('comment_node_suffix_'. $node_content_type),
          t(theme_get_setting('comment_node_'. $node_content_type)),
          "comment/reply/".$vars['node']->nid,
          array(
            'attributes' => array('title' => t(theme_get_setting('comment_node_title_'. $node_content_type))), 
            'query' => NULL, 'fragment' => 'comment-form', 'absolute' => FALSE, 'html' => TRUE
          )
        ),
        'attributes' => array('class' => 'comment-node-item'),
        'html' => TRUE,
      );
    }
  }
  if (isset($vars['node']->links['comment_new_comments'])) {
    $node_content_type = (theme_get_setting('comment_enable_content_type') == 1) ? $vars['node']->type : 'default';
    $vars['node']->links['comment_new_comments'] = array(
      'title' => acquia_slate_themesettings_link(
        theme_get_setting('comment_new_prefix_'. $node_content_type),
        theme_get_setting('comment_new_suffix_'. $node_content_type),
        format_plural(
          comment_num_new($vars['node']->nid),
          t(theme_get_setting('comment_new_singular_'. $node_content_type)),
          t(theme_get_setting('comment_new_plural_'. $node_content_type))
        ),
        "node/".$vars['node']->nid,
        array(
          'attributes' => array('title' => t(theme_get_setting('comment_new_title_'. $node_content_type))), 
          'query' => NULL, 'fragment' => 'new', 'absolute' => FALSE, 'html' => TRUE
        )
      ),
      'attributes' => array('class' => 'comment-new-item'),
      'html' => TRUE,
    );
  }
  if (isset($vars['node']->links['comment_comments'])) {
    $node_content_type = (theme_get_setting('comment_enable_content_type') == 1) ? $vars['node']->type : 'default';
    $vars['node']->links['comment_comments'] = array(
      'title' => acquia_slate_themesettings_link(
        theme_get_setting('comment_prefix_'. $node_content_type),
        theme_get_setting('comment_suffix_'. $node_content_type),
        format_plural(
          comment_num_all($vars['node']->nid),
          t(theme_get_setting('comment_singular_'. $node_content_type)),
          t(theme_get_setting('comment_plural_'. $node_content_type))
        ),
        "node/".$vars['node']->nid,
        array(
          'attributes' => array('title' => t(theme_get_setting('comment_title_'. $node_content_type))), 
          'query' => NULL, 'fragment' => 'comments', 'absolute' => FALSE, 'html' => TRUE
        )
      ),
      'attributes' => array('class' => 'comment-item'),
      'html' => TRUE,
    );
  }
  $vars['links'] = theme('links', $vars['node']->links, array('class' => 'links inline')); 
}


function acquia_slate_preprocess_comment(&$vars) {
  global $user;
  // Build array of handy comment classes
  $comment_classes = array();
  static $comment_odd = TRUE;                                                                             // Comment is odd or even
  $comment_classes[] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;
  $comment_classes[] = ($vars['comment']->status == COMMENT_NOT_PUBLISHED) ? 'comment-unpublished' : '';  // Comment is unpublished
  $comment_classes[] = ($vars['comment']->new) ? 'comment-new' : '';                                      // Comment is new
  $comment_classes[] = ($vars['comment']->uid == 0) ? 'comment-by-anon' : '';                             // Comment is by anonymous user
  $comment_classes[] = ($user->uid && $vars['comment']->uid == $user->uid) ? 'comment-mine' : '';         // Comment is by current user
  $node = node_load($vars['comment']->nid);                                                               // Comment is by node author
  $vars['author_comment'] = ($vars['comment']->uid == $node->uid) ? TRUE : FALSE;
  $comment_classes[] = ($vars['author_comment']) ? 'comment-by-author' : '';
  $comment_classes = array_filter($comment_classes);                                                      // Remove empty elements
  $vars['comment_classes'] = implode(' ', $comment_classes);                                              // Create class list separated by spaces
  // Date & author
  $submitted_by = t('by ') .'<span class="comment-name">'.  theme('username', $vars['comment']) .'</span>';
  $submitted_by .= t(' - ') .'<span class="comment-date">'.  format_date($vars['comment']->timestamp, 'small') .'</span>';     // Format date as small, medium, or large
  $vars['submitted'] = $submitted_by;
}


/**
 * Set defaults for comments display
 * (Requires comment-wrapper.tpl.php file in theme directory)
 */
function acquia_slate_preprocess_comment_wrapper(&$vars) {
  $vars['display_mode']  = COMMENT_MODE_FLAT_EXPANDED;
  $vars['display_order'] = COMMENT_ORDER_OLDEST_FIRST;
  $vars['comment_controls_state'] = COMMENT_CONTROLS_HIDDEN;
}


/**
 * Adds a class for the style of view  
 * (e.g., node, teaser, list, table, etc.)
 * (Requires views-view.tpl.php file in theme directory)
 */
function acquia_slate_preprocess_views_view(&$vars) {
  $vars['css_name'] = $vars['css_name'] .' view-style-'. views_css_safe(strtolower($vars['view']->type));
}


/**
 * Modify search results based on theme settings
 */
function acquia_slate_preprocess_search_result(&$variables) {
  static $search_zebra = 'even';
  $search_zebra = ($search_zebra == 'even') ? 'odd' : 'even';
  $variables['search_zebra'] = $search_zebra;
  
  $result = $variables['result'];
  $variables['url'] = check_url($result['link']);
  $variables['title'] = check_plain($result['title']);

  // Check for existence. User search does not include snippets.
  $variables['snippet'] = '';
  if (isset($result['snippet']) && theme_get_setting('search_snippet')) {
    $variables['snippet'] = $result['snippet'];
  }
  
  $info = array();
  if (!empty($result['type']) && theme_get_setting('search_info_type')) {
    $info['type'] = check_plain($result['type']);
  }
  if (!empty($result['user']) && theme_get_setting('search_info_user')) {
    $info['user'] = $result['user'];
  }
  if (!empty($result['date']) && theme_get_setting('search_info_date')) {
    $info['date'] = format_date($result['date'], 'small');
  }
  if (isset($result['extra']) && is_array($result['extra'])) {
    // $info = array_merge($info, $result['extra']);  Drupal bug?  [extra] array not keyed with 'comment' & 'upload'
    if (!empty($result['extra'][0]) && theme_get_setting('search_info_comment')) {
      $info['comment'] = $result['extra'][0];
    }
    if (!empty($result['extra'][1]) && theme_get_setting('search_info_upload')) {
      $info['upload'] = $result['extra'][1];
    }
  }

  // Provide separated and grouped meta information.
  $variables['info_split'] = $info;
  $variables['info'] = implode(' - ', $info);

  // Provide alternate search result template.
  $variables['template_files'][] = 'search-result-'. $variables['type'];
}


/**
 * Hide or show username '(not verified)' text
 */
function acquia_slate_username($object) {
  if ((!$object->uid) && $object->name) {
    $output = (!empty($object->homepage)) ? l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow'))) : check_plain($object->name);
    $output .= (theme_get_setting('user_notverified_display') == 1) ? ' ('. t('not verified') .')' : '';
  }
  else {
    $output = theme_username($object);
  }  
  return $output;
}


/**
 * Set form file input max char size 
 */
function acquia_slate_file($element) {
  $element['#size'] = ($element['#size'] > 40) ? 40 : $element['#size'];
  return theme_file($element);
}


/**
 * Limit string length in word increments, add ellipsis
 */
function acquia_slate_wordlimit($string, $length = 50, $ellipsis = "...") {
  $words = explode(' ', strip_tags($string));
  if (count($words) > $length)
    return implode(' ', array_slice($words, 0, $length)) . $ellipsis;
  else
    return $string;
}


/**
 * Count the total number of CSS files in $vars['css']
 */
function acquia_slate_css_count($array) {
  $count = 0;
  foreach ($array as $item) {
    $count = (is_array($item)) ? $count + acquia_slate_css_count($item) : $count + 1;
  }
  return $count;
}


/**
 * Creates a link with prefix and suffix text
 *
 * @param $prefix
 *   The text to prefix the link.
 * @param $suffix
 *   The text to suffix the link.
 * @param $text
 *   The text to be enclosed with the anchor tag.
 * @param $path
 *   The Drupal path being linked to, such as "admin/content/node". Can be an external
 *   or internal URL.
 *     - If you provide the full URL, it will be considered an
 *   external URL.
 *     - If you provide only the path (e.g. "admin/content/node"), it is considered an
 *   internal link. In this case, it must be a system URL as the url() function
 *   will generate the alias.
 * @param $options
 *   An associative array that contains the following other arrays and values
 *     @param $attributes
 *       An associative array of HTML attributes to apply to the anchor tag.
 *     @param $query
 *       A query string to append to the link.
 *     @param $fragment
 *       A fragment identifier (named anchor) to append to the link.
 *     @param $absolute
 *       Whether to force the output to be an absolute link (beginning with http:).
 *       Useful for links that will be displayed outside the site, such as in an RSS
 *       feed.
 *     @param $html
 *       Whether the title is HTML or not (plain text)
 * @return
 *   an HTML string containing a link to the given path.
 */
function acquia_slate_themesettings_link($prefix, $suffix, $text, $path, $options) {
  return $prefix . (($text) ? l($text, $path, $options) : '') . $suffix;
}


// Override theme_button for expanding graphic buttons
function acquia_slate_button($element) {
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'] .' '. $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'];
  }

  // Wrap non-hidden input elements with span tags for button graphics
  if (isset($element['#attributes']['style']) && (stristr($element['#attributes']['style'], 'display: none;') || stristr($element['#attributes']['class'], 'fivestar-submit'))) {
    return '<input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ')  .'id="'. $element['#id'].'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." />\n";
  }
  else {
    return '<span class="button-wrapper"><span class="button"><span><input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ')  .'id="'. $element['#id'].'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']) ." /></span></span></span>\n";
  }
}