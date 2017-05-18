<?php

/**
 * @param $status 
 * Get data from client and save it as node in db 
 * return message to client if success or not 
 */
function challenge_widget_inner_save_challenge($status){
  module_load_include('inc', 'node', 'node.pages');
  $challenge = (array)json_decode(filter_xss($_POST['data']));
  $info = json_decode(filter_xss($_POST['info']));
  $user = challenge_widget_validate_user($challenge,$info);
  if(!$user){
    return;
  }
  $node = challenge_widget_prepare_challenge($challenge,$user);
  /** if user not access to save this node return without save **/
  if($node->nid && !node_access('update',$node)){
    drupal_json_output(array('error'=>'you not access to save this challenge'));
    return ;
  }
  $form_state = convert_post_data_to_node_form($challenge,'challenge');
  //dpm($form_state);
  $form_id = 'challenge_node_form';
  //exit();
  drupal_form_submit($form_id, $form_state,$node);
  //dpm($node);
  //exit();
  $errors = form_get_errors();
  if($errors) {
    drupal_json_output(array('error'=>$errors,'data'=>'data'));
    return;
  }
  module_invoke_all('challenge_widget_save',$node,$status);

  $challenge['path'] = url(drupal_get_path_alias('node/' . $node->nid), array('absolute' => TRUE));
  $challenge['nid'] = $node->nid;
  drupal_json_output(array('error'=>null,'data'=>$challenge));
}

/**
 * prepare challenge node object
 * @param $challenge
 * @param $user
 * @return stdClass if success else return -1
 */
function challenge_widget_prepare_challenge($challenge, $user){
  //New challenge
  if(empty($challenge['nid']) || !(int)$challenge['nid']) {
    $node = new stdClass();
    $node->type = 'challenge';
    $node->language = LANGUAGE_NONE;
    $node->status = 1;
    $node->changed = REQUEST_TIME;
    $node->created = REQUEST_TIME;
    $node->is_new = TRUE;
    $node->name = $user->name;
    node_object_prepare($node);
  }
  //Update Challenge
  else {
    $node = node_load($challenge['nid']);
  }
  $node->title = $challenge['title'];
  return $node;
}

/**
 * this function get data from client before save challenge and check if user can save it .
 * @param $challenge
 * @param $info
 * @return user object or false if not access
 */
function challenge_widget_validate_user($challenge, $info){
  if(isset($info->external) && $info->external == true){
    $user =user_load_by_mail($challenge['user_email']);
    global $base_url;
    if(!$user->uid){
      drupal_json_output(array('error'=>'this email not exist in the site :'.$base_url.''));
      return false;
    }
  }
  else {
    global $user ;
    if($user->uid == 0){
      drupal_json_output(array('error'=>'Must login to the site!!'));
      return false;
    }
  }
  return $user;
}

function convert_from_db_data_to_client_data_chall($fields,$node) {
  $entity = array();
  $entity['title'] = $node->title;
  $entity['nid'] = $node->nid;
  foreach ($fields as $field) {
    $field_info = field_info_field($field['name']);
    $field_type = $field_info['type'];
    if (($field_type == "image" || $field_type == 'file')) {
      foreach ($node->{$field['name']}[LANGUAGE_NONE] as $file) {
        $entity[$field['name']][] = array(
          'fid' => $file['fid'],
          'filename' => $file['filename'],
          'url' => file_create_url($file['uri'])
        );
      }
    }
    if(isset($node->{$field['name']}) && isset($node->{$field['name']}[LANGUAGE_NONE])) {
      switch ($field_type) {
        case 'taxonomy_term_reference':
          $entity[$field['name']] = $node->{$field['name']};
          if (!empty($node->{$field['name']}[LANGUAGE_NONE][0])) {
            $entity[$field['name']] = $node->{$field['name']}[LANGUAGE_NONE][0];
          }
          break;
        case 'entityreference':
          $entity[$field['name']] = $node->{$field['name']};
          if (!empty($node->{$field['name']}[LANGUAGE_NONE][0])) {
            $entity[$field['name']] = array();
            foreach ($node->{$field['name']}[LANGUAGE_NONE] as $key => $value) {
               $user = user_load($value['target_id']);
               $entity[$field['name']][$key]['key'] = $user->name .'('.$user->uid.')';
               $entity[$field['name']][$key]['dispaly'] = $user->name;
            }
          }
          break;
        case 'list_text':
          if (!empty($node->{$field['name']}[LANGUAGE_NONE][0])) {
            $option_selected = $node->field_core_bus_rel['und'][0]['value'];
            $data = json_decode($_GET['data']);
            if(isset($data->action) &&  $data->action == 'edit') {
              $entity[$field['name']] = $node->{$field['name']}[LANGUAGE_NONE][0]['value'];
            } else {
              $entity[$field['name']] = t($node->{$field['name']}[LANGUAGE_NONE][0]['value']);
            }
          }
          break;
        default:
          if (isset($node->{$field['name']}[LANGUAGE_NONE][0]) && !empty($node->{$field['name']}[LANGUAGE_NONE][0]['value'])) {
            $entity[$field['name']] = $node->{$field['name']}[LANGUAGE_NONE][0]['value'];
          }
      }
    }
  }
  return $entity;
}

