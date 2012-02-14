<?php
// $Id: image_gallery_handler_field_gallery_cover.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Parent class for field handlers that gives us a gallery cover.
 *
 * This takes care of defining the descendant and depth options, getting and
 * storing a query, and running the query to get the cover nid.
 *
 * There are several ways to use this class:
 * - Define the ordering in the field definition, with an 'order clause' key.
 *   This gets tacked onto the fairly simple node query defined in this handler.
 * - Define your own query in the field definition with a 'basic query' key.
 * - Subclass this handler and create your own query by overriding
 *   get_cover_node_nid_query(). This allows some pretty complex stuff, such as
 *   grabbing the query from another view.
 */
class image_gallery_handler_field_gallery_cover extends views_handler_field_taxonomy {

  /**
   * Constructor to provide additional fields to add.
   */
  function construct() {
    parent::construct();

    $this->additional_fields['tid'] = 'tid';
  }

  function init(&$view, $options) {
    parent::init($view, $options);

    // Store the image gallery vocabulary ID.
    $this->vid = _image_gallery_get_vid();

    // The name of a key that is safe to use in $values to stash our nid.
    // This is just a dummy that should be overridden.
    // Groups of handlers that use the same query (eg, latest image and last
    // update time) should override this to a safe key that they can share.
    // This avoids running the query/ies to get the cover nid multiple times for
    // each row of the view.
    // TODO: doesn't work and probably can't ever, because if the user adds more
    // than one cover image, we can't reliably create a unique key here that
    // another field can know to find.
    /*
    if ($this->definition['cover group']) {
      $this->cover_stash =  $this->definition['cover group'];
    }
    else {
      $this->cover_stash =  'image_gallery_cover';
    }
     */
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['depth'] = array('default' => 0);
    $options['descendants'] = array('default' => 'single');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['descendants'] = array(
      '#type' => 'radios',
      '#title' => t('Descendant handling'),
      // Child handlers should replace this text with something more relevant
      // to what they do.
      '#description' => theme('advanced_help_topic', 'image_gallery', 'descendants') .
      t('How to get data from subgalleries: either only select from this gallery itself, or consider subgalleries all together, or recurse into subgalleries if the gallery itself is empty.'),
      '#options' => array(
        'single' => t('Only select from this gallery'),
        'flat' => t('Consider subgalleries, flattened.'),
        'recurse' => t('Consider subgalleries, recursively. (Warning: this can produce many queries per row if your parent galleries are empty!).'),
      ),
      '#default_value' => $this->options['descendants'],
    );

    $form['depth'] = array(
      '#type' => 'select',
      '#title' => t('Depth'),
      '#options' => array_merge(range(0, 10), array('all' => 'Unlimited')),
      '#default_value' => $this->options['depth'],
      '#description' => t('How deeply to go into subgalleries if one of the descendant options is selected above.'),
    );
  }

