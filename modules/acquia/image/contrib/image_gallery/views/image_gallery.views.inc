<?php
// $Id: image_gallery.views.inc,v 1.3.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * @file
 * Image Gallery views integration.
 *
 * A gallery is made up of two views:
 * - a view of nodes (the images)
 * - a view of terms (the child galleries)
 * The view of image nodes embeds the view of child gallery terms with a special
 * image gallery display plugin.
 * We provide the default views to make this, the display plugin, and a variety
 * of fields and relationships for the child gallery view.
 * The default views are:
 * - image_gallery
 * - image_gallery_terms
 *
 * There are two ways of getting the image gallery cover image:
 * - a plain field. This allows complex code like recursion (check the gallery itself,
 *   if empty, check the child galleries, etc)
 *   Plain field also allows us to grab the sort order set on the gallery images
 *   and get the first image from that order as the front.
 *   (combining the two options will be MONSTROUS!)
 * - a relationship
 *   This gets us onto the node table, so any fields can be used for the cover.
 *   Depth is possible too -- eg,
 *   "the newest image out of the gammery or its immediate children"
 *   However recursion is not possible.
 *
 * So we have two families of handlers:
 * - field handlers
 * - relationship handlers
 * Between these many ways of getting a cover image are possible. The handlers
 * are fairly generic, and further possibilities can be obtained just by
 * defining more fields with those handlers.
 */

/**
 * Implementation of hook_views_data_alter().
 * Add fields for image gallery (ie vocabulary terms) to the term_data table.
 */
