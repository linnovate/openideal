<?php
// $Id: nodewords_extra.nodewords.tags.inc,v 1.1.2.4 2010/03/24 13:24:28 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_contributor_form(&$form, $content, $options) {
  $form['dc.contributor'] = array(
    '#tree' => TRUE,
  );

  $form['dc.contributor']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core contributor'),
    '#description' => t('Enter the name of an entity responsible for making contributions to the resource. Examples of a contributor include a person, an organization, or a service.') . $options['description'],
    '#default_value' => empty($content['value']) ? (!empty($options['default']['dc.contributor']['value']) ? $options['default']['dc.contributor']['value'] : '') : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_contributor_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['dc.contributor'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_creator_form(&$form, $content, $options) {
  $form['dc.creator'] = array(
    '#tree' => TRUE,
  );

  $form['dc.creator']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core creator'),
    '#description' => t('Enter the name of an entity primarily responsible for making the resource. Examples of a creator include a person, an organization, or a service.') . $options['description'],
    '#default_value' => empty($content['value']) ? (!empty($options['default']['dc.creator']['value']) ? $options['default']['dc.creator']['value'] : '') : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_creator_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['dc.creator'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_date_form(&$form, $content, $options) {
  $form['dc.date'] = array(
    '#tree' => TRUE,
  );

  $bool = (
    isset($content['value']['day']) &&
    isset($content['value']['month']) &&
    isset($content['value']['year'])
  );
  if ($bool) {
    $value = $content['value']['day'] . ' ' . $content['value']['month'] . ' ' . $content['value']['year'];
  }
  elseif (isset($content['value']) && is_string($content['value'])) {
    $value = $content['value'];
  }
  else {
    $value = '';
  }

  $form['dc.date']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core date'),
    '#description' => t('Enter day, month, and year in this order, separated by a space; a invalid date will not be used. The timezone is UTC.') . $options['description'],
    '#default_value' => $value,
  );
}

/**
 * Check if the meta tag content is empty.
 */
function nodewords_extra_dc_date_is_empty($content) {
  $bool = (
    isset($content['value']['day']) &&
    isset($content['value']['month']) &&
    isset($content['value']['year'])
  );
  if ($bool) {
    return FALSE;
  }

  return empty($content['value']);
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_date_prepare(&$tags, $content, $options) {
  $bool = (
    isset($content['value']['day']) &&
    isset($content['value']['month']) &&
    isset($content['value']['year'])
  );
  if ($bool) {
    $value = $content['value']['day'] . ' ' . $content['value']['month'] . ' ' . $content['value']['year'];
  }
  elseif (isset($content['value']) && is_string($content['value'])) {
    $value = $content['value'];
  }
  else {
    $value = '';
  }
  if (!empty($value)) {
    $values = array_filter(array_map('trim', explode(' ', $value)));
    if (count($values) == 3) {
      $time = mktime(0, 0, 0, $values[0], $values[1], $values[2]);
      if ($time > 0) {
        $tags['dc.date'] = date('Y-m-d\TH:i:s\Z', $time);
      }
    }
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_description_form(&$form, $content, $options) {
  if (!isset($form['description'])) {
    $bool = (
      isset($options['content type']) &&
      variable_get('nodewords_use_teaser_' . $options['content type'], FALSE)
    );
    if (!$bool) {
      $form['dc.description'] = array(
        '#tree' => TRUE,
      );

      $form['dc.description']['value'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#description' => t('Enter a description. Limit your description to about 20 words, with a maximum of %count characters. It should not contain any HTML tags or other formatting.', array('%count' => variable_get('nodewords_max_size', 350))) . $options['description'],
        '#default_value' => empty($content['value']) ? (!empty($options['default']['dc.description']['value']) ? $options['default']['dc.description']['value'] : '') : $content['value'],
        '#cols' => 60,
        '#rows' => 6,
        '#wysiwyg' => FALSE,
      );
    }
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_description_prepare(&$tags, $content, $options) {
  if (empty($tags['description'])) {
    if (!empty($content['value'])) {
      $tags['dc.description'] = nodewords_replace_tokens($content['value'], $options);
    }
  }
  else {
    $tags['dc.description'] = $tags['description'];
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_publisher_form(&$form, $content, $options) {
  $form['dc.publisher'] = array(
    '#tree' => TRUE,
  );

  $form['dc.publisher']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core publisher'),
    '#description' => t('Enter a name of an entity responsible for making the resource available. Examples of a publisher include a person, an organization, or a service.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_publisher_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['dc.publisher'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_dc_title_form(&$form, $content, $options) {
  $form['dc.title'] = array(
    '#tree' => TRUE,
  );

  $form['dc.title']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Dublin Core title'),
    '#description' => t('Enter an alternative title. Do not use the value already used for the HTML tag TITLE, or you will probably get warning reports about duplicated titles from the search engines.') . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_dc_title_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['dc.title'] = nodewords_replace_tokens($content['value'], $options);
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_location_form(&$form, $content, $options) {
  $form['location'] = array(
    '#tree' => TRUE,
  );

  $form['location']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#description' => t('Enter a value between -180, and +180; any value outside the range will not be used.') . $options['description'],
    '#default_value' => empty($content['latitude']) ? '' : $content['latitude'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );

  $form['location']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#description' => t('Enter a value between -180, and +180; any value outside the range will not be used.') . $options['description'],
    '#default_value' => empty($content['longitude']) ? '' : $content['longitude'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Check if the meta tag content is empty.
 */
function nodewords_extra_location_is_empty($content) {
  return empty($content['latitude']) && empty($content['longitude']);
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_location_prepare(&$tags, $content, $options) {
  if (!empty($content['latitude']) && !empty($content['longitude'])) {
    $latitude = nodewords_replace_tokens($content['latitude'], $options);
    $longitude = nodewords_replace_tokens($content['longitude'], $options);

    $bool = (
      is_numeric($latitude) &&
      is_numeric($longitude) &&
      $latitude >= -180 && $latitude <= 180 &&
      is_numeric($longitude) &&
      is_numeric($longitude) &&
      $longitude >= -180 && $longitude <= 180
    );
    if ($bool) {
      $tags['location:geo.position'] = $latitude . ';' . $longitude;
      $tags['location:icbm'] = $latitude . ',' . $longitude;
    }
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_pics_form(&$form, $content, $options) {
  $form['pics-label'] = array(
    '#tree' => TRUE,
  );

  $form['pics-label']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('PICS labels'),
    '#description' => t('Enter the ICRA PICS labels only, without any extra characters; ICRA PICS labels should be something similar to %icra-pics-example. You can use the online <a href="@icra-label-generator">ICRA label generator</a>.', array('%icra-pics-example' => 'n 0 s 0 v 0 l 0 oa 0 ob 0 oc 0 od 0 oe 0 of 0 og 0 oh 0 c 1', '@icra-label-generator' => 'http://www.icra.org/label/generator/')) . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_pics_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    if ($options['type'] == NODEWORDS_TYPE_NODE) {
      $tags['pics-label'] = strtr(
        '(pics-1.1 "http://www.icra.org/pics/vocabularyv03/" l gen true for %url r (%pics))',
        array(
          '%url' => nodewords_url("node/{$options['id']}"),
          '%pics' => $value,
        )
      );
    }
    elseif ($options['type'] == NODEWORDS_TYPE_PAGE) {
      $tags['pics-label'] = strtr(
        '(pics-1.1 "http://www.icra.org/pics/vocabularyv03/" l gen true for %url r (%pics))',
        array(
          '%url' => nodewords_url($_GET['q']),
          '%pics' => $value,
        )
      );
    }
  }

  if (variable_get('nodewords_icra_validation_content', '')) {
    $tags['pics-label:meta'] = nodewords_url('labels.rdf', array('absolute' => TRUE));
  }
}

function nodewords_extra_pics_settings_form(&$form, $form_id, $options) {
  if ($form_id == 'nodewords_admin_settings_form') {
    $form['metatags_creation']['nodewords_icra_validation_content'] = array(
      '#type' => 'textarea',
      '#title' => t('ICRA validation content'),
      '#description' => t('Enter the validation content you obtained from the <a href="@icra-generator">ICRA label generator</a>. For more information, see <a href="@icra-support">ICRA support</a>.', array('@icra-support' => 'http://www.icra.org/support/', '@icra-generator' => 'http://www.icra.org/label/generator/')),
      '#default_value' => variable_get('nodewords_icra_validation_content', ''),
      '#wysiwyg' => FALSE,
    );
  }
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_extra_shorturl_form(&$form, $content, $options) {
  $form['shorturl'] = array(
    '#tree' => TRUE,
  );

  $form['shorturl']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Short URL'),
    '#description' => t('Short URLs are used instead of the original URL because they are shorter, and easier to remember. Short URLs are provided by some web services, such as <a href="@bitly">bit.ly</a>, <a href="@shorturl">ShortURL</a>, and <a href="@tinyurl">TinyURL</a>. Enter the absolute URL.', array('@bitly' => 'http://bit.ly', '@shorturl' => 'http://shorturl.com', '@tinyurl' => 'http://tinyurl.com')) . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#element_validate' => array('nodewords_validate_element'),
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_extra_shorturl_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['shorturl'] = check_url(
      nodewords_replace_tokens($content['value'], $options)
    );
  }
}
