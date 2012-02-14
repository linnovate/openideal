<?php

// $Id: nodewords.api.php,v 1.1.2.17 2010/04/18 14:16:48 kiam Exp $

/**
 * @file.
 * Nodewords hooks.
 */

/**
 * The hook is used from nodewords.module to know which API is supported by the
 * the module.
 *
 * @return
 * An array containing the following indexes:
 *
 *   - api - the API version used by the module; basing on this value
 *     Nodewords will take the necessary steps to assure to keep the module
 *     compatible with Nodewords, The minimum API currently supported by the
 *     module is contained in the constant NODEWORDS_MINIMUM_API_VERSION, and
 *     the current API version is contained in the constant
 *     NODEWORDS_API_VERSION.
 *   - path - the path where the files for the integration with Nodewords are
 *     placed.
*/
function hook_nodewords_api() {
  return array('api' => '1.12', 'path' => '');
}

/**
 * This hook allow modules to be notified when meta tags are deleted.
 *
 * @param $options.
 *   An array of options that allows to identify the meta tags being deleted.
 */
function hook_nodewords_delete_tags($options) {
  if ($options['type'] == NODEWORDS_TYPE_PAGE) {
    db_query("DELETE FROM {nodewords_custom} WHERE pid = '%s'", $options['id']);
  }
}

/**
 * This hook declares the meta tags implemented by the module.
 *
 *
 * @return
 *   An array containing the following values:
 *
 *  - attributes - the tag attributes used when outputting the tag on HTML HEAD.
 *  - callback - the string used to built the name of the functions called for
 *    any meta tags operations.
 *  - context - the contexts in which the meta tags are allowed (and denied).
 *  - label - the label used as title in the fieldset for the form field
 *    shown in the form to edit the meta tags values.
 *  - templates - the templates used when the meta tag is output.
 *  - weight - the weight used to order the meta tags before to output them;
 *    the lighter meta tag will be output first.
 *
 */
function hook_nodewords_tags_info() {
  $tags = array(
    'dc.title' => array(
      'callback' => 'nodewords_extra_dc_title',
      'context' => array(
        'denied' => array(
          NODEWORDS_TYPE_DEFAULT,
        ),
      ),
      'label' => t('Dublin Core title'),
      'templates' => array(
        'head' => array(
          'dc.title' => NODEWORDS_META,
        ),
      ),
    ),
    'location' => array(
      'callback' => 'nodewords_extra_location',
      'label' => t('Location'),
      'templates' => array(
        'geo.position' => NODEWORDS_META,
        'icbm' => NODEWORDS_META,
      ),
    ),
  );

  return $tags;
}

function hook_nodewords_tags_info_alter(&$tags_info) {
  if (isset($tags_info['abstract'])) {
    $tags_info['abstract']['label'] = t('New label for abstract');
  }
}

/**
 * Alter the meta tags content.
 *
 * @param &$tags
 *  The array of meta tags values.
 * @param $parameters
 *  An array of parameters. The currently defined are:
 *   * type - the type of object for the page to which the meta
 *     tags are associated.
 *   * ids - the array of IDs for the object associated with the page.
 *   * output - where the meta tags are being output; the parameter value can
 *     'head' or 'update index'.
 */
function hook_nodewords_tags_alter(&$tags, $parameters) {
  if (empty($output['abstract']) && $parameters['type'] == NODEWORDS_TYPE_PAGE) {
    $output['abstract'] = t('Node content');
  }
}

/**
 * Alter the string containing the meta tags output; it is called when the meta tags are already rendered.
 *
 * @param &$output
 *  The string to alter.
 * @param $parameters
 *  An array of parameters. The currently defined are:
 *   * type - the type of object for the page to which the meta
 *     tags are associated.
 *   * id - the ID for the object associated with the page.
 *   * output - where the meta tags are being output; the parameter value can
 *     'head' or 'update index'.
 */
function hook_nodewords_tags_output_alter(&$output, $parameters) {
  $bool = (
    variable_get('nodewords_add_dc_schema', FALSE) &&
    isset($parameters['output']) &&
    $parameters['output'] == 'head'
  );
  if ($bool) {
    $output = (
      '<link rel="schema.dc" href="http://purl.org/dc/elements/1.1/" />' . "\n" .
      $output
    );
  }
}

/**
 * The hook is used from the module to determinate the type of the object
 * associated with the currently viewed page (node, user, taxonomy term), and
 * the ID of the object.
 *
 * @param &$result
 *   the array used to write the result.
 * @param $arg
 *   the array as obtained from arg().
 */
function hook_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'user') {
    // User page paths: user/$uid.
    if (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_USER;
      $result['id'] = $arg[1];
    }
  }
}