function image_gallery_views_data_alter(&$data) {
  // ----------------------------------------------------------------------
  // Relationships for cover node.
  //
  // These allow any node field to be used to create the gallery cover node.
  // The limitation, however, is that either consider the immediate gallery,
  // or a flat pool of the gallery and descendants.
  // Note that a bug in Views -- http://drupal.org/node/380560 -- will cause
  // an error message when adding node fields on these relationships.
  $data['term_data']['image_gallery_cover_latest'] = array(
    'group' => t('Image gallery'),
    'relationship' => array(
      'title' => t('Latest image'),
      'label' => t('Cover image, latest'),
      'help' => t('Relate an image gallery to its most recently updated node (does not consider child galleries).'),
      'handler' => 'image_gallery_handler_relationship_gallery_cover',
      'base' => 'node',
      'field' => 'nid',
      'correlated field' => 'tid',
      'subquery order' => ' gallery_cover_node.created DESC ',
    ),
  );
  $data['term_data']['image_gallery_cover_oldest'] = array(
    'group' => t('Image gallery'),
    'relationship' => array(
      'title' => t('Oldest image'),
      'label' => t('Cover image, oldest'),
      'help' => t('Relate an image gallery to its oldest node (does not consider child galleries).'),
      'handler' => 'image_gallery_handler_relationship_gallery_cover',
      'base' => 'node',
      'field' => 'nid',
      'correlated field' => 'tid',
      'subquery order' => ' gallery_cover_node.created ASC ',
    ),
  );
  $data['term_data']['image_gallery_cover_node_title'] = array(
    'group' => t('Image gallery'),
    'relationship' => array(
      'title' => t('First image by title'),
      'label' => t('Cover image, first by title'),
      'help' => t('Relate an image gallery to its first node when sorted by title (does not consider child galleries).'),
      'handler' => 'image_gallery_handler_relationship_gallery_cover',
      'base' => 'node',
      'field' => 'nid',
      'correlated field' => 'tid',
      'subquery order' => ' gallery_cover_node.title ASC ',
    ),
  );

  // ----------------------------------------------------------------------
  // Simple fields.

  // Gallery count.
  $data['term_data']['image_gallery_count'] = array(
    'group' => t('Image gallery'),
    'field' => array(
      'title' => t('Count'),
      'help' => t('Count of items in a gallery.'),
      'handler' => 'image_gallery_handler_field_gallery_count',
    ),
  );

  // ----------------------------------------------------------------------
  // Fields for cover image.
  //
  // These use a combination of code and separate queries to get a cover node.
  // This makes them more powerful that using the relationship cover node,
  // as we can consider child galleries recursively rather than just
  // flattening all descendant galleries.
  // We can also do complex things such as grab the top-most node from the
  // gallery according to how the view for that gallery sorts them.
  // The downside however is that without a relationship, the fields here are
  // all you've got.
  // To add more fields, define them on term_data and optionally add handlers.
  // See image_gallery_handler_field_gallery_cover for more information.
  $data['term_data']['image_gallery_latest_thumbnail'] = array(
    'group' => t('Image gallery'),
    'field' => array(
      'title' => t('Latest image'),
      'help' => t('The most recently posted image in the gallery or its child galleries.'),
      'handler' => 'image_gallery_handler_field_gallery_cover_thumbnail',
      'order clause' => 'n.sticky DESC, n.created DESC',
    ),
  );
  $data['term_data']['image_gallery_latest_time'] = array(
    'group' => t('Image gallery'),
    'field' => array(
      'title' => t('Last updated time'),
      'help' => t('The time of the most recently posted image in the gallery or its child galleries.'),
      'handler' => 'image_gallery_handler_field_gallery_cover_latest_time',
      'order clause' => 'n.sticky DESC, n.created DESC',
    ),
  );
  $data['term_data']['image_gallery_first_title'] = array(
    'group' => t('Image gallery'),
    'field' => array(
      'title' => t('First image by title'),
      'help' => t('The first posted image in the gallery or its child galleries.'),
      'handler' => 'image_gallery_handler_field_gallery_cover_thumbnail',
      'order clause' => 'n.sticky DESC, n.title ASC',
    ),
  );
  $data['term_data']['image_gallery_oldest_thumbnail'] = array(
    'group' => t('Image gallery'),
    'field' => array(
      'title' => t('Oldest image'),
      'help' => t('The first posted image in the gallery or its child galleries.'),
      'handler' => 'image_gallery_handler_field_gallery_cover_thumbnail',
      'order clause' => 'n.sticky DESC, n.created ASC',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function image_gallery_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'image_gallery') . '/views',
    ),
    'handlers' => array(
      'image_gallery_handler_field_gallery_count' => array(
        'parent' => 'views_handler_field_taxonomy',
      ),
      'image_gallery_handler_field_gallery_cover' => array(
        'parent' => 'views_handler_field_taxonomy',
      ),
      'image_gallery_handler_field_gallery_cover_thumbnail' => array(
        'parent' => 'image_gallery_handler_field_gallery_cover',
      ),
      'image_gallery_handler_field_gallery_cover_latest_time' => array(
        'parent' => 'image_gallery_handler_field_gallery_cover',
      ),
      'image_gallery_handler_relationship_gallery_cover' => array(
        'parent' => 'views_handler_relationship',
      ),
      'image_gallery_plugin_display_image_gallery' => array(
        'parent' => 'views_plugin_display_page',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function image_gallery_views_plugins() {
  return array(
    'display' => array(
      /**
       * The image gallery display: like a page, but with a subgallery
       * list embedded above it.
       * Uses a handler rather than a tpl file.
       */
      'image_gallery' => array(
        'title' => t('Gallery page'),
        'help' => t('Display the view as a gallery of images, with a URL and menu links.'),
        'parent' => 'page',
        'handler' => 'image_gallery_plugin_display_image_gallery',
        'theme' => 'views_view',
        'uses hook menu' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('Page'),
        // @todo ?
        'help topic' => 'display-page',
        'path' => drupal_get_path('module', 'image_gallery') . '/views',
      ),
    ),
    'style' => array(
      /**
       * A list style for term lists.
       */
      'image_gallery_terms' => array(
        'title' => 'Subgallery list',
        'help' => t('Displays subgalleries in a formatted list.'),
        'parent' => 'list',
        'handler' => 'views_plugin_style_list',
        'theme path' => drupal_get_path('module', 'image_gallery') . '/views/theme',
        'theme file' => 'theme.inc',
        'theme' => 'image_gallery_view_image_gallery_terms',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-list',
      ),
    ),
  );
}

/**
 * Join handler for relationships that join with a subquery as the left field.
 * To use this join, set 'left query' in the definition as a subquery to go on
 * the left side of the JOIN condition, ie:
 *  LEFT JOIN node node_term_data ON ([YOUR SUBQUERY HERE]) = node_term_data.nid
 * This should ultimately be replaced with the stuff for general groupwise
 * max relationships in views: @see <http://drupal.org/node/470258>.
 */
class image_gallery_join_subquery extends views_join {
  // PHP 4 doesn't call constructors of the base class automatically from a
  // constructor of a derived class. It is your responsibility to propagate
  // the call to constructors upstream where appropriate.
  function construct($table = NULL, $left_table = NULL, $left_field = NULL, $field = NULL, $extra = array(), $type = 'LEFT') {
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);

    $this->left_query = $this->definition['left query'];
  }

  /**
   * Build the SQL for the join this object represents.
   */
  function join($table, &$query) {
    $left = $query->get_table_info($this->left_table);
    $output = " $this->type JOIN {" . $this->table . "} $table[alias] ON ($this->left_query) = $table[alias].$this->field";

    // Tack on the extra.
    if (isset($this->extra)) {
      if (is_array($this->extra)) {
        $extras = array();
        foreach ($this->extra as $info) {
          $extra = '';
          // Figure out the table name. Remember, only use aliases provided
          // if at all possible.
          $join_table = '';
          if (!array_key_exists('table', $info)) {
            $join_table = $table['alias'] . '.';
          }
          elseif (isset($info['table'])) {
            $join_table = $info['table'] . '.';
          }

          // And now deal with the value and the operator.  Set $q to
          // a single-quote for non-numeric values and the
          // empty-string for numeric values, then wrap all values in $q.
          $raw_value = $this->db_safe($info['value']);
          $q = (empty($info['numeric']) ? "'" : '');

          if (is_array($raw_value)) {
            $operator = !empty($info['operator']) ? $info['operator'] : 'IN';
            // Transform from IN() notation to = notation if just one value.
            if (count($raw_value) == 1) {
              $value = $q . array_shift($raw_value) . $q;
              $operator = $operator == 'NOT IN' ? '!=' : '=';
            }
            else {
              $value = "($q" . implode("$q, $q", $raw_value) . "$q)";
            }
          }
          else {
            $operator = !empty($info['operator']) ? $info['operator'] : '=';
            $value = "$q$raw_value$q";
          }
          $extras[] = "$join_table$info[field] $operator $value";
        }

        if ($extras) {
          if (count($extras) == 1) {
            $output .= ' AND ' . array_shift($extras);
          }
          else {
            $output .= ' AND (' . implode(' ' . $this->extra_type . ' ', $extras) . ')';
          }
        }
      }
      else if ($this->extra && is_string($this->extra)) {
        $output .= " AND ($this->extra)";
      }
    }
    return $output;
  }
}