  /**
   * Get the cover nid for the current tid.
   *
   * This makes up our query from the field definition and handler functions,
   * and then runs it according to the descendant and depth options given
   * for the field.
   */
  function get_cover_node_nid($values) {
    // The term id of the current gallery.
    // This comes straight from the database row so should be safe for
    // inclusion in a query.
    $tid = $values->tid;

    // Get the basic query but only once for all rows of the view.
    // This can be set in the handler definition, or in the handler class's
    // get_cover_node_nid_query function for more complex cases.
    // The handler definition takes priority over the function.
    // This is stored in the handler object so we only have to get it
    // once for the whole view.
    if (!isset($this->basic_query)) {
      if (isset($this->definition['basic query'])) {
        $this->basic_query = $this->definition['basic query'];
      }
      else {
        $this->basic_query = $this->get_cover_node_nid_query();
      }
    }

    // If we don't have a query, return NULL now to prevent SQL errors.
    if (is_null($this->basic_query)) {
      return NULL;
    }

    // Add the definition's ORDER clause, if there is one.
    if ($this->definition['order clause']) {
      $this->basic_query = str_replace('***ORDER_CLAUSE***', $this->definition['order clause'], $this->basic_query);
    }

    // Look at the options set for this handler.
    $depth = $this->options['depth'];
    $descendants_method = $this->options['descendants'];

    // Depth of 0 is the same as 'single'.
    if ($depth === 0) {
      $descendants_method = 'single';
    }

    // Add our WHERE clause.
    switch ($descendants_method) {
      // Plain: only consider this gallery.
      // Recursive: only consider this gallery (we'll recurse later).
      case 'single':
      case 'recurse':
        $where_clause = "tn.tid = $tid ";
        $query = str_replace('***WHERE_CLAUSE***', $where_clause, $this->basic_query);
        break;

      // Flat: all descendant galleries considered for the cover query.
      case 'flat':
        if ($depth == 'all') {
          $tree = taxonomy_get_tree($this->vid, $tid);
        }
        else {
          $tree = taxonomy_get_tree($this->vid, $tid, -1, $depth);
        }
        $descendant_tids = array_merge(array($tid), array_map('_taxonomy_get_tid_from_term', $tree));
        // The values of $descendant_tids should be safe for raw inclusion in the
        // SQL since they're all loaded from integer fields in the database.
        $where_clause = 'tn.tid IN (' . implode(',', $descendant_tids) . ')';
        $query = str_replace('***WHERE_CLAUSE***', $where_clause, $this->basic_query);
        break;
    }

    // Run the query to get the cover nid.
    if ($nid = db_result(db_query_range(db_rewrite_sql($query), '', 0, 1))) {
      return $nid;
    }

    // We have no nid: the gallery is empty. Go into descendant galleries if the
    // recurse option is set.
    if ($this->options['descendants'] == 'recurse' && $this->options['depth'] > 0) {
      $nid = $this->get_cover_node_nid_recurse($tid, 1);
      return $nid;
    }
  }

  /**
   * Recursively query descendant tids until we get a cover nid.
   *
   * Warning! This is probably quite intensive, as it drills down the term
   * hierarchy doing a query on each term until it finds a node -- and this is
   * done for EVERY row of the main gallery term view!
   *
   * @param $tid The tid whose children we should investigate for a cover nid.
   * @param $recurse The current recursion depth.
   */
  function get_cover_node_nid_recurse($tid, $recurse) {
    $tree = taxonomy_get_tree($this->vid, $tid, -1, 1);
    foreach ($tree as $term) {
      $where_clause = 'tn.tid = ' . $term->tid;
      $query = str_replace('***WHERE_CLAUSE***', $where_clause, $this->basic_query);
      if ($nid = db_result(db_query_range(db_rewrite_sql($query), '', 0, 1))) {
        return $nid;
      }

      // Recurse into this gallery, if we may.
      if ($this->options['depth'] > $recurse) {
        if ($nid = $this->get_cover_node_nid_recurse($term->tid, $recurse + 1)) {
          return $nid;
        }
      }
    }
  }

  /**
   * The query this handler should use to gets its cover node id, unless
   * the field definition specifies one.
   *
   * The return string should include a placeholder for the WHERE clause
   * as in this basic case. This is where the tid(s) will go.
   * The order clause should be supplied by the handler definition.
   * Warning: this is NOT sophisticated stuff. The WHERE clause inserted into
   * this string will expect to find a row "tn.tid" in the query.
   */
  function get_cover_node_nid_query() {
    return 'SELECT n.nid from {node} n INNER JOIN {term_node} tn ON n.nid = tn.nid ' .
        "WHERE ***WHERE_CLAUSE*** AND n.type = 'image' AND n.status = 1 " .
        'ORDER BY ***ORDER_CLAUSE***';
  }

  /**
   * Override query() so we don't query: fake field.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Returns field html. Just a dummy to inherit.
   */
  function render($values) {
    $nid = $this->get_cover_node_nid($values);
    return $nid;
  }
}

