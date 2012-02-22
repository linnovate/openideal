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
  
  $output .=  "<ul class='menu'>\n<li>";
  foreach ($tree as $term) {
    $diffdepth = 0;
    if ($term->depth > $depth) {
      $output .=  "\n<ul>\n<li>";
      $depth = $term->depth;
      $num_at_depth = 0;
    }
    if ($term->depth < $depth) {
      $diffdepth = $depth - $term->depth;
      while ($diffdepth > 0) {
        $output .=  "</li>\n</ul>\n";
        $diffdepth--;
      }
      $depth = $term->depth;
    }
    if ($term->depth == $depth && $num_at_depth > 0) {
      $output .=  "</li>\n<li>";
    }
    $output .=  l($term->name, 'taxonomy/term/' . $term->tid);
    $num_at_depth++;
  }
  $output .=  "</li>\n</ul>\n";

  return $output;
}
