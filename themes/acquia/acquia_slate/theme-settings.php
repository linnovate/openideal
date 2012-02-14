<?php
// $Id: theme-settings.php,v 1.1.4.3 2009/06/13 10:51:09 jwolf Exp $


/**
 * Theme setting defaults
 */
function acquia_slate_default_theme_settings() {
  $defaults = array(
    'mission_statement_pages'               => 'home',
    'breadcrumb_display'                    => 0,
    'user_notverified_display'              => 1,
    'search_snippet'                        => 1,
    'search_info_type'                      => 1,
    'search_info_user'                      => 1,
    'search_info_date'                      => 1,
    'search_info_comment'                   => 1,
    'search_info_upload'                    => 1,
    'submitted_by_author_default'           => 1,
    'submitted_by_date_default'             => 1,
    'submitted_by_enable_content_type'      => 0,
    'taxonomy_display_default'              => 'only',
    'taxonomy_format_default'               => 'vocab',
    'taxonomy_enable_content_type'          => 0,
    'readmore_default'                      => t('Read more'),
    'readmore_title_default'                => t('Read the rest of this posting.'),
    'readmore_prefix_default'               => '',
    'readmore_suffix_default'               => '',
    'readmore_enable_content_type'          => 0,
    'comment_node_default'                  => t('Add new comment'),
    'comment_node_title_default'            => t('Share your thoughts and opinions related to this posting.'),
    'comment_node_prefix_default'           => '',
    'comment_node_suffix_default'           => '',
    'comment_add_default'                   => t('Add new comment'),
    'comment_add_title_default'             => t('Add a new comment to this page.'),
    'comment_add_prefix_default'            => '',
    'comment_add_suffix_default'            => '',
    'comment_singular_default'              => t('1 comment'),
    'comment_plural_default'                => t('@count comments'),
    'comment_title_default'                 => t('Jump to the first comment of this posting.'),
    'comment_prefix_default'                => '',
    'comment_suffix_default'                => '',
    'comment_new_singular_default'          => t('1 new comment'),
    'comment_new_plural_default'            => t('@count new comments'),
    'comment_new_title_default'             => t('Jump to the first new comment of this posting.'),
    'comment_new_prefix_default'            => '',
    'comment_new_suffix_default'            => '',
    'comment_enable_content_type'           => 0,
    'front_page_title_display'              => 'title_slogan',
    'page_title_display_custom'             => '',
    'other_page_title_display'              => 'ptitle_slogan',
    'other_page_title_display_custom'       => '',
    'configurable_separator'                => ' | ',
    'meta_keywords'                         => '',
    'meta_description'                      => '',
    'rebuild_registry'                      => 0,
    'fix_css_limit'                         => 0,
  );
  
  // Add site-wide theme settings
  $defaults = array_merge($defaults, theme_get_settings());
  
  // Set initial content-type-specific settings to defaults
  $node_types = node_get_types('names');
  foreach ($node_types as $type => $name) {
    $defaults["submitted_by_author_{$type}"]      = $defaults['submitted_by_author_default'];
    $defaults["submitted_by_date_{$type}"]        = $defaults['submitted_by_date_default'];
    $defaults["taxonomy_display_{$type}"]         = $defaults['taxonomy_display_default'];
    $defaults["taxonomy_format_{$type}"]          = $defaults['taxonomy_format_default'];
    $defaults["readmore_{$type}"]                 = $defaults['readmore_default'];
    $defaults["readmore_title_{$type}"]           = $defaults['readmore_title_default'];
    $defaults["readmore_prefix_{$type}"]          = $defaults['readmore_prefix_default'];
    $defaults["readmore_suffix_{$type}"]          = $defaults['readmore_suffix_default'];
    $defaults["comment_node_{$type}"]             = $defaults['comment_node_default'];
    $defaults["comment_node_title_{$type}"]       = $defaults['comment_node_title_default'];
    $defaults["comment_node_prefix_{$type}"]      = $defaults['comment_node_prefix_default'];
    $defaults["comment_node_suffix_{$type}"]      = $defaults['comment_node_suffix_default'];
    $defaults["comment_add_{$type}"]              = $defaults['comment_add_default'];
    $defaults["comment_add_title_{$type}"]        = $defaults['comment_add_title_default'];
    $defaults["comment_add_prefix_{$type}"]       = $defaults['comment_add_prefix_default'];
    $defaults["comment_add_suffix_{$type}"]       = $defaults['comment_add_suffix_default'];
    $defaults["comment_singular_{$type}"]         = $defaults['comment_singular_default'];
    $defaults["comment_plural_{$type}"]           = $defaults['comment_plural_default'];
    $defaults["comment_title_{$type}"]            = $defaults['comment_title_default'];
    $defaults["comment_prefix_{$type}"]           = $defaults['comment_prefix_default'];
    $defaults["comment_suffix_{$type}"]           = $defaults['comment_suffix_default'];
    $defaults["comment_new_singular_{$type}"]     = $defaults['comment_new_singular_default'];
    $defaults["comment_new_plural_{$type}"]       = $defaults['comment_new_plural_default'];
    $defaults["comment_new_title_{$type}"]        = $defaults['comment_new_title_default'];
    $defaults["comment_new_prefix_{$type}"]       = $defaults['comment_new_prefix_default'];
    $defaults["comment_new_suffix_{$type}"]       = $defaults['comment_new_suffix_default'];
  }
    
  // Add custom theme setting defaults if present
  $defaults['theme_width'] = 'fixed';
  $defaults['theme_fonts'] = 'fonts_1';
  $defaults['theme_banner'] = 'none';
  
  return $defaults;
}


