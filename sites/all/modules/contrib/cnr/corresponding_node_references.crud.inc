<?php
// $Id: corresponding_node_references.crud.inc,v 1.1.2.3 2010/11/11 14:09:04 domidc Exp $

/**
 * @file
 * Include file providing corresponding node reference insert, update, and delete handling.
 */

/**
 * Add any corresponding references on node insertion.
 *
 * @param $home_node the node object catched in the node api on this node a reference is made to the away node
 * @param $home_field the field on the home node used to make the reference
 * @param $away_node_type the type of the away node
 * @param $away_field the name of the field on the away node referencing to the home node
 */
function corresponding_node_references_insert($home_node, $home_field, $away_node_type, $away_field) {
  $old_node = node_load($home_node->nid, NULL, true);

  // Determine the nodereference values after the insert.
  if (isset($home_node->$home_field) && is_array($home_node->$home_field)) {
    foreach ($home_node->$home_field as $reference) {
      if (!empty($reference['nid'])) {
        // Load the referenced node if it is of the specified away type.
        if ($referenced_node = node_load(array('nid' => $reference['nid'], 'type' => $away_node_type), NULL, true)) {
          // Add the new reference.
  
          // If there are no other references, we need to make sure this is delta 0
          if ($referenced_node->{$away_field}[0]['nid'] == NULL) {
            $referenced_node->{$away_field}[0]['nid'] = $home_node->nid;
          }
          else {
            // Add the new reference.
            //Check for doubles, could happen when nodes of same type are referenced
            $exists = FALSE;
          	foreach ($referenced_node->{$away_field} as $key => $value) {
              if ($value['nid'] == $home_node->nid) {
                $exists = TRUE;
                break;
              }
            }
            
            if  (!$exists) {
          	  $referenced_node->{$away_field}[] = array('nid' => $home_node->nid);
            }
          }
        	        
          _corresponding_node_references_update($referenced_node);
        }
      }
    }
  }
}

/**
 * Change corresponding references on node updating.
 *
 * Corresponding changes are made for any references removed or added.
 *
 * @param $home_node the node object catched in the node api on this node a reference is made to the away node
 * @param $home_field the field on the home node used to make the reference
 * @param $away_node_type the type of the away node
 * @param $away_field the name of the field on the away node referencing to the home node
 * @param $process_unchanged whether or not to process node reference fields whose values have not changed.
 *   This is useful for nodes that existed before installing this module and have corresponding node
 *   reference fields that don't match up.
 */
