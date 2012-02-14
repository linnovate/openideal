<?php
// $Id: page_title.admin.inc,v 1.1.2.8 2009/10/31 22:18:54 njt1982 Exp $

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function page_title_admin_settings() {
  // Define a default looking 'form element' for setting.
  $showfield_form_element = array('#type' => 'checkbox', );


  // Define a default looking 'form element' for setting.
  $pattern_form_element = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 256,
  );


  // Set the theme callback for the patterns section
  $form['patterns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Title Patterns'),
    '#collapsible' => TRUE,
    '#theme' => 'page_title_admin_settings'
  );

  // Define the basic scope column values
  $form['patterns']['scope'] = array(
    'page_title_default' => array('#type' => 'markup', '#value' => t('Global Only'), ),
    'page_title_front'   => array('#type' => 'markup', '#value' => t('Global Only'), ),
    'page_title_user'    => array('#type' => 'markup', '#value' => t('User'), ),
  );

  // Define the 'default' token patterns
  $form['patterns']['pattern'] = array(
    'page_title_default' => array(
      '#title' => t('Default'),
      '#default_value' => variable_get('page_title_default', '[page-title] | [site-name]'),
      '#required' => TRUE,
    ) + $pattern_form_element,
    'page_title_front' => array(
      '#title' => t('Frontpage'),
      '#default_value' => variable_get('page_title_front', '[site-name] | [site-slogan]'),
    ) + $pattern_form_element,
    'page_title_user' => array(
      '#title' => t('User Profile'),
      '#default_value' => variable_get('page_title_user', ''),
    ) + $pattern_form_element,
  );


  // Define the "showfield" checkbox for the user profile page
  $form['patterns']['showfield']['page_title_user_showfield'] = array(
    '#default_value' => variable_get('page_title_user_showfield', 0),
  ) + $showfield_form_element;


  // Definate the patterns per-node-type
  $types = node_get_types();
  foreach ($types as $type) {
    // Define the node-type key
    $key = 'page_title_type_'. $type->type;

    // Pattern entry
    $form['patterns']['pattern'][$key] = array(
      '#title' => t('Content Type - %type', array('%type' => $type->name)),
      '#default_value' => variable_get($key, ''),
    ) + $pattern_form_element;

    $form['patterns']['showfield'][$key .'_showfield'] = array(
      '#default_value' => variable_get($key .'_showfield', 0),
    ) + $showfield_form_element;

    $form['patterns']['scope'][$key] = array('#type' => 'markup', '#value' => t('Node'), );
  }


  // Definate the patterns per-vocab-type - if Taxonomy Module is enabled
  if (module_exists('taxonomy')) {
    $vocabs = taxonomy_get_vocabularies();
    foreach ($vocabs as $vocab) {
      // Define the vocab key
      $key = 'page_title_vocab_'. $vocab->vid;

      // Pattern entry
      $form['patterns']['pattern'][$key] = array(
        '#title' => t('Vocabulary - %vocab_name', array('%vocab_name' => $vocab->name)),
        '#default_value' => variable_get($key, ''),
      ) + $pattern_form_element;

      $form['patterns']['showfield'][$key .'_showfield'] = array(
        '#default_value' => variable_get($key .'_showfield', 0),
      ) + $showfield_form_element;

      $form['patterns']['scope'][$key] = array('#type' => 'markup', '#value' => t('Taxonomy'), );
    }
  }

  // Add the blog homepage pattern field
  if (module_exists('blog')) {
    $key = 'page_title_blog';
    $form['patterns']['pattern'][$key] = array(
      '#title' => t('Blog Homepage'),
      '#default_value' => variable_get($key, ''),
    ) + $pattern_form_element;

    $form['patterns']['scope'][$key] = array('#type' => 'markup', '#value' => t('User'), );
  }

  // Define the page pattern text field. This is appended to any page requests containing 'page=[0-9]+' in the query string
  $form['page_title_pager_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern for pages that contain a pager'),
    '#default_value' => variable_get('page_title_pager_pattern', ''),
    '#description' => t('This pattern will be appended to a page title for any given page with a pager on it'),
  );

  // Add the token help to a collapsed fieldset at the end of the configuration page.
  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Tokens List'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['content'] = array(
    '#type' => 'markup',
    '#value' => theme('token_help'),
  );

  // Return the form with the system buttons & action added to it
  return system_settings_form($form);
}

