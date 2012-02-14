<?php
// $Id: bookblock.admin.inc,v 1.1 2010/07/01 12:31:50 mcjim Exp $

/**
 * @file
 * Admin page callbacks for the booknavigationblocks module.
 */


/**
 * Builds and returns the bookblock settings form.
 *
 * @ingroup forms
 */
function bookblock_admin_settings() {
  $books = book_get_books();
  if ($books) {
    foreach ($books as $book) {
      if (!$book['has_children']) {
        $book['title'] = $book['title'] .' <em>('. t('no child pages') .')</em>';
      }
      $books[$book['nid']] = $book['title'];
    }
    $form['bookblock_books'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Generate a navigation block for each of the following books'),
      '#default_value' => variable_get('bookblock_books', array()),
      '#options' => $books,
      '#description' => t('For each book that you select, a separate navigation block will be created. You can enable these blocks on the blocks administration page or use the context module.'),
    );
    $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);
    return system_settings_form($form);
  }
}