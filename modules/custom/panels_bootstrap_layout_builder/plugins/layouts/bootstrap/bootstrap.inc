<?php

/**
 * Implementation of hook_panels_layouts()
 */
// Plugin definition
$plugin = array(
  'title' => t('Twitter Bootstrap 3'),
  'category' => t('Builders'),
  'icon' => 'bootstrap.png',
  'theme' => 'panels_bootstrap',
  'admin theme' => 'panels_bootstrap_admin',
  'css' => 'bootstrap.css',
  'admin css' => 'bootstrap-admin.css',
  'settings form' => 'panels_bootstrap_settings_form',
  'settings submit' => 'panels_bootstrap_settings_submit',
  'settings validate' => 'panels_bootstrap_settings_validate',
  'regions function' => 'panels_bootstrap_panels',
  'hook menu' => 'panels_bootstrap_menu',

  // Reuisable layout Builder specific directives
  'builder' => TRUE,
  'builder tab title' => 'Add bootstrap layout', // menu so translated elsewhere

  'get child' => 'panels_bootstrap_get_sublayout',
  'get children' => 'panels_bootstrap_get_sublayouts',

  // Define ajax callbacks
  'ajax' => array(
    'settings' => 'panels_ajax_bootstrap_edit_settings',
    'add' => 'panels_ajax_bootstrap_edit_add',
    'remove' => 'panels_ajax_bootstrap_edit_remove',
    'resize' => 'panels_ajax_bootstrap_edit_resize',
    'reuse' => 'panels_ajax_bootstrap_edit_reuse',
  ),
);

/**
 * Merge the main bootstrap plugin with a layout to create a sub plugin.
 *
 * This is used for both panels_bootstrap_get_sublayout and
 * panels_bootstrap_get_sublayouts.
 */
function panels_bootstrap_merge_plugin($plugin, $layout) {
  $plugin['name'] = 'bootstrap:' . $layout->name;
  $plugin['category'] = !empty($layout->category) ? check_plain($layout->category) : t('Miscellaneous');
  $plugin['title'] = check_plain($layout->admin_title);
  $plugin['description'] = check_plain($layout->admin_description);
  $plugin['layout'] = $layout;
  $plugin['builder'] = FALSE;
  $plugin['builder tab title'] = NULL;
  return $plugin;
}

/**
 * Callback to provide a single stored bootstrap layout.
 */
function panels_bootstrap_get_sublayout($plugin, $layout_name, $sublayout_name) {
  // Do not worry about caching; Panels is handling that for us.
  ctools_include('export');
  $item = ctools_export_crud_load('panels_layout', $sublayout_name);
  if ($item) {
    return panels_bootstrap_merge_plugin($plugin, $item);
  }
}

/**
 * Callback to provide all stored bootstrap layouts.
 */
function panels_bootstrap_get_sublayouts($plugin, $layout_name) {
  $layouts[$layout_name] = $plugin;
  ctools_include('export');
  $items = ctools_export_load_object('panels_layout', 'conditions', array('plugin' => 'bootstrap'));
  foreach ($items as $name => $item) {
    $layouts['bootstrap:' . $name] = panels_bootstrap_merge_plugin($plugin, $item);
  }

  return $layouts;
}

/**
 * Convert settings from old style to new, or provide defaults for
 * empty settings.
 * @param <type> $settings
 */
function panels_bootstrap_convert_settings(&$settings, &$layout) {
  // This indicates that this is a layout that they used the checkbox
  // on. The layout is still 'bootstrap' but it's actually pointing
  // to another stored one and we have to load it.
  if (!empty($settings['layout'])) {
    $layout = panels_get_layout('bootstrap:' . $settings['layout']);
  }

  if (!empty($layout['layout'])) {
    $settings = $layout['layout']->settings;
    if ($settings) {
      return $settings;
    }
  }

  if (empty($settings)) {
    // set up a default
    $settings = array(
      'items' => array(
        // The 'canvas' is a special row that does not get rendered
        // normally, but is used to contain the columns.
        'canvas' => array(
          'type' => 'container',
          'contains' => 'row',
          'children' => array('main'),
          'parent' => NULL,
        ),
        'main' => array(
          'type' => 'row',
          'children' => array('main-column'),
          'parent' => 'canvas',
        ),
        'main-column' => array(
          'type' => 'column',
          'contains' => 'region',
          'bootstrap_class' => array(
            'xs' => 'col-xs-12',
            'sm' => 'col-sm-12',
            'md' => 'col-md-12',
            'lg' => 'col-lg-12'
          ),					
          'children' => array('center'),
          'parent' => 'main',
        ),
        'center' => array(
          'type' => 'region',
          'title' => t('Center'),
          'parent' => 'main-column',
        ),
      ),
    );
  }
}

/**
 * Define the actual list of columns and rows for this bootstrap panel.
 */
function panels_bootstrap_panels($display, $settings, $layout) {
  $items = array();
  panels_bootstrap_convert_settings($settings, $layout);
  foreach ($settings['items'] as $id => $item) {
    // Remove garbage values.
    if (!isset($item['type'])) {
      unset($items[$id]);
    }
    else if ($item['type'] == 'region') {
      $items[$id] = $item['title'];
    }
  }

  return $items;
}

/**
 * Create a renderer object.
 *
 * The renderer object contains data that is passed around from function
 * to function allowing us to render our CSS and HTML easily.
 *
 * @todo Convert the functions to methods and make this properly OO.
 */
