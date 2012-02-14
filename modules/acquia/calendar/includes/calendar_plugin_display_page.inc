<?php
// $Id: calendar_plugin_display_page.inc,v 1.1.2.20 2009/07/24 19:44:09 karens Exp $
/**
 * The plugin that handles a full calendar page.
 * 
 * The only style option that will be available is the calendar
 * style, which creates the navigation and links to other calendar
 * displays. All options for paging, row plugins, etc. are
 * deferred to the attachments.
 */
class calendar_plugin_display_page extends views_plugin_display_page {
  
  /**
   * Display validation.
   */
  function validate() {
    $errors = parent::validate();
    
    $arguments = $this->display->handler->get_option('arguments');
    if (!in_array('date_argument', array_keys($arguments))) {
      if (empty($this->view->date_info->arg_missing)) {
        $errors[] = t("The Calendar display '@display_title' will not work without a Date argument.", array('@display_title' => $this->definition['title']));      
      }
      $this->view->date_info->arg_missing = TRUE;
    }
    elseif ($arguments['date_argument']['default_action'] != 'default' || $arguments['date_argument']['default_argument_type'] != 'date') {
      if (empty($this->view->date_info->arg_missing_default)) {
        $errors[] = calendar_errors('missing_argument_default');
      }
      $this->view->date_info->arg_missing_default = TRUE;
    }
    
    return $errors;
  }

