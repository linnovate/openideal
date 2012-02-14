<?php
// $Id: nodewords_extra.nodewords.hooks.inc,v 1.1.2.6 2010/04/11 15:39:45 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Implements hook_nodewords_tags_info().
*/
function nodewords_extra_nodewords_tags_info() {
  $tags = array(
    'dc.contributor' => array(
      'callback' => 'nodewords_extra_dc_contributor',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core contributor'),
      'templates' => array(
        'head' => array(
          'dc.contributor' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'dc.creator' => array(
      'callback' => 'nodewords_extra_dc_creator',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core creator'),
      'templates' => array(
        'head' => array(
          'dc.creator' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'dc.date' => array(
      'callback' => 'nodewords_extra_dc_date',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core date'),
      'templates' => array(
        'head' => array(
          'dc.date' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'dc.description' => array(
      'callback' => 'nodewords_extra_dc_description',
      'label' => t('Dublin Core description'),
      'templates' => array(
        'head' => array(
          'dc.description' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'dc.publisher' => array(
      'callback' => 'nodewords_extra_dc_publisher',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('Dublin Core publisher'),
      'templates' => array(
        'head' => array(
          'dc.publisher' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'dc.title' => array(
      'context' => array(
        'denied' => array(
          NODEWORDS_TYPE_ERRORPAGE,
        ),
      ),
      'callback' => 'nodewords_extra_dc_title',
      'label' => t('Dublin Core title'),
      'templates' => array(
        'head' => array(
          'dc.title' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'location' => array(
      'callback' => 'nodewords_extra_location',
      'label' => t('Location'),
      'templates' => array(
        'geo.position' => NODEWORDS_META,
        'icbm' => NODEWORDS_META,
      ),
      'tokens' => TRUE,
    ),
    'pics-label' => array(
      'attributes' => array(
        'meta' => array(
          'title' => 'ICRA labels',
          'type' => 'application/rdf+xml',
        ),
      ),
      'callback' => 'nodewords_extra_pics',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
        ),
      ),
      'label' => t('PICS labels'),
      'templates' => array(
        'pics-label' => NODEWORDS_HTTP_EQUIV,
        'meta' => NODEWORDS_LINK_REL,
      ),
      'weight' => array('meta' => -10),
    ),
    'shorturl' => array(
      'context' => array(
        'denied' => array(
          NODEWORDS_TYPE_ERRORPAGE,
        ),
      ),
      'callback' => 'nodewords_extra_shorturl',
      'label' => t('Short URL'),
      'templates' => array(
        'head' => array(
          'shorturl' => NODEWORDS_LINK_REL,
        ),
      ),
      'tokens' => TRUE,
      'weight' => array('shorturl' => -9),
    ),
  );

  return $tags;
}

function nodewords_extra_nodewords_tags_output_alter(&$output, $parameters) {
  $bool = (
    variable_get('nodewords_add_dc_schema', FALSE) &&
    isset($parameters['output']) &&
    $parameters['type'] != NODEWORDS_TYPE_NONE &&
    $parameters['output'] == 'head'
  );
  if ($bool) {
    $output = (
      '<link rel="schema.dc" href="http://purl.org/dc/elements/1.1/" />' . "\n" .
      $output
    );
  }
}
