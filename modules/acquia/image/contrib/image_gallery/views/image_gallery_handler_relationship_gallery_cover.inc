<?php
// $Id: image_gallery_handler_relationship_gallery_cover.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/*
 * @file
 * Handlers for image gallery cover node.
 *
 * NOTE: a bug in Views -- http://drupal.org/node/380560 -- will cause
 * errors when adding node fields on this relationship.
 */

/**
 * Relationship handler for image gallery cover node.
 *
 * This joins to the node table from the term_data table, giving us a node
 * based on the given term. This is the gallery's cover node, which supplies the
 * image shown in the list of galleries.
 * Using this relationship allows the user to add any node fields to the view.
 * The downside is that while we can consider descendant galleries too, there is
 * no recursion: setting a depth of 1 means "give me the (best) image out of
 * the gallery and its immediate children" -- the images are considered
 * as a flat pool from which to pick.
 *
 * This handler uses a special join object to create the relationship join on
 * the value returned by a subquery.
 * For example:
 *  "given term tid 1, find the nid of the most recent node with that term"
 *  in other words, the most recent image in this gallery.
 *
 * The main part of the query is provided in this handler.
 * To use this, you need to give the subquery sort order, and the subquery's
 * correlated field (the field from the main views query on which to link) --
 * this is usually just 'tid'.
 *
 * Define these as follows:
 *  'relationship' => array(
 *    'title' => t('Latest image'),
 *    'label'  => t('Cover image, latest'),
 *    'help' => t('Relate an image gallery to its most recently updated node (does not consider child galleries).'),
 *    'handler' => 'image_gallery_handler_relationship_gallery_cover',
 *    'base'   => 'node',
 *    'field'  => 'nid',
 *    'correlated field' => 'tid',
 *    'subquery order' => ' gallery_cover_node.changed DESC ',
 *  )
 * This example gets you a cover image that is the latest changed node.
 *
 * If you need a different query, subclass this handler and override the
 * left_query() method to return your own subquery on which to join.
 */
class image_gallery_handler_relationship_gallery_cover extends views_handler_relationship {

  /**
   * Defines default values for options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['depth'] = array('default' => array(1));

    return $options;
  }

  /**
   * Extends the field's basic options with more image specific
   * options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['depth'] = array(
      '#type' => 'weight',
      '#title' => t('Depth'),
      '#default_value' => $this->options['depth'],
      '#description' => t('The depth will determine how deep to look into descendant galleries to cover nodes.' .
        'There is no recursion: so setting the depth to 1 will choose the best fitting node out of the pool of images in the gallery and its children taken all together.' .
        'Setting this to 0 will only consider the gallery itself.'
      ),
    );
  }

  /**
   * Return the subquery to use for the left side of the relationship join clause.
   *
   */
  function left_query() {
    $order = $this->definition['subquery order'];
    $field = $this->definition['correlated field'];
    $where_tid = $this->table_alias . '.' . $field;

    $subquery = "\nSELECT gallery_cover_node.nid FROM " .
      "{term_node} gallery_cover_term_node INNER JOIN {node} gallery_cover_node \n" .
      "ON gallery_cover_term_node.nid = gallery_cover_node.nid ";
    $where = "  WHERE gallery_cover_node.status = 1 AND " .
      "gallery_cover_term_node.tid = $where_tid ";

    // Depth: this is shamelessly ripped from views_handler_argument_term_node_tid_depth
    if ($this->options['depth'] > 0) {
      $subquery .= "    LEFT JOIN {term_hierarchy} th ON th.tid = gallery_cover_term_node.tid\n";
      $last = "th";
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $subquery .= "    LEFT JOIN {term_hierarchy} th$count ON $last.parent = th$count.tid\n";
        $where .= "\n OR th$count.tid = $where_tid\n";
        $last = "th$count";
      }
    }
    else if ($this->options['depth'] < 0) {
      $last = "tn";
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $subquery .= "    LEFT JOIN {term_hierarchy} th$count ON $last.tid = th$count.parent\n";
        $where .= "\n OR th$count.tid = $where_tid\n";
        $last = "th$count";
      }
    }
    $subquery = "$subquery $where ORDER BY $order LIMIT 1";

    return $subquery;
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);
    $base_field = empty($this->definition['base field']) ? $table_data['table']['base']['field'] : $this->definition['base field'];

    $this->ensure_my_table();

    $def = $this->definition;
    $def['table'] = $this->definition['base'];
    $def['field'] = $base_field;
    $def['left_table'] = $this->table_alias;
    $def['left_field'] = $this->field;
    if (!empty($this->options['required'])) {
      $def['type'] = 'INNER';
    }

    if (!empty($this->definition['left query'])) {
      $def['left query'] = $this->definition['left query'];
    }
    else {
      $def['left query'] = $this->left_query();
    }

    if (!empty($def['join_handler']) && class_exists($def['join_handler'])) {
      $join = new $def['join_handler'];
    }
    else {
      $join = new image_gallery_join_subquery();
    }

    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}

/**
 * Subclass example.
 */
/*
class image_gallery_handler_relationship_gallery_cover_example extends views_handler_relationship {
  function left_query($field) {
    $order = $this->definition['left query order'];
    $sub_query = 'SELECT gallery_cover_node.nid FROM ' .
      '{term_node} gallery_cover_term_node INNER JOIN ' .
      ' {node} gallery_cover_node ' .
      'ON gallery_cover_term_node.nid = gallery_cover_node.nid ' .
      'WHERE gallery_cover_node.status = 1 AND ' .
      'gallery_cover_term_node.tid = ' . $field . ' ' .
      ORDER BY $order .
      'LIMIT 1';

    return $sub_query;
  }
*/