  function get_style_type() { return 'calendar'; }
  
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('style_plugin', 'row_options', 'row_plugin', 'items_per_page'))) {
      return FALSE;
    }
    return parent::defaultable_sections($section);
  }

  function options(&$display) {
    parent::options($display);
    $display->display_options['displays'] = array();
    $display->display_options['style_plugin'] = 'calendar_nav';
    $display->display_options['items_per_page'] = 0;
    $display->display_options['row_plugin'] = '';
    $display->display_options['defaults']['style_plugin'] = FALSE;
    $display->display_options['defaults']['style_options'] = FALSE;
    $display->display_options['defaults']['row_plugin'] = FALSE;
    $display->display_options['defaults']['row_options'] = FALSE;
    $display->display_options['defaults']['items_per_page'] = FALSE;
  } 
  
  function option_definition () {
    $options = parent::option_definition();
    $options['calendar_colors'] = array('default' => array());
    $options['calendar_colors_vocabulary'] = array('default' => array());
    $options['calendar_colors_taxonomy'] = array('default' => array());
    $options['calendar_popup'] = array('default' => 0);
    $options['calendar_date_link'] = array('default' => '');
    $options['style_plugin'] = array('default' => 'calendar_nav');
    return $options;
  }

  /**
   * Provide the summary for attachment options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['calendar_settings'] = array(
      'title' => theme('advanced_help_topic', 'calendar', 'settings') . t('Calendar settings'),
    );
    
    $colors = $this->get_option('calendar_colors');
    $colors_taxonomy = $this->get_option('calendar_colors_taxonomy');
    $options['calendar_colors'] = array(
      'category' => 'calendar_settings',
      'title' => t('Legend Content Types'),
      'value' => t('Edit'),
    );
    $options['calendar_colors_vocabulary'] = array(
      'category' => 'calendar_settings',
      'title' => t('Legend Vocabularies'),
      'value' => t('Edit'),
    );
    $options['calendar_colors_taxonomy'] = array(
      'category' => 'calendar_settings',
      'title' => t('Legend Terms'),
      'value' => t('Edit'),
    );
    $popup_options = $this->popup_options();
    $default = $this->get_option('calendar_popup');
    $options['calendar_popup'] = array(
      'category' => 'calendar_settings',
      'title' => t('Date changer'),
      'value' => isset($default) ? $popup_options[$default] : $popup_options[0],
    );
    $default = $this->get_option('calendar_date_link');
    $options['calendar_date_link'] = array(
      'category' => 'calendar_settings',
      'title' => t('Add new date link'),
      'value' => !empty($default) ? check_plain(node_get_types('name', $default)) : '',
    );

  }
  
  function popup_options() {
    return array(0 => t('No'), 1 => t('Yes'));
  }
   
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);
       
    switch ($form_state['section']) {
      case 'calendar_popup':
        $form['#title'] .= t('Date changer');
        $form['calendar_popup'] = array(
          '#type' => 'radios',
          '#default_value' => $this->get_option('calendar_popup'),
          '#options' => $this->popup_options(),
          '#description' => t('Display a popup calendar date selector?'),
          );        
        break;
      
      case 'calendar_date_link':
        $form['#title'] .= t('Add new date link');
        $form['calendar_date_link'] = array(
          '#type' => 'radios',
          '#default_value' => $this->get_option('calendar_date_link'),
          '#options' => array('' => t('No link')) + node_get_types('names'),
          '#description' => t('Display a link to add a new date of the specified content type. Displayed only to users with appropriate permissions.'),
          );        
        break;

      case 'calendar_colors':
        $method =  'types';
        // TODO Move the embedded styles other than the color into a stylesheet.
        $form['#title'] .= t('Content Type Legend Colors');
        $form['calendar_colors']['#tree'] = TRUE;
        $form['calendar_colors']['#prefix'] = t('<div class="form-item"><label>Content Type</label><p>Set a hex color value (like #ffffff) to use in the calendar legend for each content type. Types with empty values will have no stripe in the calendar and will not be added to the legend.</p></div>');
        $form['calendar_colors']['colorpicker'] = array(
          '#type' => 'calendar_colorpicker',
          '#prefix' => '<div class="clear-block"><div style="float:left">',
          '#suffix' => '</div>',
        );
        $colors = $this->get_option('calendar_colors'); 

        switch ($method) {
          case 'types':
            $color_types = node_get_types('names');
            break;
        }
        foreach ($color_types as $key => $name) {
          $form['calendar_colors']['color'][$key] = array(
            '#title' => $name,
            '#type' => 'calendar_colorfield',
            '#default_value' => isset($colors[$key]) ? $colors[$key] : '#ffffff',
            '#calendar_colorpicker' => 'calendar-colors-colorpicker',
            '#prefix' => '<div style="float:left;margin-right:10px">',
            '#suffix' => '</div>',
          );
        }
        $form['calendar_colors']['color']['#suffix'] = '</div>';
        break;

      case 'calendar_colors_vocabulary':
        $taxonomies = taxonomy_get_vocabularies();
        $options = array();
        foreach ($taxonomies as $vid => $vocab) {
          $options[$vid] = $vocab->name;
        }
        $colors_vocabulary = $this->get_option('calendar_colors_vocabulary');
        $form['#title'] .= t('Vocabulary Legend Types');
        $form['calendar_colors_vocabulary'] = array(
	        '#type' => 'checkboxes',
	        '#default_value' => isset($colors_vocabulary) ? $colors_vocabulary : array(),
	        '#multiple' => TRUE,
	        '#options' => $options,
	        );
	      $form['calendar_colors_vocabulary']['#prefix'] = t('<div class="form-item"><label>Vocabularies</label>Select vocabularies to use for setting calendar legend colors by taxonomy term. This works best for vocabularies with only a limited number of possible terms.</div>');
        break;

      case 'calendar_colors_taxonomy':
        $taxonomies = (array) $this->get_option('calendar_colors_vocabulary');
        $colors_taxonomy = $this->get_option('calendar_colors_taxonomy');
        $form['#title'] .= t('Taxonomy Legend Colors');
        $form['calendar_colors_taxonomy']['#prefix'] = '';
        if (empty($taxonomies)) {
          $form['calendar_colors_taxonomy']['#prefix'] .= t('<div class="form-item warning">Please select Legend vocabularies first!</div>');
        }
        $form['calendar_colors_taxonomy']['#prefix'] .= t('<div class="form-item"><label>Taxonomy Terms</label><p>Set a hex color value (like #ffffff) to use in the calendar legend for each taxonomy term. Terms with empty values will have no stripe in the calendar and will not be added to the legend.</p></div>');
        $form['calendar_colors_taxonomy']['#tree'] = TRUE;
        $form['calendar_colors_taxonomy']['colorpicker'] = array(
          '#type' => 'calendar_colorpicker',
          '#prefix' => '<div class="clear-block"><div style="float:left">',
          '#suffix' => '</div>',
        );
        foreach ($taxonomies as $vid => $taxonomy){
          $vocab = taxonomy_get_tree($vid);
          foreach ($vocab as $tid => $term){
            $form['calendar_colors_taxonomy']['color'][$term->tid] = array(
	            '#title' => t($term->name),
	            '#type' => 'calendar_colorfield',
	            '#default_value' => isset($colors_taxonomy[$term->tid]) ? $colors_taxonomy[$term->tid] : '#ffffff',
	            '#calendar_colorpicker' => 'calendar-colors-taxonomy-colorpicker',
	            '#prefix' => '<div style="float:left;margin-right:10px">',
	            '#suffix' => '</div>',
	            );
	        }
	      }
	      $form['calendar_colors_taxonomy']['color']['#suffix'] = '</div>';
        break;
    }
  }

 /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit($form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'calendar_popup':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]['popup']);
        break;
      case 'calendar_colors':
      case 'calendar_colors_taxonomy':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]['color']);
        break;
      case 'calendar_colors_vocabulary':
        $this->set_option($form_state['section'], array_filter($form_state['values'][$form_state['section']]));
        break;
      case 'calendar_date_link':
        $this->set_option($form_state['section'], ($form_state['values'][$form_state['section']]));
        variable_set('calendar_date_link_'. $form_state['values']['calendar_date_link'], $this->display->handler->get_option('path'));
        break;
    }
  }  

}
