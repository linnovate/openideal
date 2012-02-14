<?php
// $Id: hide_submit_admin.inc,v 1.1.2.12 2010/04/23 09:15:53 optalgin Exp $

/**
 * @file
 * Hide the submit button after clicked to prevent/reduce duplicate postings.
 * 
 * Admin pages and logic are defined in this file. 
 */

 /**
 * Image deletion confirmation form
 *
 * @param &$form_state
 * @param $filename - The file to delete
 * @return drupal form array
 */
 function hide_submit_admin_delete_image(&$form_state, $filename) {
     
    $filepath = HIDE_SUBMIT_IMG_DIRECTORY .'/'. $filename;
    
    if (!file_exists(HIDE_SUBMIT_IMG_DIRECTORY .'/'. $filename)) {
       drupal_goto('admin/settings/hide-submit');
    }

    $form['hide_submit_image_file_path'] = array('#type' => 'value', '#value' => $filepath);
    $form['hide_submit_image_file_name'] = array('#type' => 'value', '#value' => $filename);

    $msg = t('Are you sure you want to delete the image file %name', array('%name' => check_plain($filename)));
    
    return confirm_form($form, $msg, 'admin/settings/hide-submit');
}

/**
 * Image deletion confirmation form submit handler
 *
 * @param $form
 * @param &$form_state
 */
function hide_submit_admin_delete_image_submit($form, &$form_state) { 
    
    $filepath = $form_state['values']['hide_submit_image_file_path'];
    $filename = $form_state['values']['hide_submit_image_file_name'];
    
    if (file_exists(HIDE_SUBMIT_IMG_DIRECTORY) && 
        $filepath &&
        file_check_location($filepath, HIDE_SUBMIT_IMG_DIRECTORY) &&
        file_delete($filepath) )  {
            
       drupal_set_message( t('%name deleted succesfully.', array('%name' => check_plain($filename))) );
       $image = variable_get('hide_submit_image', HIDE_SUBMIT_DEFAULT_IMAGE);
    
       // After deleting we need to check if the deleted image was selected
       // or was it in the random list..
       if (is_array($image)) {
           $key = array_search($filepath, $image);
           if ($key !== FALSE) {
               // Remove from random list
               unset($image[$key]);
               
               if (count($image) >= 2) {
                   variable_set('hide_submit_image', $image);
                   drupal_set_message( t('Image removed from random list') );
               } 
               else {
                   $image = array_shift($image);
                   variable_set('hide_submit_image', $image);
                   variable_set('hide_submit_toggle_random', 0 ); 
                   variable_set('hide_submit_default_image', md5(basename($image)));
                   drupal_set_message( t('Random feature disabled because only one image was selected') );
               }
           }
       }
       elseif ($image == $filepath) {
           variable_set('hide_submit_image', HIDE_SUBMIT_DEFAULT_IMAGE);
       }
       
       if (variable_get('hide_submit_default_image', FALSE) == md5($filename) ) {
          variable_set('hide_submit_default_image', md5(HIDE_SUBMIT_DEFAULT_IMAGE_BASENAME));
          drupal_set_message( t('Image was selected as default, reverting to module\'s default image') );
       }
       
   } 
   else {
       drupal_set_message( t('Cannot delete %name, something went wrong.', array('%name' => check_plain($filename))) );
   }
}  