function corresponding_node_references_update($home_node, $home_field, $away_node_type, $away_field, $process_unchanged = FALSE) {
  $old_node = node_load($home_node->nid, NULL, false);
  $old = $new = array();
    
  //if ($home_node->$home_field != $old_node->$home_field) {
    // Determine the nodereference values before the update.
    if (isset($old_node->$home_field) && is_array($old_node->$home_field)) {
      foreach ($old_node->$home_field as $reference) {
        if (!empty($reference['nid'])) {
          $old[] = $reference['nid'];
        }
      }
    }
    
    // If we are processing unchanged references, remove all new references from the old references.
    if ($process_unchanged) {
      $old = array_diff($old, $new);
    }    
    
    // Determine the nodereference values after the update.
    if (isset($home_node->$home_field) && is_array($home_node->$home_field)) {
      foreach ($home_node->$home_field as $reference) {
        if (!empty($reference['nid'])) {
          $new[] = $reference['nid'];
        }
      }
    }
  
    // Handle removed references.
    if ($removed = array_diff($old, $new)) {
      foreach ($removed as $nid) {
        // Load the referenced node if it is of the specified away type.
        if ($referenced_node = node_load(array('nid' => $nid, 'type' => $away_node_type))) {
          if (isset($referenced_node->$away_field) && is_array($referenced_node->$away_field)) {
            // Iterate through the away node's references.
            foreach ($referenced_node->$away_field as $key => $value) {
              // Remove references to the deleted node.
              if ($value['nid'] && $value['nid'] == $home_node->nid) {
                unset($referenced_node->{$away_field}[$key]);
                _corresponding_node_references_update($referenced_node);
                break;
              }
            }
          }
        }
      }
    }
    // Handle added references.
    // No array diff a reference overload could of happend or a mass update
    if ($added = $new) {
      foreach ($added as $nid) {
        // Load the referenced node if it is of the specified away type.
        if ($referenced_node = node_load(array('nid' => $nid, 'type' => $away_node_type), NULL, true)) {
        	// Detect whether the reference already exists.
          $exists = FALSE;
          if ($referenced_node->$away_field && !empty($referenced_node->$away_field)) {
            foreach ($referenced_node->$away_field as $data) {
              if ($data['nid'] == $home_node->nid) {
                $exists = TRUE;
                break;
              }
            }
          }
              
          //Empty places are removed
          //Yes this means the deltas change on the away node when a reference is made on the home node
          $values = array();
          if ($referenced_node->{$away_field}) {
            foreach ($referenced_node->{$away_field} as $key => $value) {                                           
              if (!empty($value['nid'])) {
                $values[] = $value;
              }                      
            }
          }
          $referenced_node->{$away_field} = $values;
                              
          // Add the new reference. Don't create a duplicate.                   
          if (!$exists) {  
            //Get the allowed values
            $unlimited = false;
            $field = content_fields($away_field, $referenced_node->type);
            if ($field['multiple'] == 1) {
              $unlimited = true;
            }
            elseif ($field['multiple'] == 0) {
              $allowed_references = 1;
            }
            else {
              $allowed_references = $field['multiple'];
            }
            
            //check for reference overloading      
            $references = count($referenced_node->{$away_field}) + 1;
            if (($allowed_references >= $references) || $unlimited) {
              $referenced_node->{$away_field}[] = array('nid' => $home_node->nid);
              _corresponding_node_references_update($referenced_node);
            }
            else {
            	$t_reference = format_plural($references, '1 reference', '@count references');
            	$t_allowed = format_plural($allowed_references, '1 reference is', '@count references are');
            	drupal_set_message(
            	  t('Reference overloading: @title would of had @t_reference and only @t_allowed permitted. Before adding a reference, 
            	     you would need to <a href="@url">edit</a> @title to remove an existing reference and resave this node to have make it correspond. 
            	     Or you could allow this reference instance to have more references, go to the cck field settings of the instance', 
                  array(
                    '@title' => $referenced_node->title, 
                    '@url' => url('node/'. $referenced_node->nid),
                    '@t_reference' => $t_reference,
                    '@t_allowed' => $t_allowed,
                  )
                ),
                'error'
              );
            }
          }
        }
      }
    }
  //}
}

/**
 * Remove corresponding references on node deletion.
 *
 * @param $home_node the node object catched in the node api on this node a reference is made to the away node
 * @param $home_field the field on the home node used to make the reference
 * @param $away_node_type the type of the away node
 * @param $away_field the name of the field on the away node referencing to the home node
 */
function corresponding_node_references_delete($home_node, $home_field, $away_node_type, $away_field) {
  // Iterate through the field's references.
  foreach ($home_node->$home_field as $reference) {
    if (!empty($reference['nid'])) {
      // Load the referenced node if it is of the specified away type.
      if ($referenced_node = node_load(array('nid' => $reference['nid'], 'type' => $away_node_type), NULL, true)) {
        // Iterate through the away node's references.
        foreach ($referenced_node->$away_field as $key => $value) {
          // Remove references to the deleted node.
          if ($value['nid'] && $value['nid'] == $home_node->nid) {
            unset($referenced_node->{$away_field}[$key]);
            _corresponding_node_references_update($referenced_node);
            break;
          }
        }
      }
    }
  }
}

/**
 * Update field data.
 *
 * @param $node the referenced node to be updated
 */
function _corresponding_node_references_update($node) {
  content_update($node);
}
