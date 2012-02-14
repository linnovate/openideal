<?php
// $Id: image_gallery.pages.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * @file
 * Contains menu callbacks for image_gallery pages,
 * ie galleries not made with views module.
 * This file is not loaded when Views is creating the gallery.
 */

/**
 * Image gallery callback, displays an image gallery
 */
function image_gallery_page($type = NULL, $tid = 0) {
  // Determine sort order.
  $join = $where = $order = '';
  $args = array();
  switch (variable_get('image_gallery_sort_order', IMAGE_GALLERY_SORT_CREATE_DESC)) {
    case IMAGE_GALLERY_SORT_CREATE_DESC:
      $order = 'ORDER BY n.sticky DESC, n.created DESC';
      break;

    case IMAGE_GALLERY_SORT_CREATE_ASC:
      $order = 'ORDER BY n.sticky DESC, n.created ASC';
      break;

    case IMAGE_GALLERY_SORT_FILENAME:
      $join = "INNER JOIN {image} i ON n.nid = i.nid INNER JOIN {files} f ON i.fid = f.fid";
      $where = "AND f.filename = '%s'";
      $args[] = IMAGE_ORIGINAL;
      $order = "ORDER BY n.sticky DESC, f.filepath ASC";
      break;

    case IMAGE_GALLERY_SORT_TITLE:
      $order = 'ORDER BY n.sticky DESC, n.title ASC';
      break;
  }

  $vid = _image_gallery_get_vid();
  $galleries = taxonomy_get_tree($vid, $tid, -1, 1);
  for ($i = 0; $i < count($galleries); $i++) {
    $galleries[$i]->count = taxonomy_term_count_nodes($galleries[$i]->tid, 'image');
    $tree = taxonomy_get_tree($vid, $galleries[$i]->tid, -1);
    $descendant_tids = array_merge(array($galleries[$i]->tid), array_map('_taxonomy_get_tid_from_term', $tree));
    // The values of $descendant_tids should be safe for raw inclusion in the
    // SQL since they're all loaded from integer fields in the database.
    $sql = "SELECT n.nid FROM {node} n INNER JOIN {term_node} tn ON n.nid = tn.nid $join WHERE tn.tid IN (" . implode(',', $descendant_tids) . ") AND n.type = 'image' AND n.status = 1 $where $order";
    if ($nid = db_result(db_query(db_rewrite_sql($sql), $args))) {
      $galleries[$i]->latest = node_load(array('nid' => $nid));
    }
  }

  $images = array();
  if ($tid) {
    // Allow images to be sorted in a useful order.
    $query = "SELECT n.nid FROM {term_node} t INNER JOIN {node} n ON t.nid = n.nid $join WHERE n.status = 1 AND n.type = 'image' AND t.tid = %d $where $order";
    $count_query = "SELECT COUNT(DISTINCT(n.nid)) FROM {term_node} t INNER JOIN {node} n ON t.nid = n.nid WHERE n.status = 1 AND n.type = 'image' AND t.tid = %d ";
    $args = array_merge(array($tid), $args);
    $result = pager_query(db_rewrite_sql($query), variable_get('image_images_per_page', 6), 0, db_rewrite_sql($count_query), $args);
    while ($node = db_fetch_object($result)) {
      $images[] = node_load(array('nid' => $node->nid));
    }

    $gallery = taxonomy_get_term($tid);
    $parents = taxonomy_get_parents($tid);

    $breadcrumb = array();
    $breadcrumb[] = l(t('Home'), NULL);
    $breadcrumb[] = l(t('Image galleries'), 'image');
    foreach ($parents as $parent) {
      // Add parents to breadcrumb navigation
      $breadcrumb[] = l($parent->name, 'image/tid/' . $parent->tid);
    }
    drupal_set_breadcrumb($breadcrumb);
    drupal_set_title($gallery->name);
  }

  return theme('image_gallery', $galleries, $images);
}

/**
 * Theme a gallery page
 */
function theme_image_gallery($galleries, $images) {
  drupal_add_css(drupal_get_path('module', 'image_gallery') . '/image_gallery.css');

  $size = image_get_sizes(IMAGE_THUMBNAIL);

  $content = '';
  if (count($galleries)) {
    $content .= '<ul class="galleries">';
    foreach ($galleries as $gallery) {
      $content .= '<li class="clear-block">';
      if ($gallery->count) {
        $content .= l(image_display($gallery->latest, IMAGE_THUMBNAIL), 'image/tid/' . $gallery->tid, array('html' => TRUE));
      }
      $content .= "<h3>" . l($gallery->name, 'image/tid/' . $gallery->tid) . "</h3>\n";
      $content .= '<div class="description">' . check_markup($gallery->description) . "</div>\n";
      $content .= '<p class="count">' . format_plural($gallery->count, 'There is 1 image in this gallery.', 'There are @count images in this gallery.') . "</p>\n";
      if (isset($gallery->latest) && $gallery->latest->changed) {
        $content .= '<p class="last">' . t('Last updated: %date', array('%date' => format_date($gallery->latest->changed))) . "</p>\n";
      }
      $content .= "</li>\n";
    }
    $content .= "</ul>\n";
  }

  if (!empty($images)) {
    $content .= '<ul class="images">';
    foreach ($images as $image) {
      $content .= theme('image_gallery_img', $image, $size);
    }
    $content .= "</ul>\n";
  }

  if ($pager = theme('pager', NULL, variable_get('image_images_per_page', 6), 0)) {
    $content .= $pager;
  }

  if (count($images) + count($galleries) == 0) {
    $content .= '<p class="count">' . format_plural(0, 'There is 1 image in this gallery.', 'There are @count images in this gallery.') . "</p>\n";
  }

  return $content;
}

/**
 * Theme a gallery image.
 */
function theme_image_gallery_img($image, $size) {
  $width = $size['width'];
  // We'll add height to keep thumbnails lined up.
  $height = $size['height'] + 75;

  $content = '<li';
  if ($image->sticky) {
    $content .= ' class="sticky"';
  }
  $content .= " style='height : {$height}px; width : {$width}px;'>\n";
  $content .= l(image_display($image, IMAGE_THUMBNAIL), 'node/' . $image->nid, array('html' => TRUE));
  $content .= '<h3>' . l($image->title, 'node/' . $image->nid) . '</h3>';
  if (variable_get('image_gallery_node_info', 0)) {
    $content .= '<div class="author">' . t('Posted by: !name', array('!name' => theme('username', $image))) . "</div>\n";
    if ($image->created > 0) {
      $content .= '<div class="date">' . format_date($image->created) . "</div>\n";
    }
  }
  $content .= "</li>\n";

  return $content;
}