/**
 * Theme setting initialization
 * if updated, unsaved, or registry rebuild mode
 */
function acquia_slate_initialize_theme_settings($theme_name) {
  $theme_settings = theme_get_settings($theme_name);
  if (is_null($theme_settings['fix_css_limit']) || $theme_settings['rebuild_registry'] == 1) {
    // Rebuild theme registry & notify user
    if($theme_settings['rebuild_registry'] == 1) {
      drupal_rebuild_theme_registry();
      drupal_set_message(t('Theme registry rebuild completed. <a href="!link">Turn off</a> this feature for production websites.', array('!link' => url('admin/build/themes/settings/' . $GLOBALS['theme']))), 'warning');
    }
  
    // Retrieve saved or site-wide theme settings
    $theme_setting_name = str_replace('/', '_', 'theme_'. $theme_name .'_settings');
    $settings = (variable_get($theme_setting_name, FALSE)) ? theme_get_settings($theme_name) : theme_get_settings();
  
    // Skip toggle_node_info_ settings
    if (module_exists('node')) {
      foreach (node_get_types() as $type => $name) {
        unset($settings['toggle_node_info_'. $type]);
      }
    }
  
    // Retrieve default theme settings
    $defaults = acquia_slate_default_theme_settings();
  
    // Set combined default & saved theme settings
    variable_set($theme_setting_name, array_merge($defaults, $settings));
  
    // Force theme settings refresh
    theme_get_setting('', TRUE);
  }
}


