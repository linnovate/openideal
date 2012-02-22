<?php

/**
 * Theme function for the three main buttons
 * @return HTML string
 */
function theme_ideal_three_main_buttons($variables) {
  // TODO Number of parameters in this theme funcion does not match number of parameters found in hook_theme.
//  popups_add_popups(
//	   array(
//    //extend listing - reload.  
//    '.user-popup' => array(
//      'updateMethod' => 'reload',
//    ),
//    '.flag-deal-deliverd .flag' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/deals_in_progress',
//      'doneTest' => 'my_marketplace/deals_in_progress',
//    ),
//    '.delivery_approval a' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/completed_deals',
//      'doneTest' => '/',
//    ),
//    '.cancel_bid a' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/cancelled_bids',
//    ),
//    '.flag-discarded .flag-action' => array(
//      'updateMethod' => 'reload',
//    ),
//    '.flag-extended .flag-action' => array(
//      'updateMethod' => 'reload',
//    ),
//    //cancel listing - redirect to my_marketplace/cancelled_listings.
//    '#block-cliconomy-listing_actions .cancel a' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/cancelled_listings',
//    ),
//    //cancel bid - redirect to my_marketplace/cancelled_bids.
//    '.cancel-bid' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/cancelled_bids',
//    ),
//    //bid-confirm  - reload.
//    '.bid-confirm' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/my_bids',
//    ),
//    //no-credit  - reload.
//    '.no-credit' => array(
//      'updateMethod' => 'reload',
//    ),
//    //winning biddeer  - redirect to my_marketplace/deals_in_progress.
//    '.flag-selected-bid .flag-action' => array(
//      'updateMethod' => 'redirect',
//      'updateSource' => 'my_marketplace/deals_in_progress',
//      'doneTest' => 'my_marketplace/deals_in_progress',
//    ),
//  )
//	 );
  global $user;
  $output = '<div class="three-main-buttons">';
  if ($user->uid == 0) { // Send anonymous to log-in
    $output .= '<span class="new-idea">' . l(t('New Idea1'), 'user/login', array('attributes' => array('class' => 'popups'))) . '</span>';
  }
  else {
    $output .= '<span class="new-idea">' . l(t('New Idea1'), 'node/add/idea', array('attributes' => array('class' => 'user-popup'))) . '</span>';
  }
  $output .= '<span class="all-idea">' . l(t('Vote and Comment'), 'ideas') . '</span>';
  $output .= '<span class="in-action">' . l(t('Ideas in action'), 'in-action') . '</span>';
  $output .= '</div>';
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_head_to_head_block($variables) {
  $title = $variables['0'];
  $first = $variables['1'];
  $chart = $variables['2'];
  $second = $variables['3'];
  // TODO Number of parameters in this theme funcion does not match number of parameters found in hook_theme.
  $output = '';
  $output .= '<div class="block-text">' . $title . '</div>';
  $output .= '<table>';
  $output .= '<tr>';
  $output .= '<td class="first">' . $first . '</td>';
  $output .= '<td>' . $chart . '</td>';
  $output .= '<td class="second">' . $second . '</td>';
  $output .= '</tr>';
  $output .= '</table>';

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_head_to_head_chart($variables) {
  $score1 = $variables['0'];
  $score2 = $variables['1'];
  // TODO Number of parameters in this theme funcion does not match number of parameters found in hook_theme.
  return '<div class="charts"><span class="chart chart1" style="padding-top:' . $h1 . 'px">' . $score1 . '</span>' . '<span class="chart chart2" style="padding-top:' . $h2 . 'px">' . $score2 . '</span></div>';
}

/*
 * Override filter.module's theme_filter_tips() function to disable tips display.
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ideal_filter_tips($tips, $long = FALSE, $extra = '') {
  return '';
}

/*
 * Override filter.module's theme_filter_tipsmore_info() function to disable tips display.
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ideal_filter_tips_more_info() {
  return '';
}

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