/**
 * Implementation of admin_settings callback
*/
function hide_submit_admin_settings(&$form_state) {
    
  drupal_add_js(HIDE_SUBMIT_ADMIN_SETTINGS_JS);
  
  $directory = HIDE_SUBMIT_IMG_DIRECTORY;
    
  // Check for a new uploaded image, and use that instead.
  if ($file = file_save_upload('hide_submit_image_upload', array('file_validate_is_image' => array()))) {
     if (file_check_directory( $directory, FILE_CREATE_DIRECTORY, 'hide_submit_image_upload')) {
        if (file_copy($file, $directory, FILE_EXISTS_RENAME)) {
           $_POST['hide_submit_custom_image_link'] = $file->filepath;
           $_POST['hide_submit_toggle_custom_image'] = 0;
        }
     }
  }
    
  // Create the form
  $form = array();
   
  // --- PREVIEW ---
  if (variable_get('hide_submit_script_mode', HIDE_SUBMIT_MODE_HIDE) == HIDE_SUBMIT_MODE_HIDE) {

      $form['fieldset_preview'] = array(
        '#type' => 'fieldset',
        '#title' => t('Preview of current animation and message settings'),
        '#collapsible' => FALSE,
      );
      
      $form['fieldset_preview']['preview'] = array(
        '#type' => 'markup',
        '#id' => 'hide-submit-preview-user',
        '#value' => filter_xss_admin( _hide_submit_clean_for_javascript( _hide_submit_get_message( ) ) ),
      );
  } 


  // --- LOAD OPTIONS FIELDSET ---

  $form['fieldset_js_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Script settings'),
      '#collapsible' => TRUE,
  );
  
  $form['fieldset_js_settings']['hide_submit_script_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Script mode'),
      '#default_value' => variable_get('hide_submit_script_mode', HIDE_SUBMIT_MODE_HIDE),
      '#options' => array(
            HIDE_SUBMIT_MODE_HIDE      => t('Hide buttons and display message'), 
            HIDE_SUBMIT_MODE_DISABLE   => t('Disable buttons (Experimental)'), ),
      '#description' => t("What the script shall do? hide the buttons or disable them"),
  );

  $form['fieldset_js_settings']['hide_submit_keypress'] = array(
      '#type' => 'radios',
      '#title' => t('Trigger options'),
      '#default_value' => variable_get('hide_submit_keypress', HIDE_SUBMIT_BUTTON_AND_KEY),
      '#options' => array(
                HIDE_SUBMIT_BUTTON_ONLY    => t('Trigger when button is clicked'), 
                HIDE_SUBMIT_BUTTON_AND_KEY => t('Trigger when button is clicked or when ENTER key pressed'), 
            ),
      '#description' => t("Control the script triggering handlers"),
  );
  
  
  $form['fieldset_js_settings']['hide_submit_js_load_option'] = array(
      '#type' => 'radios',
      '#title' => t('Load options'),
      '#default_value' => variable_get('hide_submit_js_load_option', HIDE_SUBMIT_DEFAULT_JS_LOAD),
      '#options' => array(
           HIDE_SUBMIT_EXCLUDE_LISTED_PAGES => t('Every page except the listed pages.'),
            HIDE_SUBMIT_IN_CONTENT_ADD_EDIT => t('Add/Edit node pages only'), 
                HIDE_SUBMIT_IN_LISTED_PAGES => t('Only on the listed pages.'),
          ),
      '#description' => t("Add/Edit node view will load for pages with url '/node/add' and '/node/*/edit'.  Every page, is well, EVERY PAGE.  For the last two options use the page list box to set a list of include/exclude urls."),
  );
  
  $form['fieldset_js_settings']['hide_submit_js_load_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Page list'),
      '#default_value' => variable_get('hide_submit_js_load_pages', ''),
      '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  // --- MESSAGE  ---
  
  $form['fieldset_message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message'),
    '#collapsible' => TRUE,
    '#description' => t("The message to print after submit is clicked and the button become invisible. i.e. Please wait.."),
  );

  $form['fieldset_message']['hide_submit_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Default Message'),
    '#rows' => 2,
    '#default_value' => variable_get('hide_submit_message', HIDE_SUBMIT_DEFAULT_MESSAGE ),
  );
  

  $form['fieldset_message']['fieldset_locale'] = array(
    '#type' => 'fieldset',
    '#title' => t('Localization'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $languages = language_list('language', TRUE);
  foreach ($languages as $langcode => $language) {
    if ($language->enabled) {
      $msg_for_language = "hide_submit_message_$langcode";
      $form['fieldset_message']['fieldset_locale'][$msg_for_language] = array(
        '#type' => 'textarea',
        '#rows' => 2,
        '#title' => check_plain($language->native),
        '#default_value' => variable_get($msg_for_language, ""),
      );
    }
  }
  
  // --- IMAGE SELECTION FIELDSET ---
  
  $form['fieldset_images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image or animation'),
    '#collapsible' => FALSE,
    '#theme' => 'hide_submit_images_fieldset',
    //'#description' => t("The message to print after submit is clicked and the button become invisible. i.e. Please wait.."),
  );

  // jpg, jpeg, gif, png (file_scan_directory using ereg)
  $mask = "^.+\.(([jJ][pP][gG)|([jJ][pP][eE][gG])|([gG][iI][fF])|([pP][nN][gG]))$";
  
  $default_images = file_scan_directory(HIDE_SUBMIT_MODULE_DIRECTORY .'/images', $mask);
  $user_images = file_scan_directory(HIDE_SUBMIT_IMG_DIRECTORY, $mask);

  $options = array();
  foreach (array_merge($default_images, $user_images) as $image_file) {
      //$count = 1;
      $key = md5($image_file->basename);
      $options[$key] = '';
      $list_of_images[$key] = $image_file->filename; 
      
      $form['fieldset_images']['hide_submit_images'][$key]['image'] = array(
        '#type' => 'markup',
        '#title' => $image_file->basename,
        '#value' => theme('image', $image_file->filename),
      );
  }
  
  $form['list_of_images'] = array( 
      '#type' => 'value',
      '#value' => $list_of_images,
  );
  
  foreach ($user_images as $image_file) {
      $key = md5($image_file->basename);
      $form['fieldset_images']['hide_submit_images'][$key]['operations'] = array('#value' => l(t('delete'), 'admin/settings/hide-submit/delete/'. $image_file->basename) );
  }

  $form['fieldset_images']['hide_submit_random'] = array(
      '#type' => 'checkboxes',
      '#default_value' => variable_get('hide_submit_random', array() ),
      '#options' => $options,
  );

  $form['fieldset_images']['hide_submit_default_image'] = array(
      '#type' => 'radios',
      '#default_value' => variable_get('hide_submit_default_image', md5(basename(HIDE_SUBMIT_DEFAULT_IMAGE)) ),
      '#options' => $options,
  );
  
  $form['fieldset_images']["hide_submit_toggle_custom_image"] = array(
      '#type' => 'checkbox',
      '#title' => t('Use custom link'),
      '#default_value' => variable_get('hide_submit_toggle_custom_image', 0 ),
      '#tree' => FALSE,
      '#description' => t('Check here if you want use a custom image link.')
    );

  $form['fieldset_images']['hide_submit_custom_image_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom image or animation'),
    '#default_value' => variable_get('hide_submit_custom_image_link', HIDE_SUBMIT_DEFAULT_IMAGE ),
    '#description' => t("A link to an image, this image will be added to message.."),
  );

  // Upload picture
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['fieldset_images']['hide_submit_image_upload'] = array(
    '#type' => 'file', 
    '#title' => t('Upload picture'), 
    '#size' => 48, 
    '#description' => t('Upload image or gif animation to display with the message.'),
  );
  
  $form['fieldset_images']["hide_submit_toggle_random"] = array(
      '#type' => 'checkbox',
      '#title' => t('Randomize images'),
      '#default_value' => variable_get('hide_submit_toggle_random', 0 ),
      '#tree' => FALSE,
      '#description' => t('Check here if you want use random image each time the script loads. You need to check the <em>random</em> box for at least two images')
    );

  // --- Advanced ---
  
  $form['fieldset_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['fieldset_advanced']['hide_submit_fix_known_conflicts'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Automatically fix known conflicts'), 
    '#default_value' => variable_get('hide_submit_fix_known_conflicts', TRUE),
    '#description' => t('There are some known conflicts between the hide submit script and other scripts using AJAX, check this box to try and prevent some of the known conflicts. Useful if you are using shoutbox module or insert module'),
  );  
  
  $form['fieldset_advanced']['hide_submit_form_exclusion_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Exclusion mode'),
      '#default_value' => variable_get('hide_submit_form_exclusion_mode', HIDE_SUBMIT_EXCLUDE_LISTED_FORMS),
      '#options' => array(
            HIDE_SUBMIT_EXCLUDE_LISTED_FORMS   => t('Exclude listed forms'), 
            HIDE_SUBMIT_EXCLUDE_UNLISTED_FORMS => t('Exclude unlisted forms'), ),
      '#description' => t("Choose exclusion mode for listed forms. <br /> NOTE: Forms that are not part of drupal Form-API will not be affected by this configuration"),
  );
    
  $form['fieldset_advanced']['hide_submit_form_exclusion'] = array(
      '#type' => 'textarea',
      '#title' => t('Form-id exclusion list'),
      '#default_value' => variable_get('hide_submit_form_exclusion', 'shoutbox_add_form'),
      '#description' => t("Here you can make a list of forms (form-id) you wish not to hide submit buttons . <br />For example you can exclude the login block submit button from hiding by typing <em>user_login_block</em>"),
  );  

  $form['fieldset_advanced']['hide_submit_attribute_filter'] = array(
      '#type' => 'textarea',
      '#title' => t('Attribute Filter (Experimental)'),
      '#default_value' => variable_get('hide_submit_attribute_filter', ''),
      '#description' => t("Here you can make a list of attribute filters see %selectors for formatting details<br />For example to filter the 'Save as draft' button type [value!=Save As Draft] ", array('%selectors' => l('Attribute Filters:', 'http://docs.jquery.com/Selectors'))),
  );  
    
  $form['fieldset_advanced']["hide_submit_debug"] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable debugging'),
      '#default_value' => variable_get('hide_submit_debug', FALSE ),
      '#tree' => FALSE,
      '#description' => t('Debugging turned on will paint included and excluded submit buttons')
    );
    
  $form = system_settings_form($form); 
  $form['#validate'][] = 'hide_submit_admin_settings_validate'; // add validation  
  $form['#submit'][] = 'hide_submit_admin_settings_submit';     // add submit handler 
  
  return $form;
}
  