function panels_bootstrap_create_renderer($admin, $css_id, $content, $settings, &$display, $layout, $handler) {
  $renderer = new stdClass;
  $renderer->settings = $settings;
  $renderer->content = $content;
  $renderer->css_id = $css_id;
  $renderer->did = &$display->did;
  if ($admin) {
    // always scale in admin mode.
    $renderer->scale_base = 99.0;
  }
  $renderer->id_str = $css_id ? 'id="' . $css_id . '"' : '';
  $renderer->admin = $admin;
  $renderer->handler = $handler;

  // Set up basic classes for all of our components.
  $renderer->name                    = !empty($layout['layout']) ? $layout['layout']->name : $display->did;
  $renderer->base_class              = $renderer->name;
  $renderer->item_class['column']    = 'panels-bootstrap-column';
  $renderer->item_class['container'] = 'panels-bootstrap-container';	
  $renderer->item_class['row']       = 'panels-bootstrap-row';
  $renderer->item_class['region']    = 'panels-bootstrap-region';
  $renderer->base['canvas']          = 'panels-bootstrap-' . $renderer->base_class;

  // Override these if selected from the UI and not in admin mode.
  if (!$admin) {
    if (!empty($settings['items']['canvas']['class'])) {
      $renderer->base_class = $settings['items']['canvas']['class'];
      $renderer->base['canvas'] = $renderer->base_class;
    }
    if (!empty($settings['items']['canvas']['column_class'])) {
      $renderer->item_class['column'] = $settings['items']['canvas']['column_class'];
    }
    if (!empty($settings['items']['canvas']['row_class'])) {
      $renderer->item_class['row'] = $settings['items']['canvas']['row_class'];
    }
    if (!empty($settings['items']['canvas']['region_class'])) {
      $renderer->item_class['region'] = $settings['items']['canvas']['region_class'];
    }
    if (!empty($settings['items']['canvas']['container_class'])) {
      $renderer->item_class['container'] = $settings['items']['canvas']['container_class'];
    }		
  }

  // Make some appended classes so it's easier to reference them.
  $renderer->base['column']    = $renderer->item_class['column'] . '-' . $renderer->base_class;
  $renderer->base['row']       = $renderer->item_class['row'] . '-' . $renderer->base_class;
  $renderer->base['region']    = $renderer->item_class['region'] . '-' . $renderer->base_class;
  $renderer->base['container'] = $renderer->item_class['container'] . '-' . $renderer->base_class;
	
  if ($renderer->name != 'new') {
    // Use v2 to guarantee all CSS gets regenerated to account for changes in
    // how some divs will be rendered.
    $renderer->css_cache_name = 'bootstrapv2:' . $renderer->name;
    if ($admin) {
      ctools_include('css');
      ctools_css_clear($renderer->css_cache_name);
    }
  }
  return $renderer;
}

/**
 * Draw the bootstrap layout.
 */
function theme_panels_bootstrap($vars) {
  $css_id = $vars['css_id'];
  $content = $vars['content'];
  $settings = $vars['settings'];
  $display = $vars['display'];
  $layout = $vars['layout'];
  $handler = $vars['renderer'];

  panels_bootstrap_convert_settings($settings, $layout);
	drupal_add_css(drupal_get_path('module', 'panels_bootstrap_layout_builder') . '/plugins/layouts/bootstrap/bootstrap.css');

  $renderer = panels_bootstrap_create_renderer(FALSE, $css_id, $content, $settings, $display, $layout, $handler);
	if(!empty($settings['items']['canvas']['container_class_add'])) {
		$class = ' container';
	} else {
		$class = '';
	}
  $output = '<div class="panel-bootstrap ' . $renderer->base['canvas'] . $class .'"' . $renderer->id_str .">\n";
  $output .= panels_bootstrap_render_items($renderer, $settings['items']['canvas']['children'], $renderer->base['canvas']);
  $output .= "</div>\n";

  return $output;
}

/**
 * Draw the bootstrap layout.
 */
function theme_panels_bootstrap_admin($vars) {
  $css_id = $vars['css_id'];
  $content = $vars['content'];
  $settings = $vars['settings'];
  $display = $vars['display'];
  $layout = $vars['layout'];
  $handler = $vars['renderer'];

  if ($path = libraries_get_path('bootstrap')) {
    drupal_add_css($path . '/css/bootstrap.min.css');
    drupal_add_css($path . '/css/bootstrap-theme.min.css');
  }

  // We never draw stored bootstrap layouts in admin mode; they must be edited
  // from the stored layout UI at that point.
  if (!empty($layout['layout'])) {
    return theme_panels_bootstrap(array('css_id' => $css_id, 'content' => $content, 'settings' => $settings, 'display' => $display, 'layout' => $layout, 'renderer' => $handler));
  }

  panels_bootstrap_convert_settings($settings, $layout);
  $renderer = panels_bootstrap_create_renderer(TRUE, $css_id, $content, $settings, $display, $layout, $handler);

  if (empty($display->editing_layout)) {
    $output = '<input type="submit" id="panels-bootstrap-toggle-layout" class="form-submit" value ="' .
      t('Show layout designer') . '">';
    if (user_access('administer panels layouts')) {
      $output .= '<input type="hidden" class="panels-bootstrap-reuse-layout-url" value="' . url($handler->get_url('layout', 'reuse'), array('absolute' => TRUE)) . '">';
      $output .= '<input type="submit" id="panels-bootstrap-reuse-layout" class="form-submit ctools-use-modal" value ="' .
        t('Reuse layout') . '">';
    }
    $output .= "<div class=\"panel-bootstrap " . $renderer->base['canvas'] . " clearfix panel-bootstrap-admin panel-bootstrap-no-edit-layout\" $renderer->id_str>\n";
  }
  else {
    $output = "<div class=\"panel-bootstrap " . $renderer->base['canvas'] . " clearfix panel-bootstrap-admin panel-bootstrap-edit-layout\" $renderer->id_str>\n";
  }

  $output .= "<div class=\"row panel-bootstrap-inside " . $renderer->base['canvas'] . "-inside \">\n";

  $content = panels_bootstrap_render_items($renderer, $settings['items']['canvas']['children'], $renderer->base['row'] . '-canvas');
  $output .= panels_bootstrap_render_item($renderer, $settings['items']['canvas'], $content, 'canvas', 0, 0, TRUE);

  // Wrap the whole thing up nice and snug
  $output .= "</div>\n</div>\n";

  drupal_add_js($layout['path'] . '/bootstrap-admin.js');
  drupal_add_js(array('bootstrap' => array('resize' => url($handler->get_url('layout', 'resize'), array('absolute' => TRUE)))), 'setting');
  return $output;
}

/**
 * Render a piece of a bootstrap layout.
 */
