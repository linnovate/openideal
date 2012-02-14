<?php
// $Id: nodewords_basic.nodewords.tags.inc,v 1.1.2.6 2010/04/17 19:00:54 kiam Exp $

/**
 * @file
 * Meta tags definition file.
 */

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_abstract_form(&$form, $content, $options) {
  $form['abstract'] = array(
    '#tree' => TRUE,
  );

  $form['abstract']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Abstract'),
    '#description' => t('Enter a short abstract. Typically it is one sentence.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_abstract_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['abstract'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_canonical_form(&$form, $content, $options) {
  $form['canonical'] = array(
    '#tree' => TRUE,
  );

  $form['canonical']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Canonical URL'),
    '#description' => t('Canonical URLs are used from the search engines, and allow them to not report duplicate titles for HTML pages that are accessible from different URLs. Use a relative URL without the initial slash, or enter <code><front></code> for the front page path; canonical URLs that point to a different domain are normally not accepted.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_basic_canonical_form_validate',
    ),
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

function nodewords_basic_canonical_form_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $canonical_url = trim($element['#value'], '/');

    if (!empty($canonical_url) && !valid_url(drupal_urlencode($canonical_url))) {
      form_error($element, t('Canonical URL must be a relative URL.'));
    }
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_canonical_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['canonical'] = check_url(
      nodewords_url(
        trim(nodewords_replace_tokens($content['value'], $options), '/'), $options
      )
    );
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_copyright_form(&$form, $content, $options) {
  $form['copyright'] = array(
    '#tree' => TRUE,
  );

  $form['copyright']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
    '#description' => t('Enter a short copyright statement.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_copyright_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['copyright'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_description_form(&$form, $content, $options) {
  $form['description'] = array(
    '#tree' => TRUE,
  );

  $form['description']['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Enter a description. Limit your description to about 20 words, with a maximum of %count characters. It should not contain any HTML tags or other formatting.', array('%count' => variable_get('nodewords_max_size', 350))) . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#cols' => 60,
    '#rows' => 6,
    '#wysiwyg' => FALSE,
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_description_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['description'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_keywords_form(&$form, $content, $options) {
  $form['keywords'] = array(
    '#tree' => TRUE,
  );

  $form['keywords']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Keywords'),
    '#description' => t('Enter a comma separated list of keywords. Avoid duplication of words as this will lower your search engine ranking.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_basic_keywords_form_validate',
    ),
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Validate the values passed as keywords.
 */
function nodewords_basic_keywords_form_validate($element, &$form_state) {
  if (trim($element['#value']) == ',') {
    form_error($element, t('The meta tag KEYWORDS value must be a comma separated list of words.'));
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_keywords_prepare(&$tags, $content, $options) {
  $value = variable_get('nodewords_first_keywords', '');

  if (!empty($content['value'])) {
    $value .= ',' . $content['value'];
  }

  $tags['keywords'] = nodewords_unique_values(
    nodewords_replace_tokens($value . ',' . variable_get('nodewords_last_keywords', ''), $options)
  );
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_revisit_after_form(&$form, $content, $options) {
  $form['revisit-after'] = array(
    '#tree' => TRUE,
  );

  $form['revisit-after']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Revisit after'),
    '#description' => t('The meta tag REVISIT-AFTER defines how often a search engine or spider should come to your website for re-indexing. This tag is used for websites that change their content and on a regular basis. This tag can also be beneficial in boosting your rankings if search engines display results based on the most recent submissions.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#size' => 3,
    '#element_validate' => array('nodewords_validate_element'),
    '#validate_args' => array(
      'module' => 'nodewords_basic',
      'callback' => 'nodewords_basic_revisit_after_form_validate',
    ),
    '#maxlength' => 3,
    '#field_suffix' => t('day(s)'),
  );
}

/**
 * Validate the meta tag value.
 */
function nodewords_basic_revisit_after_form_validate($element, &$form_state) {
  if (!preg_match('/^[0-9]*$/', $element['#value'])) {
    form_error($element, t('The meta tag REVISIT-AFTER value must be a positive number.'));
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_revisit_after_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $value = $content['value'];
    $tags['revisit-after'] = $value > 1 ? "$value days" : '1 day';
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_basic_robots_form(&$form, $content, $options) {
  $checkbox_options = array(
    'noarchive' => t('NOARCHIVE'),
    'noodp' => t('NOODP'),
    'nosnippet' => t('NOSNIPPET'),
    'noydir' => t('NOYDIR'),
  );

  $select_options = array(
    'index,follow' => 'INDEX,FOLLOW',
    'noindex,follow' => 'NOINDEX,FOLLOW',
    'index,nofollow' => 'INDEX,NOFOLLOW',
    'noindex,nofollow' => 'NOINDEX,NOFOLLOW',
  );

  if (isset($content['value']['noopd'])) {
    $content['value']['noodp'] = is_string($content['value']['noopd']) ? 'noodp' : 0;
    unset($content['value']['noopd']);
  }

  $form['robots'] = array(
    '#tree' => TRUE,
  );

  $form['robots']['index_follow'] = array(
    '#type' => 'select',
    '#title' => t('Robots'),
    '#options' => $select_options,
    '#multiple' => FALSE,
    '#default_value' => empty($content['index_follow']) ? 'noindex,nofollow' : $content['index_follow'],
  );

  $form['robots']['value'] = array(
    '#type' => 'checkboxes',
    '#description' => t("The meta tag ROBOTS, supported by several major search engines, controls whether search engine spiders are allowed to index a page, or not, and whether they should follow links from a page, or not. The meta tag can contain one or more comma-separate values. The <em>noindex</em> value prevents a page from being indexed, and <em>nofollow</em> prevents links from being crawled. Other values recognized by one or more search engines can influence how the engine indexes pages, and how those pages appear on the search results. These include <em>noarchive</em>, which instructs a search engine not to store an archived copy of the page, and <em>nosnippet</em>, which asks that the search engine not include a snippet from the page along with the page's listing in search results."),
    '#default_value' => empty($content['value']) ? array() : $content['value'],
    '#options' => $checkbox_options,
    '#checkall' => variable_get('nodewords_enable_checkall', FALSE),
  );
}

/**
 * Check if the meta tag content is empty.
 */
function nodewords_basic_robots_is_empty($content) {
  $result = empty($content['index_follow']);

  if (isset($content['value']) && is_array($content['value'])) {
    $value = array_filter($content['value']);

    return ($result && empty($value));
  }

  return FALSE;
}

/**
 * Set the meta tag content.
 */
function nodewords_basic_robots_prepare(&$tags, $content, $options) {
  if ($options['type'] == NODEWORDS_TYPE_PAGER) {
    $index_follow = variable_get('nodewords_list_robots_index_follow', 'noindex,nofollow');
    $robots = variable_get('nodewords_list_robots_value', array());
    $value = explode(',', $index_follow);
    $value = array_merge($value, !empty($robots) ? array_values(array_filter($robots)) : array());
  }
  else {
    $value = !empty($content['index_follow']) ? explode(',', $content['index_follow']) : array('noindex', 'nofollow');
    $value = array_merge($value, !empty($content['value']) ? array_values(array_filter($content['value'])) : array());
  }

  if (!empty($value)) {
    $tags['robots'] = implode(',', $value);
  }
}

function nodewords_basic_robots_settings_form(&$form, $form_id, $options) {
  switch ($form_id) {
    case 'nodewords_admin_settings_form':
      $checkbox_options = array(
        'noarchive' => t('NOARCHIVE'),
        'noodp' => t('NOODP'),
        'nosnippet' => t('NOSNIPPET'),
        'noydir' => t('NOYDIR'),
      );

      $select_options = array(
        'index,follow' => 'INDEX,FOLLOW',
        'noindex,follow' => 'NOINDEX,FOLLOW',
        'index,nofollow' => 'INDEX,NOFOLLOW',
        'noindex,nofollow' => 'NOINDEX,NOFOLLOW',
      );

      $form['metatags_creation']['nodewords_list_robots_index_follow'] = array(
        '#type' => 'select',
        '#title' => t('Meta tag ROBOTS for lists'),
        '#options' => $select_options,
        '#multiple' => FALSE,
        '#default_value' => variable_get('nodewords_list_index_follow', 'noindex.nofollow'),
      );

      $form['metatags_creation']['nodewords_list_robots_value'] = array(
        '#type' => 'checkboxes',
        '#description' => t('The value to use for the meta tag ROBOTS for the pages that use a pager; this setting is not used for the first page of the list, or when the option to repeat the meta tags for all the pages in the same list is selected.'),
        '#default_value' => variable_get('nodewords_list_value', array()),
        '#options' => $checkbox_options,
        '#checkall' => variable_get('nodewords_enable_checkall', FALSE),
      );
  }
}
