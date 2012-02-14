<?php
// $Id: page_title-admin-settings-form.tpl.php,v 1.1.2.5 2009/10/31 22:18:54 njt1982 Exp $

/**
 * @file
 * Tempalte file for the admin settings form. Displays configuration in a neat table
 */

$rows = array();

foreach (element_children($form['pattern']) as $key) {
  $title = array(
    '#type' => 'item',
    '#title' => $form['pattern'][$key]['#title'],
    '#required' => $form['pattern'][$key]['#required'],
  );
  unset($form['pattern'][$key]['#title']);

  $row = array(
    drupal_render($title),
    drupal_render($form['scope'][$key]),
    drupal_render($form['pattern'][$key]),
    isset($form['showfield'][$key .'_showfield']) ? drupal_render($form['showfield'][$key .'_showfield']) : '',
  );

  $rows[] = $row;
}

print theme('table', array(t('Page Type'), t('Token Scope'), t('Pattern'), t('Show Field')), $rows);

print drupal_render($form);