function panels_bootstrap_render_items($renderer, $list, $owner_id) {
  $output = '';
  $groups = array('left' => '', 'middle' => '', 'right' => '');
  $max = count($list) - 1;
  $prev = NULL;

  foreach ($list as $position => $id) {
    $item = $renderer->settings['items'][$id];
    $location = isset($renderer->positions[$id]) ? $renderer->positions[$id] : 'middle';

    switch ($item['type']) {
			case 'container':
        $content = panels_bootstrap_render_items($renderer, $item['children'], $renderer->base['container'] . '-' . $id);
        $groups[$location] .= panels_bootstrap_render_item($renderer, $item, $content, $id, $position, $max);
				break;
      case 'row':
        $content = panels_bootstrap_render_items($renderer, $item['children'], $renderer->base['row'] . '-' . $id);
        $groups[$location] .= panels_bootstrap_render_item($renderer, $item, $content, $id, $position, $max, TRUE);
        break;
      case 'column':
        $content = panels_bootstrap_render_items($renderer, $item['children'], $renderer->base['column'] . '-' . $id);
        $groups[$location] .= panels_bootstrap_render_item($renderer, $item, $content, $id, $position, $max);
        break;
      case 'region':
        $content = isset($renderer->content[$id]) ? $renderer->content[$id] : "&nbsp;";
        $groups[$location] .= panels_bootstrap_render_item($renderer, $item, $content, $id, $position, $max);
        break;
    }
    $prev = $id;
  }

  $group_count = count(array_filter($groups));

  // Render each group. We only render the group div if we're in admin mode
  // or if there are multiple groups.
  foreach ($groups as $position => $content) {
    if (!empty($content) || $renderer->admin) {
      if ($group_count > 1 || $renderer->admin) {
        $output .= $content;
      }
      else {
        $output .= $content;
      }
    }
  }

  return $output;
}

/**
 * Render a column in the bootstrap layout.
 */
function panels_bootstrap_render_item($renderer, $item, $content, $id, $position, $max, $clear = FALSE) {
  $base = $renderer->item_class[$item['type']];
  $output = '';
  $item_class = '';
	
	switch($item['type']) {
		case 'container':
			if(!empty($item['container_class_add'])) $item_class .= 'container ';
			if(!empty($item['container_full_class_add'])) $item_class .= 'container-full ';			
			if(!empty($item['class'])) $item_class .= check_plain($item['class']).' ';
			if(!empty($base)) $item_class .= $base.' ';								
			break;		
		case 'row':
			$item_class .= 'row ';
			if(!empty($item['class'])) $item_class .= check_plain($item['class']).' ';
			if(!empty($base)) $item_class .= $base.' ';					
			break;
		case 'column':
			$item_class .= 'column ';
			if(!empty($item['class'])) $item_class .= check_plain($item['class']).' ';
			if(!empty($base)) $item_class .= $base.' ';			
			if(isset($item['bootstrap_class'])) {
				$bootstrap_class = $item['bootstrap_class'];
			} else {
				$bootstrap_class = array(
					'xs' => 'col-xs-12',
					'sm' => 'col-sm-12',
					'md' => 'col-md-12',
					'lg' => 'col-lg-12'
				);
			}
			$item_class = implode(' ', $bootstrap_class).' ';
			break;
		case 'region':
			if(!empty($base)) $item_class .= $base.' ';					
			break;
		default:
			break;
	}
		
	
  $output .= '  <div id="panel-bootstrap-'. $item['type'] .'-'. $id .'" class="' . $item_class . 'inside';
	
  if ($position == 0) $output .= ' ' . $item['type'] . '-inside-first';
  if ($position == $max) $output .= ' ' . $item['type'] . '-inside-last';
  if ($clear) $output .= ' clearfix';

  $output .= "\">\n";
	
  if (!empty($renderer->admin)) {
    $output .= panels_bootstrap_render_item_links($renderer, $id, $item);
    if($item['type'] == 'column'){
      $output .= '<div class="bootstrap-class-selector clearfix" >';
      $output .= panels_bootstrap_render_bootstrap_class_selector($renderer,$id);
      $output .= '</div>';
    }
    
  }
  $output .= $content;
  $output .= '  </div>' . "\n";

  return $output;
}
/**
 * Render a splitter div to place between the $left and $right items.
 *
 * If the right ID is NULL that means there isn't actually a box to the
 * right, but we need a splitter anyway. We'll mostly use info about the
 * left, but pretend it's 'fluid' so that the javascript won't actually
 * modify the right item.
 */
function panels_bootstrap_render_bootstrap_class_selector($renderer,$item_id) {
  $bootstrap_class = '';
  $item = $renderer->settings['items'][$item_id];
  $screen_sizes = array("xs","sm","md","lg");
  foreach($screen_sizes as $screen_size){
    $form[] = array(
      '#type' => 'select',
      '#id' => $item_id.'-'.$screen_size.'-class-selector',
      '#attributes' => array('data-for-id' => $item_id,'data-for' => 'panel-bootstrap-'.$item['type'].'-'.$item_id, 'data-screen-size' => $screen_size),
      '#title' => t($screen_size . ' column size'),
      '#options' => array(
        'col-'.$screen_size.'-1' => 'col-'.$screen_size.'-1',
        'col-'.$screen_size.'-2' => 'col-'.$screen_size.'-2',
        'col-'.$screen_size.'-3' => 'col-'.$screen_size.'-3',
        'col-'.$screen_size.'-4' => 'col-'.$screen_size.'-4',
        'col-'.$screen_size.'-5' => 'col-'.$screen_size.'-5',
        'col-'.$screen_size.'-6' => 'col-'.$screen_size.'-6',
        'col-'.$screen_size.'-7' => 'col-'.$screen_size.'-7',
        'col-'.$screen_size.'-8' => 'col-'.$screen_size.'-8',
        'col-'.$screen_size.'-9' => 'col-'.$screen_size.'-9',
        'col-'.$screen_size.'-10' => 'col-'.$screen_size.'-10',
        'col-'.$screen_size.'-11' => 'col-'.$screen_size.'-11',
        'col-'.$screen_size.'-12' => 'col-'.$screen_size.'-12'
        ),
      '#default_value' => "1",
      '#value' => isset($item['bootstrap_class'][$screen_size]) ? $item['bootstrap_class'][$screen_size] : 'col-'.$screen_size.'-12',
    );
  }
  return drupal_render($form);
}

/**
 * Render the dropdown links for an item.
 */
