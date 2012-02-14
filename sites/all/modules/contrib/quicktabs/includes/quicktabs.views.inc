<?php
// Id:$

/**
 * @file Add Views module hooks to Quicktabs.
 */
 
/**
 * Implementation of hook_views_plugins().
 */
function quicktabs_views_plugins() {
  $path = drupal_get_path('module', 'quicktabs');
  
  return array(
    'style' => array(
      'quicktabs' => array(
        'title' => t('Quicktabs'),
        'help' => t('Display view in Quicktabs.'),
        'handler' => 'quicktabs_style_plugin',
        'path' => "$path/includes",
        'theme' => 'quicktabs_view',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

function theme_quicktabs_views_render($view, $tabs, $qtid) {
  foreach ($tabs as $index => $tab) {
    if (is_array($tab['content'])) {
      $text = '';
      foreach ($tabs[$index]['content'] as $content) {
        $text .= '<div class="quicktabs-views-group">' . $content . '</div>';
      }
      $tabs[$index]['text'] = $text;
    }
    else {
      $tabs[$index]['text'] = $tabs[$index]['content'];
    }
    $tabs[$index]['type'] = 'freetext';
    unset($tabs[$index]['content']);
  }

  $quicktabs = array(
    'qtid' => $qtid,
    'ajax' => '0', // TODO: Support AJAX, set this to $view->use_ajax.
    'hide_empty_tabs' => '0',
    'default_tab' => '0',
    'style' => $view->style_options['tab_style'],
    'tabs' => $tabs,
  );

  return theme('quicktabs', $quicktabs);
}
