<?php
// $Id: nodewords_basic.nodewords.hooks.inc,v 1.1.2.5 2010/04/11 15:39:45 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
* Implements hook_nodewords_tags_info().
 */
function nodewords_basic_nodewords_tags_info() {
  $tags = array(
    'abstract' => array(
      'callback' => 'nodewords_basic_abstract',
      'label' => t('Abstract'),
      'templates' => array(
        'head' => array(
          'abstract' => NODEWORDS_META,
        ),
      ),
      'tokens' => TRUE,
    ),
    'canonical' => array(
      'callback' => 'nodewords_basic_canonical',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
          NODEWORDS_TYPE_PAGER,
          NODEWORDS_TYPE_TERM,
          NODEWORDS_TYPE_USER,
          NODEWORDS_TYPE_VOCABULARY,
        ),
        'denied' => array(
          NODEWORDS_TYPE_ERRORPAGE,
        ),
      ),
      'label' => t('Canonical URL'),
      'templates' => array(
        'head' => array(
          'canonical' => NODEWORDS_LINK_REL,
        ),
      ),
      'tokens' => TRUE,
      'weight' => array('canonical' => -10),
    ),
    'copyright' => array(
      'callback' => 'nodewords_basic_copyright',
      'label' => t('Copyright'),
      'templates' => array(
        'head' => array(
          'copyright' => NODEWORDS_META
        ),
      ),
      'tokens' => TRUE,
      'weight' => array('copyright' => -7),
    ),
    'description' => array(
      'callback' => 'nodewords_basic_description',
      'label' => t('Description'),
      'templates' => array(
        'head' => array(
          'description' => NODEWORDS_META
        ),
      ),
      'tokens' => TRUE,
      'weight' => array('description' => -6),
    ),
    'keywords' => array(
      'callback' => 'nodewords_basic_keywords',
      'label' => t('Keywords'),
      'templates' => array(
        'head' => array(
          'keywords' => NODEWORDS_META
        ),
      ),
      'tokens' => TRUE,
      'weight' => array('keywords' => -8),
    ),
    'revisit-after' => array(
      'callback' => 'nodewords_basic_revisit_after',
      'label' => t('Revisit after'),
      'templates' => array(
        'head' => array(
          'revisit-after' => NODEWORDS_META,
        ),
      ),
    ),
    'robots' => array(
      'callback' => 'nodewords_basic_robots',
      'context' => array(
        'allowed' => array(
          NODEWORDS_TYPE_DEFAULT,
          NODEWORDS_TYPE_ERRORPAGE,
          NODEWORDS_TYPE_NODE,
          NODEWORDS_TYPE_PAGE,
          NODEWORDS_TYPE_PAGER,
          NODEWORDS_TYPE_TERM,
          NODEWORDS_TYPE_USER,
          NODEWORDS_TYPE_VOCABULARY,
        ),
      ),
      'label' => t('Robots'),
      'templates' => array(
        'head' => array(
          'robots' => NODEWORDS_META,
        ),
      ),
    ),
  );

  return $tags;
}