function panels_bootstrap_render_item_links($renderer, $id, $item) {
  $links = array();
  $remove = '';
  $add = '';
	
	switch($item['type']) {
		case 'container':
			if($id == 'canvas') {
				$title = t('Canvas');
				$settings = t('Canvas settings');
			} else {
				$title = t('Container');
				$settings = t('Container settings');
			}
			if (empty($item['children'])) {
				if($id != 'canvas') {
					$remove = t('Remove container');
				}
				switch($item['contains']) {
					case 'region':
						$add = t('Add region');
						break;
					case 'row':
						$add = t('Add row');
						break;
					case 'container':
						$add = t('Add container');
						break;
				}
			}	else {
				switch($item['contains']) {
					case 'region':
						$add = t('Add region to top');
						$add2 = t('Add region to bottom');
						break;
					case 'row':
						$add = t('Add row to top');
						$add2 = t('Add row to bottom');					
						break;
					case 'container':
						$add = t('Add container to top');
						$add2 = t('Add container to bottom');
				}
			}
			break;
		case 'row':
			$title = t('Row');
			$settings = t('Row settings');
			if (empty($item['children'])) {
				$remove = t('Remove row');
				$add = t('Add column');
			} else {
				$add = t('Add column to left');
				$add2 = t('Add column to right');
				break;
			}
			break;
		case 'column':
			$title = t('Column');
			$settings = t('Column settings');
			if (empty($item['children'])) {
				$remove = t('Remove column');
				switch($item['contains']) {
					case 'container':
						$add = t('Add container');
						break;
					case 'row':
						$add = t('Add row');
						break;
					case 'region':
						$add = t('Add region');
						break;
				}
			}	else {
				switch($item['contains']) {
					case 'container':
						$add = t('Add container to top');
						$add2 = t('Add container to bottom');
						break;
					case 'row':
						$add = t('Add row to top');
						$add2 = t('Add row to bottom');					
						break;
					case 'region':
						$add = t('Add region to top');
						$add2 = t('Add region to bottom');					
						break;
				}
			}
			break;
		case 'region':
			$title = t('Region');
			$settings = t('Region settings');
			$remove = t('Remove region');
			break;
		default:
			break;
	}

  if (!empty($settings)) {
    $links[] = array(
      'title' => $settings,
      'href' => $renderer->handler->get_url('layout', 'settings', $id),
      'attributes' => array('class' => array('ctools-use-modal')),
    );
  }
  if ($add) {
    $links[] = array(
      'title' => $add,
      'href' => $renderer->handler->get_url('layout', 'add', $id),
      'attributes' => array('class' => array('ctools-use-modal')),
    );
  }
  if (isset($add2)) {
    $links[] = array(
      'title' => $add2,
      'href' => $renderer->handler->get_url('layout', 'add', $id, 'right'),
      'attributes' => array('class' => array('ctools-use-modal')),
    );
  }
  if ($remove) {
    $links[] = array(
      'title' => $remove,
      'href' => $renderer->handler->get_url('layout', 'remove', $id),
      'attributes' => array('class' => array('use-ajax')),
    ); 
  }

  return theme('ctools_dropdown', array('title' => $title, 'links' => $links, 'class' => 'bootstrap-layout-only bootstrap-links bootstrap-title bootstrap-links-' . $id));
}



/**
 * AJAX responder to edit bootstrap settings for an item.
 *
 * $handler object
 *   The display renderer handler object.
 */
function panels_ajax_bootstrap_edit_settings($handler, $id) {
  $settings = &$handler->display->layout_settings;
  panels_bootstrap_convert_settings($settings, $handler->plugins['layout']);

  if (empty($settings['items'][$id])) {
    ctools_modal_render(t('Error'), t('Invalid item id.'));
  }

  $item = &$settings['items'][$id];
  $siblings = array();

  if ($id != 'canvas') {
    $siblings = $settings['items'][$item['parent']]['children'];
  }


  switch ($item['type']) {
		case 'container':
      if ($id == 'canvas') {
        $title = t('Configure canvas');
      } else {	
				$title = t('Configure container');
			}
			break;
    case 'column':
      $title = t('Configure column');
      break;
    case 'row':
			$title = t('Configure row');
      break;
    case 'region':
      $title = t('Configure region');
      break;
  }

  $form_state = array(
    'display' => &$handler->display,
    'item' => &$item,
    'id' => $id,
    'siblings' => $siblings,
    'settings' => &$settings,
    'ajax' => TRUE,
    'title' => $title,
    'op' => 'edit',
  );

  $output = ctools_modal_form_wrapper('panels_bootstrap_config_item_form', $form_state);
  if (!empty($form_state['executed'])) {
    // If the width type changed then other nearby items will have
    // to have their widths adjusted.
    panels_edit_cache_set($handler->cache);

    $css_id = isset($handler->display->css_id) ? $handler->display->css_id : '';
    $renderer = panels_bootstrap_create_renderer(TRUE, $css_id, array(), $settings, $handler->display, $handler->plugins['layout'], $handler);

    $output = array();
    // If the item is a region, replace the title.
    $class = $renderer->base[$item['type']] . '-' . $id;
    if ($item['type'] == 'region') {
      $output[] = ajax_command_replace(".$class h2.label",
        '<h2 class="label">' . check_plain($item['title']) . '</h2>');
    }

    // Rerender our links in case something changed.
    $output[] = ajax_command_replace('.bootstrap-links-' . $id,
		panels_bootstrap_render_item_links($renderer, $id, $item));

    // If editing the canvas, reset the CSS width
    if ($id == 'canvas') {
      // update canvas CSS.
      $css = array(
        '.' . $renderer->item_class['column'] . '-inside' => array(
          'padding-left' => $renderer->column_separation,
          'padding-right' => $renderer->column_separation,
        ),
        '.' . $renderer->item_class['region'] . '-inside' => array(
          'padding-left' => $renderer->region_separation,
          'padding-right' => $renderer->region_separation,
        ),
        '.' . $renderer->item_class['row'] => array(
          'padding-bottom' => $renderer->row_separation,
        ),
      );
      if (!empty($item['fixed_width']) && intval($item['fixed_width'])) {
        $css['.' . $renderer->base['canvas']] = array('width' => intval($item['fixed_width']) . 'px');
      }
      else {
        $css['.' . $renderer->base['canvas']] = array('width' => 'auto');
      }
      foreach ($css as $selector => $data) {
        $output[] = ajax_command_css($selector, $data);
      }
    }

    $output[] = ctools_modal_command_dismiss();
  }

  $handler->commands = $output;
}

