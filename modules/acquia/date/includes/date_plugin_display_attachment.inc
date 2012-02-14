<?php
// $Id: date_plugin_display_attachment.inc,v 1.1.2.2 2008/09/15 21:31:25 karens Exp $
/**
 * The plugin that handles date navigation attachments.
 * 
 * Creates a special attachment for this purpose only. 
 */
class date_plugin_display_attachment extends views_plugin_display_attachment {
  
  // Require the date_nav style. That style has a date_nav type
  // so it won't show up as a style option on any other display.
  function get_style_type() { return 'date_nav'; }
  
  // No options to set style, force it to the right value.
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('row_options', 'row_plugin', 'items_per_page'))) {
      return FALSE;
    }
    return parent::defaultable_sections($section);
  }

  function options(&$display) {
    parent::options($display);
    $display->display_options['style_plugin'] = 'date_nav';
    $display->display_options['items_per_page'] = 0;
    $display->display_options['row_plugin'] = '';
    //$display->display_options['defaults']['style_plugin'] = FALSE;
    $display->display_options['defaults']['style_options'] = FALSE;
    $display->display_options['defaults']['items_per_page'] = FALSE;
    $display->display_options['defaults']['row_plugin'] = FALSE;
    $display->display_options['defaults']['row_options'] = FALSE;
  } 
}