/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function acquia_slate_settings($saved_settings) {
  global $base_url;
  $theme_name = 'acquia_slate';

  // Retrieve & combine default and saved theme settings
  $defaults = acquia_slate_default_theme_settings();
  $settings = array_merge($defaults, $saved_settings);
  
  // Create theme settings form widgets using Forms API
  
  // TNT Fieldset
  $form['tnt_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Acquia Slate settings'),
    '#description' => t('Use these settings to change what and how information is displayed in your theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  // General Settings
  $form['tnt_container']['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );
  
  
  // Mission Statement
  $form['tnt_container']['general_settings']['mission_statement'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mission statement'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['mission_statement']['mission_statement_pages'] = array(
    '#type'          => 'radios',
    '#title'         => t('Where should your mission statement be displayed?'),
    '#default_value' => $settings['mission_statement_pages'],
    '#options'       => array(
                          'home' => t('Display mission statement only on front page'),
                          'all' => t('Display mission statement on all pages'),
                        ),
  );
  
  // Breadcrumb
  $form['tnt_container']['general_settings']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => $settings['breadcrumb_display'],
  );
  
  // Username
  $form['tnt_container']['general_settings']['username'] = array(
    '#type' => 'fieldset',
    '#title' => t('Username'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['general_settings']['username']['user_notverified_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "not verified" for unregistered usernames'),
    '#default_value' => $settings['user_notverified_display'],
  );
  
  // Search Settings
  if (module_exists('search')) {
    $form['tnt_container']['general_settings']['search_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search results'),
      '#description' => t('What additional information should be displayed on your search results page?'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_snippet'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display text snippet'),
      '#default_value' => $settings['search_snippet'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display content type'),
      '#default_value' => $settings['search_info_type'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_user'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display author name'),
      '#default_value' => $settings['search_info_user'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_date'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display posted date'),
      '#default_value' => $settings['search_info_date'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_comment'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display comment count'),
      '#default_value' => $settings['search_info_comment'],
    );
    $form['tnt_container']['general_settings']['search_container']['search_results']['search_info_upload'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display attachment count'),
      '#default_value' => $settings['search_info_upload'],
    );
  }
  
  // Node Settings
  $form['tnt_container']['node_type_specific'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node settings'),
    '#description' => t('Here you can make adjustments to which information is shown with your content, and how it is displayed.  You can modify these settings so they apply to all content types, or check the "Use content-type specific settings" box to customize them for each content type.  For example, you may want to show the date on stories, but not pages.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  // Author & Date Settings
  $form['tnt_container']['node_type_specific']['submitted_by_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Author and date'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Default & content-type specific settings
  if (module_exists('submitted_by') == FALSE) {
    foreach ((array('default' => 'Default') + node_get_types('names')) as $type => $name) {
      $form['tnt_container']['node_type_specific']['submitted_by_container']['submitted_by'][$type] = array(
        '#type' => 'fieldset',
        '#title' => t('!name', array('!name' => t($name))),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['tnt_container']['node_type_specific']['submitted_by_container']['submitted_by'][$type]["submitted_by_author_{$type}"] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Display author\'s username'),
        '#default_value' => $settings["submitted_by_author_{$type}"],
      );
      $form['tnt_container']['node_type_specific']['submitted_by_container']['submitted_by'][$type]["submitted_by_date_{$type}"] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Display date posted (you can customize this format on your Date and Time settings page)'),
        '#default_value' => $settings["submitted_by_date_{$type}"],
      );
      // Options for default settings
      if ($type == 'default') {
        $form['tnt_container']['node_type_specific']['submitted_by_container']['submitted_by']['default']['#title'] = t('Default');
        $form['tnt_container']['node_type_specific']['submitted_by_container']['submitted_by']['default']['#collapsed'] = $settings['submitted_by_enable_content_type'] ? TRUE : FALSE;
        $form['tnt_container']['node_type_specific']['submitted_by_container']['submitted_by']['submitted_by_enable_content_type'] = array(
          '#type'          => 'checkbox',
          '#title'         => t('Use custom settings for each content type instead of the default above'),
          '#default_value' => $settings['submitted_by_enable_content_type'],
        );
      }
      // Collapse content-type specific settings if default settings are being used
      else if ($settings['submitted_by_enable_content_type'] == 0) {
        $form['submitted_by'][$type]['#collapsed'] = TRUE;
      }
    }
  } else {
      $form['tnt_container']['node_type_specific']['submitted_by_container']['#description'] = 'NOTICE: You currently have the "Submitted By" module installed and enabled, so the Author & Date theme settings have been disabled to prevent conflicts.  If you wish to re-enable the Author & Date theme settings, you must first disable the "Submitted By" module.';
  }
    
  // Taxonomy Settings
  if (module_exists('taxonomy')) {
    $form['tnt_container']['node_type_specific']['display_taxonomy_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy terms'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    // Default & content-type specific settings
    foreach ((array('default' => 'Default') + node_get_types('names')) as $type => $name) {
      // taxonomy display per node
      $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy'][$type] = array(
        '#type' => 'fieldset',
        '#title'       => t('!name', array('!name' => t($name))),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      // display
      $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy'][$type]["taxonomy_display_{$type}"] = array(
        '#type'          => 'select',
        '#title'         => t('When should taxonomy terms be displayed?'),
        '#default_value' => $settings["taxonomy_display_{$type}"],
        '#options'       => array(
                              '' => '',
                              'never' => t('Never display taxonomy terms'),
                              'all' => t('Always display taxonomy terms'),
                              'only' => t('Only display taxonomy terms on full node pages'),
                            ),
      );
      // format
      $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy'][$type]["taxonomy_format_{$type}"] = array(
        '#type'          => 'radios',
        '#title'         => t('Taxonomy display format'),
        '#default_value' => $settings["taxonomy_format_{$type}"],
        '#options'       => array(
                              'vocab' => t('Display each vocabulary on a new line'),
                              'list' => t('Display all taxonomy terms together in single list'),
                            ),
      );
      // Get taxonomy vocabularies by node type
      $vocabs = array();
      $vocabs_by_type = ($type == 'default') ? taxonomy_get_vocabularies() : taxonomy_get_vocabularies($type);
      foreach ($vocabs_by_type as $key => $value) {
        $vocabs[$value->vid] = $value->name;
      }
      // Display taxonomy checkboxes
      foreach ($vocabs as $key => $vocab_name) {
        $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy'][$type]["taxonomy_vocab_hide_{$type}_{$key}"] = array(
          '#type'          => 'checkbox',
          '#title'         => t('Hide vocabulary: '. $vocab_name),
          '#default_value' => $settings["taxonomy_vocab_hide_{$type}_{$key}"], 
        );
      }
      // Options for default settings
      if ($type == 'default') {
        $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy']['default']['#title'] = t('Default');
        $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy']['default']['#collapsed'] = $settings['taxonomy_enable_content_type'] ? TRUE : FALSE;
        $form['tnt_container']['node_type_specific']['display_taxonomy_container']['display_taxonomy']['taxonomy_enable_content_type'] = array(
          '#type'          => 'checkbox',
          '#title'         => t('Use custom settings for each content type instead of the default above'),
          '#default_value' => $settings['taxonomy_enable_content_type'],
        );
      }
      // Collapse content-type specific settings if default settings are being used
      else if ($settings['taxonomy_enable_content_type'] == 0) {
        $form['display_taxonomy'][$type]['#collapsed'] = TRUE;
      }
    }
  }
  
  // Read More & Comment Link Settings
  $form['tnt_container']['node_type_specific']['link_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Links'),
    '#description' => t('Customize the text of node links'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
   );
  
  // Read more link settings
  $form['tnt_container']['node_type_specific']['link_settings']['readmore'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('"Read more"'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
   );
  // Default & content-type specific settings
  foreach ((array('default' => 'Default') + node_get_types('names')) as $type => $name) {
    // Read more
    $form['tnt_container']['node_type_specific']['link_settings']['readmore'][$type] = array(
      '#type'        => 'fieldset',
      '#title'       => t('!name', array('!name' => t($name))),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['readmore'][$type]["readmore_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text'),
      '#default_value' => $settings["readmore_{$type}"],
      '#description'   => t('HTML is allowed.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['readmore'][$type]["readmore_title_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Title text (tool tip)'),
      '#default_value' => $settings["readmore_title_{$type}"],
      '#description'   => t('Displayed when hovering over link. Plain text only.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['readmore'][$type]["readmore_prefix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Prefix'),
      '#default_value' => $settings["readmore_prefix_{$type}"],
      '#description'   => t('Text or HTML placed before the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['readmore'][$type]["readmore_suffix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Suffix'),
      '#default_value' => $settings["readmore_suffix_{$type}"],
      '#description'   => t('Text or HTML placed after the link.'),
    );
    // Options for default settings
    if ($type == 'default') {
      $form['tnt_container']['node_type_specific']['link_settings']['readmore']['default']['#title'] = t('Default');
      $form['tnt_container']['node_type_specific']['link_settings']['readmore']['default']['#collapsed'] = $settings['readmore_enable_content_type'] ? TRUE : FALSE;
      $form['tnt_container']['node_type_specific']['link_settings']['readmore']['readmore_enable_content_type'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Use custom settings for each content type instead of the default above'),
        '#default_value' => $settings['readmore_enable_content_type'],
      );
    }
    // Collapse content-type specific settings if default settings are being used
    else if ($settings['readmore_enable_content_type'] == 0) {
      $form['readmore'][$type]['#collapsed'] = TRUE;
    }
  }
    
  // Comments link settings
  $form['tnt_container']['node_type_specific']['link_settings']['comment'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('"Comment"'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  // Default & content-type specific settings
  foreach ((array('default' => 'Default') + node_get_types('names')) as $type => $name) {
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type] = array(
      '#type'        => 'fieldset',
      '#title'       => t('!name', array('!name' => t($name))),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    // Full nodes
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('For full content'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node']['add'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('"Add new comment" link'),
      '#description' => t('The link when the full content is being displayed.'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node']['add']["comment_node_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text'),
      '#default_value' => $settings["comment_node_{$type}"],
      '#description'   => t('HTML is allowed.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node']['add']["comment_node_title_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Title text (tool tip)'),
      '#default_value' => $settings["comment_node_title_{$type}"],
      '#description'   => t('Displayed when hovering over link. Plain text only.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node']['add']['extra'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node']['add']['extra']["comment_node_prefix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Prefix'),
      '#default_value' => $settings["comment_node_prefix_{$type}"],
      '#description'   => t('Text or HTML placed before the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['node']['add']['extra']["comment_node_suffix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Suffix'),
      '#default_value' => $settings["comment_node_suffix_{$type}"],
      '#description'   => t('Text or HTML placed after the link.'),
    );
    // Teasers
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('For teasers'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['add'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('"Add new comment" link'),
      '#description' => t('The link when there are no comments.'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['add']["comment_add_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text'),
      '#default_value' => $settings["comment_add_{$type}"],
      '#description'   => t('HTML is allowed.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['add']["comment_add_title_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Title text (tool tip)'),
      '#default_value' => $settings["comment_add_title_{$type}"],
      '#description'   => t('Displayed when hovering over link. Plain text only.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['add']['extra'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['add']['extra']["comment_add_prefix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Prefix'),
      '#default_value' => $settings["comment_add_prefix_{$type}"],
      '#description'   => t('Text or HTML placed before the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['add']['extra']["comment_add_suffix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Suffix'),
      '#default_value' => $settings["comment_add_suffix_{$type}"],
      '#description'   => t('Text or HTML placed after the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('"Comments" link'),
      '#description' => t('The link when there are one or more comments.'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard']["comment_singular_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text when there is 1 comment'),
      '#default_value' => $settings["comment_singular_{$type}"],
      '#description'   => t('HTML is allowed.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard']["comment_plural_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text when there are multiple comments'),
      '#default_value' => $settings["comment_plural_{$type}"],
      '#description'   => t('HTML is allowed. @count will be replaced with the number of comments.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard']["comment_title_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Title text (tool tip)'),
      '#default_value' => $settings["comment_title_{$type}"],
      '#description'   => t('Displayed when hovering over link. Plain text only.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard']['extra'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard']['extra']["comment_prefix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Prefix'),
      '#default_value' => $settings["comment_prefix_{$type}"],
      '#description'   => t('Text or HTML placed before the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['standard']['extra']["comment_suffix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Suffix'),
      '#default_value' => $settings["comment_suffix_{$type}"],
      '#description'   => t('Text or HTML placed after the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('"New comments" link'),
      '#description' => t('The link when there are one or more new comments.'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new']["comment_new_singular_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text when there is 1 new comment'),
      '#default_value' => $settings["comment_new_singular_{$type}"],
      '#description'   => t('HTML is allowed.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new']["comment_new_plural_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Link text when there are multiple new comments'),
      '#default_value' => $settings["comment_new_plural_{$type}"],
      '#description'   => t('HTML is allowed. @count will be replaced with the number of comments.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new']["comment_new_title_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Title text (tool tip)'),
      '#default_value' => $settings["comment_new_title_{$type}"],
      '#description'   => t('Displayed when hovering over link. Plain text only.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new']['extra'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new']['extra']["comment_new_prefix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Prefix'),
      '#default_value' => $settings["comment_new_prefix_{$type}"],
      '#description'   => t('Text or HTML placed before the link.'),
    );
    $form['tnt_container']['node_type_specific']['link_settings']['comment'][$type]['teaser']['new']['extra']["comment_new_suffix_{$type}"] = array(
      '#type'          => 'textfield',
      '#title'         => t('Suffix'),
      '#default_value' => $settings["comment_new_suffix_{$type}"],
      '#description'   => t('Text or HTML placed after the link.'),
    );
    // Options for default settings
    if ($type == 'default') {
      $form['tnt_container']['node_type_specific']['link_settings']['comment']['default']['#title'] = t('Default');
      $form['tnt_container']['node_type_specific']['link_settings']['comment']['default']['#collapsed'] = $settings['comment_enable_content_type'] ? TRUE : FALSE;
      $form['tnt_container']['node_type_specific']['link_settings']['comment']['comment_enable_content_type'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Use custom settings for each content type instead of the default above'),
        '#default_value' => $settings['comment_enable_content_type'],
      );
    }
    // Collapse content-type specific settings if default settings are being used
    else if ($settings['comment_enable_content_type'] == 0) {
      $form['comment'][$type]['#collapsed'] = TRUE;
    }
  }
  
  // SEO settings
  $form['tnt_container']['seo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search engine optimization (SEO) settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Page titles
  $form['tnt_container']['seo']['page_format_titles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page titles'),
    '#description'   => t('This is the title that displays in the title bar of your web browser. Your site title, slogan, and mission can all be set on your Site Information page'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // front page title
  if (module_exists('page_title') == FALSE) {
    $form['tnt_container']['seo']['page_format_titles']['front_page_format_titles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Front page title'),
      '#description'   => t('Your front page in particular should have important keywords for your site in the page title'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tnt_container']['seo']['page_format_titles']['front_page_format_titles']['front_page_title_display'] = array(
      '#type' => 'select',
      '#title' => t('Set text of front page title'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#default_value' => $settings['front_page_title_display'],
      '#options' => array(
                    'title_slogan' => t('Site title | Site slogan'),
                    'slogan_title' => t('Site slogan | Site title'),
                    'title_mission' => t('Site title | Site mission'),
                    'custom' => t('Custom (below)'),
                  ),
    );
    $form['tnt_container']['seo']['page_format_titles']['front_page_format_titles']['page_title_display_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom'),
      '#size' => 60,
      '#default_value' => $settings['page_title_display_custom'],
      '#description'   => t('Enter a custom page title for your front page'),
    );
    // other pages title
    $form['tnt_container']['seo']['page_format_titles']['other_page_format_titles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Other page titles'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tnt_container']['seo']['page_format_titles']['other_page_format_titles']['other_page_title_display'] = array(
      '#type' => 'select',
      '#title' => t('Set text of other page titles'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#default_value' => $settings['other_page_title_display'],
      '#options' => array(
                    'ptitle_slogan' => t('Page title | Site slogan'),
                    'ptitle_stitle' => t('Page title | Site title'),
                    'ptitle_smission' => t('Page title | Site mission'),
                    'ptitle_custom' => t('Page title | Custom (below)'),
                    'custom' => t('Custom (below)'),
                  ),
    );
    $form['tnt_container']['seo']['page_format_titles']['other_page_format_titles']['other_page_title_display_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom'),
      '#size' => 60,
      '#default_value' => $settings['other_page_title_display_custom'],
      '#description'   => t('Enter a custom page title for all other pages'),
    );
    // SEO configurable separator
    $form['tnt_container']['seo']['page_format_titles']['configurable_separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Title separator'),
      '#description' => t('Customize the separator character used in the page title'),
      '#size' => 60,
      '#default_value' => $settings['configurable_separator'],
    );
  } else {
    $form['tnt_container']['seo']['page_format_titles']['#description'] = 'NOTICE: You currently have the "Page Title" module installed and enabled, so the Page titles theme settings have been disabled to prevent conflicts.  If you wish to re-enable the Page titles theme settings, you must first disable the "Page Title" module.';
    $form['tnt_container']['seo']['page_format_titles']['configurable_separator']['#disabled'] = 'disabled';
  }
  // Metadata
  $form['tnt_container']['seo']['meta'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags'),
    '#description' => t('Meta tags aren\'t used much by search engines anymore, but the meta description is important -- this is what will be shown as the description of your link in search engine results.  NOTE: For more advanced meta tag functionality, check out the Meta Tags (aka. Node Words) module.  These theme settings do not work in conjunction with this module and will not appear if you have it enabled.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (module_exists('nodewords') == FALSE) {
    $form['tnt_container']['seo']['meta']['meta_keywords'] = array(
      '#type' => 'textfield',
      '#title' => t('Meta keywords'),
      '#description' => t('Enter a comma-separated list of keywords'),
      '#size' => 60,
      '#default_value' => $settings['meta_keywords'],
    );
    $form['tnt_container']['seo']['meta']['meta_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Meta description'),
      '#cols' => 60,
      '#rows' => 6,
      '#default_value' => $settings['meta_description'],
    );
  }
  else {
    $form['tnt_container']['seo']['meta']['#description'] = 'NOTICE: You currently have the "nodewords" module installed and enabled, so the meta tag theme settings have been disabled to prevent conflicts.  If you later wish to re-enable the meta tag theme settings, you must first disable the "nodewords" module.';
    $form['tnt_container']['seo']['meta']['meta_keywords']['#disabled'] = 'disabled';
    $form['tnt_container']['seo']['meta']['meta_description']['#disabled'] = 'disabled';
  }

  // Development settings
  $form['tnt_container']['themedev'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme development settings'),
    '#collapsible' => TRUE,
    '#collapsed' => $settings['rebuild_registry'] ? FALSE : TRUE,
  );
 $form['tnt_container']['themedev']['rebuild_registry'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild theme registry for every page.'),
    '#default_value' => $settings['rebuild_registry'],
    '#description' => t('This setting is useful while developing themes (see <a href="!link">rebuilding the theme registry</a>). However, it <strong>significantly degrades performance</strong> and should be turned off for any production website.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
  );
 $form['tnt_container']['themedev']['fix_css_limit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fix IE stylesheet limit.'),
    '#default_value' => $settings['fix_css_limit'],
    '#description' => t('This setting groups css files so Internet Explorer can see more than 30 of them. This is useful when you cannot use aggregation (e.g., when developing or using private file downloads). But because it degrades performance and can load files out of order, CSS aggregation (<a href="!link">Optimize CSS files</a>) is <strong>strongly</strong> recommended instead for any production website.', array('!link' => $base_url .'/admin/settings/performance')),
  );

  //
  // Add custom settings, if present
  //
  // Custom settings
  $form['tnt_container']['custom_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Theme width
  $form['tnt_container']['custom_settings']['theme_width_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme width'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['custom_settings']['theme_width_config']['theme_width'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select a fixed or fluid width'),
    '#default_value' => $settings['theme_width'] ? $settings['theme_width'] : 'fixed',
    '#options'       => array(
      'fixed' => t('<strong>DEFAULT</strong> - Fixed'),
      'fluid' => t('Fluid'),
    ),
  );
// Theme fonts
  $form['tnt_container']['custom_settings']['theme_fonts_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme fonts'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['custom_settings']['theme_fonts_config']['theme_fonts'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select a font family'),
    '#default_value' => $settings['theme_fonts'] ? $settings['theme_fonts'] : 'fonts_1',
    '#options'       => array(
      'fonts_1' => t('<strong>DEFAULT</strong> - Helvetica, Arial, Verdana, "Bitstream Vera Sans", sans-serif'),
      'fonts_2' => t('Lucida Grande, Lucida Sans Unicode, Verdana, Helvetica, Arial, sans-serif'),
      'fonts_3' => t('Tahoma, Verdana, Arial, Helvetica, sans-serif'),
      'fonts_4' => t('Georgia, Garamond, Times New Roman, serif'),
    ),
  );
  // Theme banner
  // Scan the "banner" directory for images
  $dir =  drupal_get_path('theme', $theme_name) . '/theme_settings/banners';
  $ext = array('.jpg', '.png', '.gif');
  $files = array('none' => 'none');
  if ($handle = opendir($dir)) {
    while (false !== ($file = readdir($handle))) {
      if (in_array(substr($file, -4), $ext) && $filename = filter_xss(substr($file, 0, -4))) {
        $files[$file] = $file;
      }
    }
  }
  closedir($handle);
  // Custom banner setting
  $form['tnt_container']['custom_settings']['theme_banner_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme banner'),
    '#description' => t('You can add images to be used as the banner. Place your image in the banners directory located at acquia_slate/theme_settings/banners. Once you add an image to the banners directory and then reload this page, your newly added image will be available to choose from in the drop down below. Only .jpg, .png, and .gif image files can be used. For best results, resize your images to 960x520 pixels.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tnt_container']['custom_settings']['theme_banner_config']['theme_banner'] = array(
    '#type'          => 'select',
    '#title'         => t('Select a theme banner'),
    '#default_value' => $settings['theme_banner'] ? $settings['theme_banner'] : 'none',
    '#options'       => $files,
  );

  // Return theme settings form
  return $form;
}  

?>