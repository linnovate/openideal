<?php

/**
 * @file graphael.tpl.php
 *
 * Theme template for gRaphael graphs.
 *
 * Available variables:
 * - $method: The gRaphael graph type to use. Example: 'bar'.
 * - $values: The values to be passed to $.graphael().
 * - $params: The params array to be passed to $.graphael().
 * - $extend: Added values that may be used by extending functionality.
 *
 * Note that it is the template's responsibility to add the graph to the Drupal
 * settings js. This is done in the template file rather than in a preprocess
 * function as it ensures that all other preprocessors have a chance to run
 * prior to the graph values being collected and added.
 */

$graph = array('method' => $method, 'values' => $values, 'params' => $params, 'extend' => $extend);
drupal_add_js(array('graphael' => array($attr['id'] => $graph)), 'setting');
?>
<div <?php print drupal_attributes($attr) ?>></div>
