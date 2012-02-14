<?php
// $Id: template.php,v 1.1.2.3 2009/07/18 17:48:55 dvessel Exp $
function ideal_protest_theme() {
  return array(
    // The form ID.
    'user_login' => array(
      // Forms always take the form argument.
      'arguments' => array('form' => NULL),
    ),
  );
}
function ideal_protest_user_login($form) {
  $output = '';
  // Print out the $form array to see all the elements we are working with.
  $output .= t('If you have a facebook account you can easily login/register using the facebook button bellow. ');
  $output .= drupal_render($form['fbconnect_button'] );
  $output .='<div class="log-text">';
  $output .= t('Don\'t have a facebook account click here to !login or here to !register.', array(
          '!login' => l(t('login'), $_GET['q'], array('fragment' => 'go-to-login', 'attributes' => array('class' => 'login-link'))),
          '!register' => l(t('register'), 'user/register'),
  ));
  $output .='</div>';
  // Once I know which part of the array I'm after we can change it.

  // You can use the normal Form API structure to change things, like so:
  // Change the Username field title to Login ID.
  

  // Make sure you call a drupal_render() on the entire $form to make sure you
  // still output all of the elements (particularly hidden ones needed
  // for the form to function properly.)
  $output .= '<a id="go-to-login"/>';
  $output .= '<div class="regular-login">';
  $output .= drupal_render($form);
  $output .= '</div>';
  return $output;
}

/**
 * Implementation of theme_views_mini_pager().
 * Implementing a looping pager for front page focused ideas block.
 */
function ideal_protest_views_mini_pager($tags = array(), $limit = 10, $element = 0, $parameters = array(), $quantity = 9) {
  global $pager_page_array, $pager_total;
  $view = views_get_current_view();
  $view_name = $view->name; 
  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.


  $li_previous = theme('pager_previous', (isset($tags[1]) ? $tags[1] : t('‹‹')), $limit, $element, 1, $parameters);
  if (empty($li_previous)) {
    $li_previous = "&nbsp;";
  }

  $li_next = theme('pager_next', (isset($tags[3]) ? $tags[3] : t('››')), $limit, $element, 1, $parameters);
  if (empty($li_next) && $view_name == 'idea_in_focus') {
    $li_next = theme('pager_first', (isset($tags[0]) ? $tags[0] : '1'), $limit, $element, $parameters);
  }
  elseif (empty($li_next)) {
    $li_next = "&nbsp;";
  }
  
  if ($pager_total[$element] > 1) {
    $items[] = array(
      'class' => 'pager-previous',
      'data' => $li_previous,
    );

    $items[] = array(
      'class' => 'pager-current',
      'data' => t('@current of @max', array('@current' => $pager_current, '@max' => $pager_max)),
    );

    $items[] = array(
      'class' => 'pager-next',
      'data' => $li_next,
    );
    return theme('item_list', $items, NULL, 'ul', array('class' => 'pager'));
  }
}