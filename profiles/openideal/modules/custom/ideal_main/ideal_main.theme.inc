<?php

/**
 * Category list for sidebar:
 * Show all terms from the "categories" taxonomy.
 */
function theme_ideal_category_list_block() {
  $voc = taxonomy_vocabulary_machine_name_load('categories');
  $tree = taxonomy_get_tree($voc->vid);
  $depth = 0;
  $num_at_depth = 0;
  $output = "";
  
  // create the top level container ul
  $output .=  "<ul class='category-menu level-0'>\n<li>";
  foreach ($tree as $term) {
    $diffdepth = 0;
    // if we're moving into a deeper level, add a new container ul
    if ($term->depth > $depth) {
      $output .=  "\n<ul class='level-" . $term->depth . "'>\n<li>";
      $depth = $term->depth;
      $num_at_depth = 0;
    }
    // if we're moving out of a deeper level, close the container ul
    if ($term->depth < $depth) {
      $diffdepth = $depth - $term->depth;
      while ($diffdepth > 0) {
        $output .=  "</li>\n</ul>\n";
        $diffdepth--;
      }
      $depth = $term->depth;
    }
    // if we're staying on the same level, move on to the next li
    if ($term->depth == $depth && $num_at_depth > 0) {
      $output .=  "</li>\n<li>";
    }
    // Here we print the actual term link
    $output .=  l($term->name, 'taxonomy/term/' . $term->tid);
//    $output .= " (" . ideal_count_term_ideas($term->tid) . ")";
    $output .= " (" . ideal_main_quicktabs_view_counter('ideas_by_category', 'block_1', $term->tid) . ")";
    $num_at_depth++;
  }
  // close the top level container ul
  $output .=  "</li>\n</ul>\n";

  return $output;
}
