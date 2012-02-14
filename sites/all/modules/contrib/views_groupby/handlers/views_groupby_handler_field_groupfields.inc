<?php
// $Id: views_groupby_handler_field_groupfields.inc,v 1.4.2.3 2009/10/23 20:43:33 inadarei Exp $

/** @file 
* Group By Views Field Handler Implementation
*/

/**
 * Field handler 
 */
class views_groupby_handler_field_groupfields extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   *
   * This constructer assumes the calais_term table. If using another
   * table, we'll need to be more specific.
   */
  function construct() {
    parent::construct();
//      $this->additional_fields['nid'] = 'nid';

//    $this->additional_fields['tdid'] = 'tdid';
//    $this->additional_fields['vid'] = 'vid';
  }

/**  function option_definition() {
    $options = parent::option_definition();
    $options['link_to'] = array('default' => FALSE);
    return $options;
  }
  **/

  /**
   * Provide link to taxonomy option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    unset($form['label']);
    unset($form['alter']);    

    // Make the this utility field invisible ;)
    $form['exclude']['#type'] = 'hidden';
    $form['exclude']['#default_value'] = '1';
    
//    dpm($this->view);

    $curr_disp = $this->view->current_display;
    $options_fields = $this->_get_views_fields();
    
    $def_val = $this->options['views_groupby_fields_to_group'];
    $def_val = (empty($def_val) || !is_array($def_val)) ? array(t('-- None --')) : $def_val;
    
    $form['views_groupby_fields_to_group'] = array(
      '#title' => t('Fields to Group On'),
      '#type' => 'select',
      '#options' => $options_fields,
      '#multiple' => TRUE,
      '#required' => TRUE,
      '#description' => t('Select fields to group by. Attention: You need to first select these fields as Views Fields!'),
      '#default_value' => $def_val,
    );
    
    
    $options_sql_func = array( 'count' => t('Count') );
    $form['views_groupby_sql_function'] = array(
      '#title' => t('SQL Aggregation Function'),
      '#type' => 'select',
      '#options' => $options_sql_func,
      '#multiple' => FALSE,
      '#required' => TRUE,
      '#description' => t('Please select SQL aggregation function to use with grouping. Only \'COUNT\' is supported currently'),
      '#default_value' => $this->options['views_groupby_sql_function'],
    );

    $def_val2 = $this->options['views_groupby_fields_to_aggregate'];
    $def_val2 = (empty($def_val2) || !is_array($def_val2)) ? array(t('-- None --')) : $def_val2;
    
    $form['views_groupby_fields_to_aggregate'] = array(
      '#title' => t('Fields to Aggregate with the SQL function'),
      '#type' => 'select',
      '#options' => $options_fields,
      '#multiple' => TRUE,
      '#required' => TRUE,
      '#description' => t('Attention: in ANSI SQL you may not select fields that are used in grouping! If you do, Views will ignore them anyway'),
      '#default_value' => $def_val2,
    );

    $def_val3 = $this->options['views_groupby_field_sortby'];
    $def_val3 = (empty($def_val3)) ? '' : $def_val3;
    
    $form['views_groupby_field_sortby'] = array(
      '#title' => t('Field to Sort Resultset On (after SQL Function is applied)'),
      '#type' => 'select',
      '#options' => $options_fields,
      '#multiple' => FALSE,
      '#required' => TRUE,
      '#description' => t('Attention: usual Views sorting is ignored when you are using SQL aggregation'),
      '#default_value' => $def_val3,
    );

    $options_order_direction = array( 'asc' => t('Ascending'), 'desc' => t('Descending') );
    $form['views_groupby_sortby_direction'] = array(
      '#title' => t('Sorting Direction'),
      '#type' => 'select',
      '#options' => $options_order_direction,
      '#multiple' => FALSE,
      '#required' => TRUE,
      //'#description' => t('Please select SQL aggregation function to use with grouping. Only \'COUNT\' is supported currently'),
      '#default_value' => $this->options['views_groupby_sortby_direction'],
    );


  }
  
  function _get_views_fields($just_aliases=FALSE) {
    
    $curr_disp = $this->view->current_display;
    $handlers = $this->view->display[$curr_disp]->handler->get_handlers('field');    
        
    $avail_fields = array();
    if (is_array($handlers)) {
      foreach ($handlers as $key => $val) {
        if ($key == 'views_sql_groupedfields') continue; // May not select ourseleves!

        $unique_id = $val->options['id'];
        
        if ($just_aliases) {
          $avail_fields[] = $val->field_alias;
          continue;
        }

        $field_alias = $val->options['table'] . '_' . $val->options['field'];
        $relationship = '';

          $all_relationships = $this->_get_relationships ();        
          $rel = $val->options['relationship']; 
          $rel = $all_relationships[$rel];
          
          if (!empty($rel)) {
            $field_name = $rel->fieldprefix . '.' . $val->options['field']; // reserved, not used
            $field_alias = $rel->fieldprefix . '_' . $val->options['field'];          
            $relationship = (empty($rel)) ? '' : '[' . $rel->label . '] ';
          }
          
        $field_name = $relationship . $val->definition['group'] . ': ' . $val->definition['title'];
        $avail_fields[$unique_id] = $field_name;
        
      }
    }
    
    return $avail_fields;
  }
  
  function _get_relationships() {
    $default_rels = array();
    $curr_disp_rels = array();
    
    $curr_disp = $this->view->current_display;
    
    $default_rels = $this->view->display['default']->handler->options['relationships'];
    $curr_displ_rels = $this->view->display[$curr_disp]->handler->options['relationships'];
    
    $default_rels = (is_array($default_rels)) ? $default_rels : array();
    $curr_displ_rels = (is_array($curr_displ_rels)) ? $curr_displ_rels : array();

    $relationships = array_merge($default_rels, $curr_displ_rels);

    $all_rels = array();    
    $base_table = $this->view->base_table;
    
    if (is_array($relationships)) {
        foreach ($relationships as $key => $val) {
            $obj = new stdClass();
            $obj->fieldprefix = $base_table . '_' . $val['table'];          
            $obj->label = $val['label'];
            $obj->table = $val['table'];
            $obj->field = $val['field'];                        
            $all_rels[$key] = $obj;
        }
    }

    return $all_rels;    
    
  }

  /**
  * This may only be called from query() function because field aliases
  * are not generated prior to that in the lifecycle (e.g. in the options_form function).
  */
  function _unique_to_alias($unique_id) {
  
      $curr_disp = $this->view->current_display;
      $handlers = $this->view->display[$curr_disp]->handler->get_handlers('field');
      
      return $handlers[$unique_id]->field_alias;    
  }
  
  /**
  * When settings arrays are saved by option forms, field aliases are not
  * yet available, in the Views lifecycle, so we save settings using "id"
  * fields. Before we can use them in a query, we have to transform back
  * to sql aliases. This is what this function does.
  */
  function _fix_settings_array($from_array) {
  
    $to_array = NULL;
    if (is_array($from_array)) {
      $to_array = array();
      foreach ($from_array as $val) {
        $to_val = $this->_unique_to_alias($val);
        $to_array[] = $to_val;        
      }
    } else if (!empty($from_array)) { // if single value
      $to_array = array();    
      $to_array[] = $this->_unique_to_alias($from_array);
    }

    return $to_array;
  }

  /** 
  * Rewrite query to include aggregation
  */ 
  function query() {
    
    $to_group = $this->options['views_groupby_fields_to_group'];
    $to_group = $this->_fix_settings_array($to_group);
    $to_aggregate = $this->options['views_groupby_fields_to_aggregate'];
    $to_aggregate = $this->_fix_settings_array($to_aggregate);

    $sql_func = $this->options['views_groupby_sql_function'];    
    $orderby = $this->options['views_groupby_field_sortby'];
    $orderby = $this->_unique_to_alias($orderby);
    $orderby_dir =  $this->options['views_groupby_sortby_direction'];
        
    /**  If group_by or to_aggregate lists are empty nothing
    *    should be done. 
    **/
    if ( (!is_array($to_group) || sizeof($to_group)<1) ||
         (!is_array($to_aggregate) || sizeof($to_aggregate)<1) ) {
      return;
    }
    
    /**
    * Prevent the default behaviour of including the primary field in 
    * groupying by clause. That defeats the purpose of grouping since
    * primary field is unique on almost every row.
    */
    //$btable = $this->view->base_table;
    //$bfield = $this->view->base_field;
    //$this->query->add_field($btable, $bfield, $bfield, array('count' => TRUE));
    
    if ( is_array($to_group) ) {
      $sz_to_group = ''; $count = 0;
      foreach ($to_group as $field) {
        if ($count) {
          $sz_to_group .= ', ';
        }        
        $count++;
        $sz_to_group .= $field;
      }
      $this->query->add_groupby($sz_to_group);    
    }
    
    $views_fields = $this->_get_views_fields($just_aliases=TRUE);
                
    /** Add all other fields into count, to make sure they do not mess up group_by **/
    if (is_array($this->query->fields)) {
      foreach ($this->query->fields as $key => $field) {
        if ( !in_array($key, $to_group) &&
             !in_array($key, $to_aggregate) ) {

             //if ($key != $bfield) {
             // unset($this->query->fields[$key]);
             //}             
        } 
        else if (in_array($key, $to_aggregate)) {
            $this->query->fields[$key][$sql_func] = TRUE;
            $this->query->fields[$key]['alias'] = $key;
        }
      }
    }
    
    //dpm($this->query);
    //----$this->query->fields[$bfield]['count'] = true;
    //----$this->query->add_field($btable, $bfield, $btable . '_' . $bfield, array('count' => TRUE)); 

    $this->query->add_orderby(NULL, NULL, $orderby_dir, $orderby);
  }

  function render($values) {
//    dpm($values);
    return 'test';
  }
}