/**
 * Admin settings validation handler
 *
 * @param $form
 * @param &$form_state
 */
function hide_submit_admin_settings_validate($form, &$form_state) {
    
    $values = (object) $form_state['values'];
    
    if ($values->hide_submit_toggle_random) {
        $selected_count = 0;
        foreach ($values->hide_submit_random as $key => $v) {
            if ($v) {
                $selected_count++;
            }
        }
        if ($selected_count < 2) {
            form_set_error('hide_submit_toggle_random', 'Random feature requires at least two selected images '. $selected_count);
        }
    }  
}

/**
 * Admin settings submit handler
 *
 * @param $form
 * @param &$form_state
 */
function hide_submit_admin_settings_submit($form, &$form_state) {
    
    $values = (object) $form_state['values'];
    $op = isset($values->op) ? $values->op : '';

    if ($op == t('Reset to defaults')) {
      variable_del('hide_submit_image');
    }
    // Set custom link
    if ($values->hide_submit_toggle_custom_image) {
        variable_set('hide_submit_image', $values->hide_submit_custom_image_link);
        drupal_set_message("Custom image is set..."); 
    }
    // Randomize
    elseif ($values->hide_submit_toggle_random) {
        drupal_set_message("Random image feature is set..."); 
        $images_to_random = array();
        foreach ($values->hide_submit_random as $key => $v) {
            if ($v) {
                $images_to_random[] = $values->list_of_images[$key];
            }
        }
        variable_set('hide_submit_image', $images_to_random);    
    }
    // Use single default image
    else {
        variable_set('hide_submit_image', $values->list_of_images[$values->hide_submit_default_image]);
        drupal_set_message("Default image is set...");    
    }
}

//------------------------------------------------------------------------
//                          THEME FUNCTIONS
//------------------------------------------------------------------------

/**
 * Theme function for the admin settings image fieldset
 *
 * @param $form
 */
function theme_hide_submit_images_fieldset($form) {
    
  $images = $form['hide_submit_images'];
  foreach (element_children($images) as $key) {

    $filename = $images[$key]['image']['#title'];
    unset($form[$key]['image']['#title']);                 
    
    $row = array();
    $row[] = array( 'data' => drupal_render($form['hide_submit_images'][$key]['image']) .'<br />'. $filename, 'class' => 'hide-submit-image');
    $row[] = array( 'data' => drupal_render($form['hide_submit_random'][$key]), 'class' => 'hide-submit-random');
    $row[] = array( 'data' => drupal_render($form['hide_submit_default_image'][$key]), 'class' => 'hide-submit-radio');
    $row[] = array( 'data' => drupal_render($form['hide_submit_images'][$key]['operations']), 'class' => 'hide-submit-operations');
    
    $rows[] = $row;
  }

  $header = array( t('Image'), t('Random'), t('Default'), t('Operations')  );
  
  $output  = drupal_render($form) . theme('table', $header, $rows);
  
  return $output;
}
  