/**
 * @param $challenge-data from client
 * @param $type - type of the node(for Ex challenge)
 * @return array
 */
function convert_post_data_to_node_form($challenge, $type){
  $values['op'] = t('Save');
  unset($challenge['title']);
  $instance_info_fields = field_info_instances('node', 'challenge');
  foreach ($challenge as $name=>$value){
    $field = field_info_field($name);
    $field_type = $field['type'];
    if (($field_type == "image" || $field_type == 'file') ) {
      if(!is_array($value)){
        $value = explode(',',$value);
      }
      $form_state['field'][$name] = challenge_widget_field_file($value, $field,$instance_info_fields);
    }
    switch ($field_type) {
      case 'taxonomy_term_reference':
        if($value != '' && $value != null )
          $values[$name][LANGUAGE_NONE][0]= $value;
      case 'list_text':
        if($value != '' && $value != null )
          $values[$name][LANGUAGE_NONE][0]= $value;
        break;
      case 'mail':
        break;
      case 'entityreference':
        foreach ($value as $key => $value_item) {
          $values[$name][LANGUAGE_NONE][$key]['target_id'] = $value_item->key;
        }
        if($name == "field_challenge_ref"){
          $values[$name][LANGUAGE_NONE][0]= $value;
        }
      case 'datetime':
        if ($value != '' && $value != null ) {
          $d = date('m/d/Y',strtotime($value));
          $values[$name][LANGUAGE_NONE][0]['value'] = array('date' =>$d);
        }
        break;
      default:
        $values[$name][LANGUAGE_NONE][0]['value'] = $value;
    }
  }
  $form_state['submit_handlers'] = array('node_form_submit');
  $form_state['values'] = $values;
  return $form_state;
}

/**
 * the function get file data and return it in format that drupal_form know.
 * @param $images-file data
 * @param $field-settings about the field
 * @param $instance-settings about the instance
 * @return array
 */
function challenge_widget_field_file($images, $field, $instance) {
  $values = array();
  $values[LANGUAGE_NONE] = array("array_parents" => array(), "errors" => array(), "field" => $field, "instance" => $instance, "items_count" => 1);
  // add images to form state
  $values[LANGUAGE_NONE]['items'] = array();
  $values[LANGUAGE_NONE]['items'][0] = array("_weight" => 0, "fid" => 0, "display" => 1, "description" => "", "upload_button" => "Upload", "remove_button" => "Remove", "upload" =>  "", "alt" => "", "title" => "");
  if (empty($images)) {
    return $values;
  }
  if (!is_array($images)) {
    $images = array($images);
  }
  foreach ($images as $image) {
    if(is_array($image) || is_object($image)){
      $image = (object)$image;
      if($image->fid){
        $fid = $image->fid;
      }
      else $uri = $image->uri;
    }
    else $uri = $image;
    if(!isset($fid))
      $fid = db_query("select fid from file_managed where uri='" . $uri . "'")->fetchField();
    $file = file_load($fid);
    $values[LANGUAGE_NONE]['items'][] = (array)$file;
  }
  $values[LANGUAGE_NONE]['items_count'] = count($images);
  return $values;
}

//example implement hook_challenge_widget_save
function challenge_widget_challenge_widget_save($node,$status){
  // dpm($status);
}