/**
 * Configure a row, column or region on the bootstrap page.
 *
 * @param <type> $form_state
 * @return <type>
 */
function panels_bootstrap_config_item_form($form, &$form_state) {
  $display = &$form_state['display'];
  $item = &$form_state['item'];
  $siblings = &$form_state['siblings'];
  $settings = &$form_state['settings'];
  $id = &$form_state['id'];
	
	switch($item['type']) {
		case 'container':
			if ($id == 'canvas') {
				$form['class'] = array(
					'#title' => t('Canvas class'),
					'#type' => 'textfield',
					'#default_value' => isset($item['class']) ? $item['class'] : '',
					'#description' => t('It will add a special class to this canvas'),
				);
				$form['container_class_add'] = array(
					'#type' => 'checkbox',
					'#title' => t('Add container class'),
					'#default_value' => !empty($item['container_class_add']) ? $item['container_class_add'] : 0,
					'#description' => t('Add class "container" to this container')
				);
				$form['container_full_class_add'] = array(
					'#type' => 'checkbox',
					'#title' => t('Add container-full class'),
					'#default_value' => !empty($item['container_full_class_add']) ? $item['container_full_class_add'] : 0,
					'#description' => t('If you use a full width layout, you need to add the "container-full" class to your container to avoid horizontal scrolling.')
				);				
				if(empty($item['children'])) {				
					$form['contains'] = array(
						'#type' => 'select',
						'#title' => t('Contains'),
						'#default_value' => $item['contains'],
						'#options' => array(
							'row' => t('Row'),			
							'region' => t('Regions'),
							'container' => t('Container'),
						),
					);
				}
				$form['column_class'] = array(
					'#title' => t('Column class'),
					'#type' => 'textfield',
					'#default_value' => isset($item['column_class']) ? $item['column_class'] : '',
					'#description' => t('This class will be applied to all columns of the layout. If left blank this will be panels-bootstrap-column.'),
				);
				$form['row_class'] = array(
					'#title' => t('Row class'),
					'#type' => 'textfield',
					'#default_value' => isset($item['row_class']) ? $item['row_class'] : '',
					'#description' => t('This class will be applied to all rows of the layout. If left blank this will be panels-bootstrap-row.'),
				);
				$form['region_class'] = array(
					'#title' => t('Region class'),
					'#type' => 'textfield',
					'#default_value' => isset($item['region_class']) ? $item['region_class'] : '',
					'#description' => t('This class will be applied to all regions of the layout. If left blank this will be panels-bootstrap-region.'),
				);
				$form['container_class'] = array(
					'#title' => t('Container class'),
					'#type' => 'textfield',
					'#default_value' => isset($item['container_class']) ? $item['container_class'] : '',
					'#description' => t('This class will be applied to all containers of the layout. If left blank this will be panels-bootstrap-container.'),
				);
				
			} else {
				$form['class'] = array(
					'#title' => t('CSS class'),
					'#type' => 'textfield',
					'#default_value' => isset($item['class']) ? $item['class'] : '',
					'#description' => t('Enter a CSS class that will be used. This can be used to apply automatic styling from your theme, for example.'),
				);
				$form['container_class_add'] = array(
					'#type' => 'checkbox',
					'#title' => t('Add container class'),
					'#default_value' => !empty($item['container_class_add']) ? $item['container_class_add'] : 0,
					'#description' => t('Add class "container" to this container')
				);
				$form['container_full_class_add'] = array(
					'#type' => 'checkbox',
					'#title' => t('Add container-full class'),
					'#default_value' => !empty($item['container_full_class_add']) ? $item['container_full_class_add'] : 0,
					'#description' => t('If you use a full width layout, you need to add the "container-full" class to your container to avoid horizontal scrolling.')
				);								
				if(empty($item['children'])) {
					$form['contains'] = array(
						'#type' => 'select',
						'#title' => t('Contains'),
						'#default_value' => $item['contains'],
						'#options' => array(
							'row' => t('Row'),			
							'region' => t('Regions'),
						),
					);			
				}
			}
			break;			
		case 'row':
			$form['class'] = array(
				'#title' => t('CSS class'),
				'#type' => 'textfield',
				'#default_value' => isset($item['class']) ? $item['class'] : '',
				'#description' => t('Enter a CSS class that will be used. This can be used to apply automatic styling from your theme, for example.'),
			);
			break;			
		case 'column':
			$form['class'] = array(
				'#title' => t('CSS class'),
				'#type' => 'textfield',
				'#default_value' => isset($item['class']) ? $item['class'] : '',
				'#description' => t('Enter a CSS class that will be used. This can be used to apply automatic styling from your theme, for example.'),
			);
			if(empty($item['children'])) {
				$form['contains'] = array(
					'#type' => 'select',
					'#title' => t('Contains'),
					'#default_value' => $item['contains'],
					'#options' => array(
						'region' => t('Regions'),				
						'row' => t('Row'),			
					),
				);
			}
			break;
		case 'region':
			$form['title'] = array(
				'#title' => t('Region title'),
				'#type' => 'textfield',
				'#default_value' => $item['title'],
				'#required' => TRUE,
			);
			break;
		default:
			break;
	}

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for editing a bootstrap item.
 */
function panels_bootstrap_config_item_form_submit(&$form, &$form_state) {
  $item = &$form_state['item'];
	
	switch($item['type']) {
		case 'container':
		  $item['class'] = $form_state['values']['class'];
			if(!empty($form_state['values']['contains'])) $item['contains'] = $form_state['values']['contains'];
			$item['container_class_add'] = $form_state['values']['container_class_add'];
			$item['container_full_class_add'] = $form_state['values']['container_full_class_add'];			
			
			if ($form_state['id'] == 'canvas') {
				$item['column_class'] = $form_state['values']['column_class'];
				$item['row_class'] = $form_state['values']['row_class'];
				$item['region_class'] = $form_state['values']['region_class'];
				$item['container_class'] = $form_state['values']['container_class'];				
			}
			break;
		case 'row':
		  $item['class'] = $form_state['values']['class'];
			break;
		case 'column':
		  $item['class'] = $form_state['values']['class'];	
			if(!empty($form_state['values']['contains'])) $item['contains'] = $form_state['values']['contains'];
			break;
		case 'region':
			$item['title'] = $form_state['values']['title'];
			break;
		default:
			break;
	}
}

/**
 * AJAX responder to add a new row, column or region to a bootstrap layout.
 */
function panels_ajax_bootstrap_edit_add($handler, $id, $location = 'left') {
  ctools_include('modal');
  ctools_include('ajax');
  $settings = &$handler->display->layout_settings;
  panels_bootstrap_convert_settings($settings, $handler->plugins['layout']);

  if (empty($settings['items'][$id])) {
    ctools_modal_render(t('Error'), t('Invalid item id.'));
  }

  $parent = &$settings['items'][$id];

  switch ($parent['type']) {
		case 'container':
			switch($parent['contains']) {
				case 'container':
          $title = $location == 'left' ? t('Add container to top') : t('Add container to bottom');
          $item = array(
            'type' => 'container',
            'title' => '',
						'children' => array(),
            'parent' => $id,
          );
          break;									
				case 'row':
					$title = t('Add row');
					$item = array(
						'type' => 'row',
						'contains' => 'region',
						'children' => array(),
						'parent' => $id,
					);
					break;
				case 'region':
          $title = $location == 'left' ? t('Add region to top') : t('Add region to bottom');
          $item = array(
            'type' => 'region',
            'title' => '',
            'parent' => $id,
          );
          break;
				}		
			break;
    case 'row':
			$title = $location == 'left' ? t('Add column to left') : t('Add column to right');
			$item = array(
				'type' => 'column',
				'parent' => $id,
				'children' => array(),
			);
			break;				
    case 'column':
			switch($parent['contains']) {
				case 'container':
          $title = $location == 'left' ? t('Add container to top') : t('Add container to bottom');
          $item = array(
            'type' => 'container',
            'title' => '',
						'children' => array(),
            'parent' => $id,
          );
          break;													
				case 'row':
					$title = t('Add row');
					// Create the new item with defaults.
					$item = array(
						'type' => 'row',
						'contains' => 'region',
						'children' => array(),
						'parent' => $id,
					);
					break;
				case 'region':
          $title = $location == 'left' ? t('Add region to top') : t('Add region to bottom');
          $item = array(
            'type' => 'region',
            'title' => '',
            'parent' => $id,
          );
          break;
				}
    case 'region':
      // Cannot add items to regions.
      break;
		default:
			break;
  }

  $form_state = array(
    'display' => &$handler->display,
    'parent' => &$parent,
    'item' => &$item,
    'id' => $id,
    'settings' => &$settings,
    'ajax' => TRUE,
    'title' => $title,
    'location' => $location,
  );

  $output = ctools_modal_form_wrapper('panels_bootstrap_add_item_form', $form_state);
  if (!empty($form_state['executed'])) {
    // If the width type changed then other nearby items will have
    // to have their widths adjusted.
    panels_edit_cache_set($handler->cache);
    $output = array();

    $css_id = isset($handler->display->css_id) ? $handler->display->css_id : '';
    // Create a renderer object so we can render our new stuff.
    $renderer = panels_bootstrap_create_renderer(TRUE, $css_id, array(), $settings, $handler->display, $handler->plugins['layout'], $handler);

    $content = '';
    if ($item['type'] == 'region') {
      $handler->plugins['layout']['regions'][$form_state['key']] = $item['title'];

      $content = $handler->render_region($form_state['key'], array());

      // Manually add the hidden field that our region uses to store pane info.
      $content .= '<input type="hidden" name="panel[pane][' .
        $form_state['key'] . ']" value="" />';

    }
    else {
      // We need to make sure the left/middle/right divs exist inside this
      // so that more stuff can be added inside it as needed.
      foreach (array('left', 'middle', 'right') as $position) {
        if (!empty($content) || $renderer->admin) {
          $content .= '<div class="' . $renderer->base[$item['type']] . '-' . $form_state['key'] . '-' . $position . '"></div>';
        }
      }

    }

    // render the item
    $parent_class =  $renderer->base[$parent['type']] . '-' . $id;
    $item_output = panels_bootstrap_render_item($renderer, $item, $content, $form_state['key'], 0, 0, $item['type'] == 'row');

    // Get all the CSS necessary for the entire row (as width adjustments may
    // have cascaded).
    $css = array();

    $position = isset($renderer->positions[$form_state['key']]) ? $renderer->positions[$form_state['key']] : 'middle';
    // If there's a nearby item, add the splitter and rewrite the width
    // of the nearby item as it probably got adjusted.
    // The blocks of code in this else look very similar but are not actually
    // duplicated because the order changes based on left or right.
    switch ($position) {
      case 'left':
        if ($location == 'left') {
          $output[] = ajax_command_prepend('#panels-dnd-main .' . $parent_class . '-left', $item_output);
        }
        else if ($location == 'right') {
          // If we are adding to the right side of the left box, there is
          // a splitter that we have to remove; then we add our box normally,
          // and then add a new splitter for just our guy.
          $output[] = ajax_command_remove('panels-bootstrap-splitter-for-' . $renderer->base[$item['type']] . '-' . $form_state['key']);
          $output[] = ajax_command_append('#panels-dnd-main .' . $parent_class . '-left', $item_output);
        }
        break;
      case 'right':
        if (!empty($form_state['sibling'])) {
        }
        $output[] = ajax_command_append('#panels-dnd-main .' . $parent_class . '-right', $item_output);
        break;
      case 'middle':
        if ($location == 'left') {
          if (!empty($form_state['sibling'])) {
          }
          $output[] = ajax_command_prepend('#panels-dnd-main .' . $parent_class . '-middle', $item_output);
        }
        else {
          if (!empty($form_state['sibling'])) {
          }
          $output[] = ajax_command_append('#panels-dnd-main .' . $parent_class . '-middle', $item_output);
        }
        break;

    }

    // Send our fix height command.
    $output[] = array('command' => 'bootstrap_fix_height');

    if (!empty($form_state['sibling'])) {
      $sibling_width = '#panels-dnd-main .' . $renderer->base[$item['type']] . '-' . $form_state['sibling'] . '-width';
      $output[] = array(
        'command' => 'bootstrap_set_width',
        'selector' => $sibling_width,
        'width' => $settings['items'][$form_state['sibling']]['width'],
      );
    }
    foreach ($css as $selector => $data) {
      $output[] = ajax_command_css($selector, $data);
    }

    // Rerender our parent item links:
    $output[] = ajax_command_replace('.bootstrap-links-' . $id,
      panels_bootstrap_render_item_links($renderer, $id, $parent));

    $output[] = array(
      'command' => 'bootstrap_fix_firstlast',
      'selector' => '.' . $parent_class . '-inside',
      'base' => 'panels-bootstrap-' . $item['type'],
    );

    $output[] = ctools_modal_command_dismiss();
  }

  $handler->commands = $output;
}
/**
 * Form to add a row, column or region to a bootstrap layout.
 * @param <type> $form_state
 * @return <type>
 */
function panels_bootstrap_add_item_form($form, &$form_state) {
  $display = &$form_state['display'];
  $item = &$form_state['item'];
  $parent = &$form_state['parent'];
  $settings = &$form_state['settings'];
  $location = &$form_state['location'];
  $id = &$form_state['id'];
	
	switch($item['type']) {
		case 'container':
			$form['class'] = array(
				'#title' => t('CSS Class'),
				'#type' => 'textfield',
				'#default_value' => isset($item['class']) ? $item['class'] : '',
				'#description' => t('Enter a CSS class that will be used. This can be used to apply automatic styling from your theme, for example.'),
			);
			$form['container_class_add'] = array(
				'#type' => 'checkbox',
				'#title' => t('Add container class'),
				'#default_value' => !empty($item['container_class_add']) ? $item['container_class_add'] : 0,
				'#description' => t('Add class "container" to this container')
			);
			$form['container_full_class_add'] = array(
				'#type' => 'checkbox',
				'#title' => t('Add container-full class'),
				'#default_value' => !empty($item['container_full_class_add']) ? $item['container_full_class_add'] : 0,
				'#description' => t('If you use a full width layout, you need to add the "container-full" class to your container to avoid horizontal scrolling.')
			);				
			$form['contains'] = array(
				'#type' => 'select',
				'#title' => t('Contains'),
				'#default_value' => $item['contains'],
				'#options' => array(
					'row' => t('Row'),				
					'region' => t('Regions'),
				),
			);					
			break;		
		case 'row':
			$form['class'] = array(
				'#title' => t('CSS Class'),
				'#type' => 'textfield',
				'#default_value' => isset($item['class']) ? $item['class'] : '',
				'#description' => t('Enter a CSS class that will be used. This can be used to apply automatic styling from your theme, for example.'),
			);
			break;		
		case 'column':
			$form['class'] = array(
				'#title' => t('CSS Class'),
				'#type' => 'textfield',
				'#default_value' => isset($item['class']) ? $item['class'] : '',
				'#description' => t('Enter a CSS class that will be used. This can be used to apply automatic styling from your theme, for example.'),
			);
			$form['contains'] = array(
				'#type' => 'select',
				'#title' => t('Contains'),
				'#default_value' => $item['contains'],
				'#options' => array(
					'region' => t('Regions'),									
					'row' => t('Row'),
				),
			);		
			break;			
		case 'region':
			$form['title'] = array(
				'#title' => t('Region title'),
				'#type' => 'textfield',
				'#default_value' => $item['title'],
				'#required' => TRUE,
			);
			break;
		default:
			break;
	}

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for editing a bootstrap item.
 */
function panels_bootstrap_add_item_form_submit(&$form, &$form_state) {
  $item = &$form_state['item'];
  $parent = &$form_state['parent'];
  $location = &$form_state['location'];
  $settings = &$form_state['settings'];
	
	switch($item['type']) {
		case 'container':
			$item['class'] = $form_state['values']['class'];
			$item['container_class_add'] = $form_state['values']['container_class_add'];
			$item['container_full_class_add'] = $form_state['values']['container_full_class_add'];			
			$item['contains'] = $form_state['values']['contains'];
			break;
		case 'row':
			$item['class'] = $form_state['values']['class'];		
			break;
		case 'column':
			$item['class'] = $form_state['values']['class'];				
			$item['contains'] = $form_state['values']['contains'];
			break;			
		case 'region':
	    $item['title'] = $form_state['values']['title'];		
			break;
		default:
			break;
	}

  if ($item['type'] == 'region') {
    // derive the region key from the title
    $key = preg_replace("/[^a-z0-9]/", '_', drupal_strtolower($item['title']));
    while (isset($settings['items'][$key])) {
      $key .= '_';
    }
    $form_state['key'] = $key;
  } else {
    $form_state['key'] = $key = max(array_keys($settings['items'])) + 1;
  }

  $form_state['sibling'] = NULL;
  if ($item['type'] != 'row' && !empty($parent['children'])) {
    // Figure out what the width should be and adjust our sibling if
    // necessary.
    if ($location == 'left') {
      $form_state['sibling'] = reset($parent['children']);
    } else {
      $form_state['sibling'] = end($parent['children']);
    }
  }

  // Place the item.
  $settings['items'][$key] = $item;
  if ($location == 'left') {
    array_unshift($parent['children'], $key);
  } else {
    $parent['children'][] = $key;
  }
}

/**
 * AJAX responder to remove an existing row, column or region from a bootstrap
 * layout.
 */
function panels_ajax_bootstrap_edit_remove($handler, $id) {
  $settings = &$handler->display->layout_settings;
  panels_bootstrap_convert_settings($settings, $handler->plugins['layout']);

  if (empty($settings['items'][$id])) {
    ajax_render_error(t('Invalid item id.'));
  }

  $item = &$settings['items'][$id];
  $css_id = isset($handler->display->css_id) ? $handler->display->css_id : '';
  // Create a renderer object so we can render our new stuff.
  $renderer = panels_bootstrap_create_renderer(TRUE, $css_id, array(), $settings, $handler->display, $handler->plugins['layout'], $handler);

  $siblings = &$settings['items'][$item['parent']]['children'];
  $parent_class = '.'  . $renderer->base[$settings['items'][$item['parent']]['type']] . '-' . $item['parent'];

  // Find the offset of our array. This will also be the key because
  // this is a simple array.
  $offset = array_search($id, $siblings);

  // Only bother with this stuff if our item is fluid, since fixed is
  // as fixed does.
  if ($item['type'] != 'row') {
    if (isset($siblings[$offset + 1])) {
      $next = $siblings[$offset + 1];
    }
    if (isset($siblings[$offset - 1])) {
      $prev = $siblings[$offset - 1];
    }


    // Not sure what happens if they both failed. Maybe nothing.
  }

  // Remove the item.
  array_splice($siblings, $offset, 1);

  unset($settings['items'][$id]);

  // Save our new state.
  panels_edit_cache_set($handler->cache);
  $class = $renderer->base[$item['type']] . '-' . $id;
  $output = array();

  $output[] = ajax_command_remove('#panels-dnd-main .' . $class);

  // Regenerate the CSS for siblings.
  if (!empty($siblings)) {
    // Get all the CSS necessary for the entire row (as width adjustments may
    // have cascaded).
    $css = array();
    foreach ($css as $selector => $data) {
      $output[] = ajax_command_css($selector, $data);
    }
  }

  // There are potentially two splitters linked to this item to be removed.
  if (!empty($prev)) {
    $output[] = ajax_command_remove('.bootstrap-splitter-for-' . $renderer->base[$item['type']] . '-' . $prev);
  }

  // Try to remove the 'next' one even if there isn't a $next.
  $output[] = ajax_command_remove('.bootstrap-splitter-for-' . $renderer->base[$item['type']] . '-' . $id);

  if (!empty($prev) && !empty($next)) {
    // Add a new splitter that links $prev and $next:
    $prev_class = '#panels-dnd-main .' . $renderer->base[$item['type']] . '-' . $prev;
    $output[] = ajax_command_after($prev_class, $splitter);
    // Send our fix height command.
    $output[] = array('command' => 'bootstrap_fix_height');
  }
  // Rerender our parent item links:
  $output[] = ajax_command_replace('.bootstrap-links-' . $item['parent'],
    panels_bootstrap_render_item_links($renderer, $item['parent'], $settings['items'][$item['parent']]));

  $output[] = array(
    'command' => 'bootstrap_fix_firstlast',
    'selector' => $parent_class . '-inside',
    'base' => 'panels-bootstrap-' . $item['type'],
  );

  $handler->commands = $output;
}

/**
 * AJAX responder to store resize information when the user adjusts the
 * splitter.
 */
function panels_ajax_bootstrap_edit_resize($handler) {
  ctools_include('ajax');
  $settings = &$handler->display->layout_settings;
  panels_bootstrap_convert_settings($settings, $handler->plugins['layout']);
  $bootstrap_class = $_POST['bootstrap_class'];
  $bootstrap_screen = $_POST['bootstrap_screen'];
  $settings['items'][$_POST['item']]['bootstrap_class'][$bootstrap_screen] = $bootstrap_class;

  // Save our new state.
  panels_edit_cache_set($handler->cache);

  $handler->commands = array('ok');
}

/**
 * AJAX form to bring up the "reuse" modal.
 */
function panels_ajax_bootstrap_edit_reuse($handler) {
  $settings = &$handler->display->layout_settings;
  panels_bootstrap_convert_settings($settings, $handler->plugins['layout']);

  $form_state = array(
    'display' => &$handler->display,
    'settings' => &$settings,
    'ajax' => TRUE,
    'title' => t('Save this layout for reuse'),
  );

  $output = ctools_modal_form_wrapper('panels_bootstrap_reuse_form', $form_state);
  if (!empty($form_state['executed'])) {
    // Create the new layout.
    ctools_include('export');
    $layout = ctools_export_crud_new('panels_layout');
    $layout->plugin = 'bootstrap';
    $layout->name = $form_state['values']['name'];
    $layout->admin_title = $form_state['values']['admin_title'];
    $layout->admin_description = $form_state['values']['admin_description'];
    $layout->category = $form_state['values']['category'];
    $layout->settings = $handler->display->layout_settings;

    // Save it.
    ctools_export_crud_save('panels_layout', $layout);

    if (empty($form_state['values']['keep'])) {
      // Set the actual layout_settings to now use the newly minted layout:
      $handler->display->layout = 'bootstrap:' . $layout->name;
      $handler->display->layout_settings = array();

      // Save our new state.
      panels_edit_cache_set($handler->cache);
    }

    // Dismiss the modal.
    $output[] = ctools_modal_command_dismiss();
  }

  $handler->commands = $output;
}

function panels_bootstrap_reuse_form($form, &$form_state) {
  $form['markup'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#value' => t('If you save this layout for reuse it will appear in the list of reusable layouts at admin/structure/panels/layouts, and you will need to go there to edit it. This layout will then become an option for all future panels you make.'),
  );

  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrative title'),
    '#description' => t('This will appear in the administrative interface to easily identify it.'),
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#description' => t('The machine readable name of this layout. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
    '#machine_name' => array(
      'exists' => 'panels_bootstrap_edit_name_exists',
      'source' => array('admin_title'),
    ),
  );

  $form['category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category'),
    '#description' => t('What category this layout should appear in. If left blank the category will be "Miscellaneous".'),
  );

  $form['admin_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Administrative description'),
    '#description' => t('A description of what this layout is, does or is for, for administrative use.'),
  );

  $form['keep'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep current panel layout bootstrap'),
    '#description' => t('If checked, this panel will continue to use a generic bootstrap layout and will not use the saved layout. Use this option if you wish to clone this layout.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function panels_bootstrap_reuse_form_validate(&$form, &$form_state) {
  if (empty($form_state['values']['name'])) {
    form_error($form['name'], t('You must choose a machine name.'));
  }

  ctools_include('export');
  $test = ctools_export_crud_load('panels_layout', $form_state['values']['name']);
  if ($test) {
    form_error($form['name'], t('That name is used by another layout: @layout', array('@layout' => $test->admin_title)));
  }

  // Ensure name fits the rules:
  if (preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['name'])) {
    form_error($form['name'], t('Name must be alphanumeric or underscores only.'));
  }
}

/**
 * Test for #machine_name type to see if an export exists.
 */
function panels_bootstrap_edit_name_exists($name, $element, &$form_state) {
  ctools_include('export');
  $plugin = $form_state['plugin'];

  return (empty($form_state['item']->export_ui_allow_overwrite) && ctools_export_crud_load($plugin['schema'], $name));
}
