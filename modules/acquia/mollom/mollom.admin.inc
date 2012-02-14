<?php
// $Id: mollom.admin.inc,v 1.1.2.39 2010/09/15 02:56:56 dries Exp $

/**
 * @file
 * Administrative page callbacks for Mollom module.
 */

/**
 * Menu callback; Displays a list of forms configured for Mollom.
 */
function mollom_admin_form_list() {
  _mollom_testing_mode_warning();

  $modes = array(
    MOLLOM_MODE_DISABLED => t('No protection'),
    MOLLOM_MODE_CAPTCHA => t('CAPTCHA only'),
    MOLLOM_MODE_ANALYSIS => t('Text analysis and CAPTCHA backup'),
  );

  $header = array(
    t('Form'),
    t('Protection mode'),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $rows = array();
  $result = db_query("SELECT form_id FROM {mollom_form}");
  while ($form_id = db_result($result)) {
    $mollom_form = mollom_form_load($form_id);
    $rows[] = array(
      $mollom_form['title'],
      $modes[$mollom_form['mode']],
      l(t('Configure'), 'admin/settings/mollom/manage/' . $form_id),
      l(t('Unprotect'), 'admin/settings/mollom/unprotect/' . $form_id),
    );
  }

  // Add a row to add a form.
  if (empty($rows)) {
    $rows[] = array(array('data' => l(t('Add form'), 'admin/settings/mollom/add'), 'colspan' => 4));
  }

  return theme('table', $header, $rows);
}

/**
 * Return registered forms as an array suitable for a 'checkboxes' form element #options property.
 */
function mollom_admin_form_options() {
  // Retrieve all registered forms.
  $form_list = mollom_form_list();

  // Remove already configured form ids.
  $result = db_query("SELECT form_id FROM {mollom_form}");
  while ($form_id = db_result($result)) {
    unset($form_list[$form_id]);
  }
  // If all registered forms are configured already, output a message, and
  // redirect the user back to overview.
  if (empty($form_list)) {
    drupal_set_message(t('All available forms are protected already.'));
    drupal_goto('admin/settings/mollom');
  }

  // Load module information.
  $modules = module_implements('mollom_form_info');
  $placeholders = db_placeholders($modules, 'varchar');
  $result = db_query("SELECT name, info FROM {system} WHERE type = 'module' AND name IN ($placeholders)", $modules);
  $modules = array();
  while ($row = db_fetch_object($result)) {
    $module_info = unserialize($row->info);
    $modules[$row->name] = t($module_info['name']);
  }

  // Transform form information into an associative array suitable for #options.
  foreach ($form_list as $form_id => $info) {
    $form_list[$form_id] = $modules[$info['module']] . ': ' . $info['title'];
  }
  // Sort form options by title.
  asort($form_list);

  return $form_list;
}

/**
 * Form builder; Configure Mollom protection for a form.
 */
function mollom_admin_configure_form(&$form_state, $mollom_form = NULL) {
  // If no $mollom_form was passed, then we are adding a new form configuration.
  if (!isset($mollom_form)) {
    if (!isset($form_state['storage']['mollom_form'])) {
      $form_state['storage']['step'] = 'select';
    }
    else {
      $form_state['storage']['step'] = 'configure';
      $mollom_form = $form_state['storage']['mollom_form'];
    }
  }
  // When adding a new form configuration, passing form_id via path argument.
  elseif (is_string($mollom_form)) {
    $mollom_form = mollom_form_new($mollom_form);
    $form_state['storage']['step'] = 'configure';
    $form_state['storage']['mollom_form'] = $mollom_form;
  }
  // Otherwise, we are editing an existing form configuration.
  else {
    $form_state['storage']['step'] = 'configure';
    $form_state['storage']['mollom_form'] = $mollom_form;
  }

  $form['#tree'] = TRUE;

  switch ($form_state['storage']['step']) {
    case 'select':
      $form['mollom']['form_id'] = array(
        '#type' => 'select',
        '#title' => t('Form'),
        '#options' => mollom_admin_form_options(),
        '#required' => TRUE,
      );
      $form['actions']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
        '#submit' => array('mollom_admin_configure_form_next_submit'),
      );
      break;

    case 'configure':
      drupal_set_title(t('Configure %form-title protection', array('%form-title' => $mollom_form['title'])));
      $form['#after_build'][] = 'mollom_admin_configure_form_after_build';

      // Display a list of fields for textual analysis (last step).
      $form['mollom']['form_id'] = array(
        '#type' => 'value',
        '#value' => $mollom_form['form_id'],
      );

      $modes = array();
      // Textual analysis, if any elements are available.
      if (!empty($mollom_form['elements'])) {
        $modes[MOLLOM_MODE_ANALYSIS] = t('Text analysis');
      }
      // CAPTCHA-only, always available.
      $modes[MOLLOM_MODE_CAPTCHA] = t('CAPTCHA');

      $form['mollom']['mode'] = array(
        '#type' => 'radios',
        '#title' => t('Protection mode'),
        '#options' => $modes,
        '#default_value' => $mollom_form['mode'],
      );

      if (!empty($mollom_form['elements'])) {
        // If not re-configuring an existing protection, make it the default.
        if (!isset($mollom_form['mode'])) {
          $form['mollom']['mode']['#default_value'] = MOLLOM_MODE_ANALYSIS;
        }

        // Textual analysis filters.
        $form['mollom']['checks'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Analyze text for'),
          '#options' => array(
            'spam' => t('Spam'),
          ),
          '#default_value' => $mollom_form['checks'],
          // D7 only.
          '#value' => array('spam' => 'spam'),
          '#access' => FALSE,
        );

        // Form elements defined by hook_mollom_form_info() use the
        // 'parent][child' syntax, which Form API also uses internally for
        // form_set_error(), and which allows us to recurse into nested fields
        // during processing of submitted form values. However, since we are using
        // those keys also as internal values to configure the fields to use for
        // textual analysis, we need to encode them. Otherwise, a nested field key
        // would result in the following checkbox attribute:
        //   '#name' => 'mollom[enabled_fields][parent][child]'
        // This would lead to a form validation error, because it is a valid key.
        // By encoding them, we prevent this from happening:
        //   '#name' => 'mollom[enabled_fields][parent%5D%5Bchild]'
        $elements = array();
        foreach ($mollom_form['elements'] as $key => $value) {
          $elements[rawurlencode($key)] = $value;
        }
        $enabled_fields = array();
        foreach ($mollom_form['enabled_fields'] as $value) {
          $enabled_fields[] = rawurlencode($value);
        }
        $form['mollom']['enabled_fields'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Text fields to analyze'),
          '#options' => $elements,
          '#default_value' => $enabled_fields,
          '#required' => $mollom_form['mode'] == MOLLOM_MODE_ANALYSIS,
        );
      }

      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );
      break;
  }

  $form['actions']['cancel'] = array(
    '#value' => l(t('Cancel'), 'admin/settings/mollom'),
  );

  return $form;
}

/**
 * #after_build callback for mollom_admin_configure_form().
 */
function mollom_admin_configure_form_after_build($form, &$form_state) {
  // Make field checkboxes required, if protection mode is textual analysis.
  // Accounts for different add and edit form builds and rebuilds.
  $form['mollom']['enabled_fields']['#required'] = ($form_state['values']['mollom']['mode'] == MOLLOM_MODE_ANALYSIS);

  return $form;
}

/**
 * Form submit handler for 'Next' button on Mollom form configuration form.
 */
function mollom_admin_configure_form_next_submit($form, &$form_state) {
  $form_id = $form_state['values']['mollom']['form_id'];
  $form_state['redirect'] = $_GET['q'] . '/' . $form_id;
  // D6 only.
  unset($form_state['storage']);
}

/**
 * Form validation handler for mollom_admin_configure_form().
 */
function mollom_admin_configure_form_validate(&$form, &$form_state) {
  // For the 'configure' step, output custom #required form element errors for
  // 'checks' and 'enabled_fields', as their labels do not work with the default
  // #required form error message.
  if ($form_state['storage']['step'] == 'configure') {
    // Make field checkboxes required, if protection mode is textual analysis.
    $required = ($form_state['values']['mollom']['mode'] == MOLLOM_MODE_ANALYSIS);
    $form['mollom']['checks']['#required'] = $required;
    $form['mollom']['enabled_fields']['#required'] = $required;

    if ($required && !array_filter($form_state['values']['mollom']['checks'])) {
      form_error($form['mollom']['checks'], t('At least one text analysis check is required.'));
    }
    if ($required && !array_filter($form_state['values']['mollom']['enabled_fields'])) {
      form_error($form['mollom']['enabled_fields'], t('At least one field is required for text analysis.'));
    }
  }
}

/**
 * Form submit handler for mollom_admin_configure_form().
 */
function mollom_admin_configure_form_submit($form, &$form_state) {
  $mollom_form = $form_state['values']['mollom'];
  // Merge in form information from $form_state.
  $mollom_form += $form_state['storage']['mollom_form'];

  // Only store a list of enabled textual analysis checks.
  $mollom_form['checks'] = array_keys(array_filter($mollom_form['checks']));
  // Prepare selected fields for storage.
  $enabled_fields = array();
  foreach (array_keys(array_filter($mollom_form['enabled_fields'])) as $field) {
    $enabled_fields[] = rawurldecode($field);
  }
  $mollom_form['enabled_fields'] = $enabled_fields;

  $status = mollom_form_save($mollom_form);
  if ($status === SAVED_NEW) {
    drupal_set_message(t('The form protection has been added.'));
  }
  else {
    drupal_set_message(t('The form protection has been updated.'));
  }

  unset($form_state['storage']);
  $form_state['redirect'] = 'admin/settings/mollom';
}

/**
 * Form builder; Remove Mollom protection from a form.
 */
function mollom_admin_unprotect_form(&$form_state, $mollom_form) {
  $form['#tree'] = TRUE;
  $form['form'] = array(
    '#type' => 'item',
    '#title' => t('Form'),
    '#value' => $mollom_form['title'],
  );
  $form['mollom']['form_id'] = array(
    '#type' => 'value',
    '#value' => $mollom_form['form_id'],
  );

  return confirm_form($form,
    t('Are you sure you want to unprotect this form?'),
    'admin/settings/mollom',
    t('Mollom will no longer protect this form from spam.')
  );
}

/**
 * Form submit handler for mollom_admin_unprotect_form().
 */
function mollom_admin_unprotect_form_submit($form, &$form_state) {
  db_query("DELETE FROM {mollom_form} WHERE form_id = '%s'", $form_state['values']['mollom']['form_id']);

  $form_state['redirect'] = 'admin/settings/mollom';
}

/**
 * Form constructor to configure the blacklist.
 *
 * @param $type
 *   The type of blacklist; i.e., 'spam', 'profanity', or 'unwanted'.
 */
function mollom_admin_blacklist_form(&$form_state, $type = 'spam') {
  $form['#tree'] = TRUE;

  // Select options and translation of internal values for rendering.
  $contexts = array(
    'everything' => t('All fields'),
    'links' => t('Links'),
    'author' => t('Author name'),
  );

  $form['blacklist'] = array();
  // Do not retrieve the current blacklist when submitting the form.
  $blacklist = (empty($form_state['input']) ? mollom('mollom.listBlacklistText') : array());
  if (is_array($blacklist)) {
    foreach ($blacklist as $id => $entry) {
      if ($entry['reason'] != $type) {
        continue;
      }
      $row = array(
        'context' => array('#value' => check_plain($contexts[$entry['context']])),
        'text' => array('#value' => check_plain($entry['text'])),
        'text' => array('#value' => check_plain($entry['text'])),
      );
      $delete_url_parts = array(
        'admin/settings/mollom/blacklist/delete',
        base64_encode($entry['text']),
        $entry['context'],
        $entry['reason'],
      );
      $row['actions']['delete'] = array(
        '#value' => l(t('delete'), implode('/', $delete_url_parts), array('query' => drupal_get_destination())),
      );
      $form['blacklist'][$id] = $row;
    }
  }

  $form['entry']['context'] = array(
    '#type' => 'select',
    '#options' => $contexts,
    '#default_value' => 'everything',
    '#required' => TRUE,
  );
  $form['entry']['text'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#required' => TRUE,
    '#maxlength' => 64,
  );
  $form['entry']['reason'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['entry']['actions'] = array(
    '#tree' => FALSE,
  );
  $form['entry']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Form submit handler to save a text string to the Mollom blacklist.
 */
function mollom_admin_blacklist_form_submit($form, &$form_state) {
  $result = mollom('mollom.addBlacklistText', $form_state['values']['entry']);

  if ($result === TRUE) {
    drupal_set_message(t('The entry was added to the blacklist.'));
  }
  else {
    drupal_set_message(t('An error occurred upon trying to add the text to the blacklist.'), 'error');
  }
}

/**
 * Formats the blacklist form as table to embed the form.
 */
function theme_mollom_admin_blacklist_form($form) {
  $header = array(
    t('Context'),
    t('Text'),
    '',
  );
  $rows = array();

  $rows[] = array(
    drupal_render($form['entry']['context']),
    drupal_render($form['entry']['text']),
    drupal_render($form['entry']['actions']),
  );

  foreach (element_children($form['blacklist']) as $id) {
    $rows[] = array(
      drupal_render($form['blacklist'][$id]['context']),
      drupal_render($form['blacklist'][$id]['text']),
      drupal_render($form['blacklist'][$id]['actions']),
    );
  }

  // This table is never empty due to the form.
  $output  = theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Form builder; Builds the confirmation form for deleting a blacklist item.
 *
 * @param $key
 *   The blacklist entry text to remove, base64-encoded.
 * @param $context
 *   The context of the blacklist entry.
 * @param $reason
 *   The reason of the blacklist entry.
 *
 * @ingroup forms
 * @see mollom_admin_blacklist_delete_submit()
 */
function mollom_admin_blacklist_delete(&$form_state, $key, $context, $reason) {
  $text = base64_decode($key);
  $form['text'] = array(
    '#type' => 'value',
    '#value' => $text,
  );
  $form['context'] = array(
    '#type' => 'value',
    '#value' => $context,
  );
  $form['reason'] = array(
    '#type' => 'value',
    '#value' => $reason,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete %text from the blacklist?', array('%text' => $text)),
    'admin/settings/mollom/blacklist',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Form submit handler to delete an entry from the blacklist.
 */
function mollom_admin_blacklist_delete_submit($form, &$form_state) {
  $data = array(
    'text' => $form_state['values']['text'],
    'context' => $form_state['values']['context'],
    'reason' => $form_state['values']['reason'],
  );
  $result = mollom('mollom.removeBlacklistText', $data);

  if ($result === TRUE) {
    drupal_set_message(t('The entry was removed from the blacklist.'));
  }
  else {
    drupal_set_message(t('An error occurred upon trying to remove the item from the blacklist.'), 'error');
  }

  $form_state['redirect'] = 'admin/settings/mollom/blacklist';
}

/**
 * Form builder; Global Mollom settings form.
 *
 * This form does not validate Mollom API keys, since the fallback method still
 * needs to be able to be reconfigured in case Mollom services are down.
 * mollom.verifyKey would invalidate the keys and throw an error; hence,
 * _mollom_fallback() would invoke form_set_error(), effectively preventing this
 * form from submitting.
 */
function mollom_admin_settings(&$form_state) {
  // Output a positive status message, since users keep on asking whether
  // Mollom should work or not. Re-check on every regular visit of this form to
  // verify the module's configuration.
  if (empty($form_state['post'])) {
    $status = _mollom_status();
    // If there is any configuration error, then mollom_init() will have output
    // it already.
    if ($status === TRUE) {
      drupal_set_message(t('We contacted the Mollom servers to verify your keys: the Mollom services are operating correctly. We are now blocking spam.'));
    }
  }

  $form['access-keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access keys'),
    '#description' => t('To use Mollom, you need a public and private key. To obtain your keys, <a href="@mollom-login-url">register and login on mollom.com</a>, and <a href="@mollom-manager-add-url">create a subscription</a> for your site. Once you created a subscription, copy your private and public access keys from the <a href="@mollom-manager-url">site manager</a> into the form fields below, and you are ready to go.', array(
      '@mollom-login-url' => 'http://mollom.com/user',
      '@mollom-manager-add-url' => 'http://mollom.com/site-manager/add',
      '@mollom-manager-url' => 'http://mollom.com/site-manager',
    )),
    '#collapsible' => TRUE,
    // Only show key configuration fields if they are not configured or invalid.
    '#collapsed' => !isset($status) ? FALSE : $status === TRUE,
  );
  // Keys are not #required to allow to install this module and configure it
  // later.
  $form['access-keys']['mollom_public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public key'),
    '#default_value' => variable_get('mollom_public_key', ''),
    '#description' => t('Used to uniquely identify you.'),
  );
  $form['access-keys']['mollom_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key'),
    '#default_value' => variable_get('mollom_private_key', ''),
    '#description' => t('Used to prevent someone else from hijacking your requests. Similar to a password, it should never be shared with anyone.'),
  );

  $form['mollom_fallback'] = array(
    '#type' => 'radios',
    '#title' => t('Fallback strategy for protected forms'),
    // Default to treating everything as inappropriate.
    '#default_value' => variable_get('mollom_fallback', MOLLOM_FALLBACK_BLOCK),
    '#options' => array(
      MOLLOM_FALLBACK_BLOCK => t('Block all form submissions'),
      MOLLOM_FALLBACK_ACCEPT => t('Accept all form submissions'),
    ),
    '#description' => t('In case the Mollom services are unreachable, no text analysis is performed and no CAPTCHAs are generated. If this occurs, your site will use the configured fallback strategy until the server problems are resolved. Subscribers to <a href="@pricing-url">Mollom Plus</a> receive access to <a href="@sla-url">Mollom\'s high-availability backend infrastructure</a>, not available to free users, reducing potential downtime.', array(
      '@pricing-url' => 'http://mollom.com/pricing',
      '@sla-url' => 'http://mollom.com/standard-service-level-agreement',
    )),
  );

  $form['mollom_privacy_link'] = array(
    '#type' => 'checkbox',
    '#title' => t("Link to Mollom's privacy policy on forms protected by textual analysis"),
    '#return_value' => 1,
    '#default_value' => variable_get('mollom_privacy_link', 1),
    '#description' => t('Displays a link to the recommended <a href="@privacy-policy-url">privacy policy on mollom.com</a> on all forms that are protected via <a href="@help-url">textual analysis</a>. When disabling this option, you are required to inform visitors about data privacy through other means, as stated in the <a href="@terms-of-service-url">terms of service</a> applying to your subscription.', array(
      '@privacy-policy-url' => 'http://mollom.com/web-service-privacy-policy',
      '@help-url' => url('admin/help/mollom'),
      '@terms-of-service-url' => 'http://mollom.com/terms-of-service',
    )),
  );

  $form['mollom_testing_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable testing mode'),
    '#default_value' => variable_get('mollom_testing_mode', 0),
    '#description' => t('Submitting "ham", "unsure", or "spam" on a protected form will trigger the corresponding behavior, and similarly, word verifications will only respond to "correct" and "incorrect", instead of the actual characters asked for. This option should be disabled in production environments.'),
  );

  return system_settings_form($form);
}

/**
 * Form submit handler to mass-report and unpublish or delete comments.
 */
function mollom_comment_admin_overview_submit($form, &$form_state) {
  if (strpos($form_state['values']['operation'], 'mollom-') === 0) {
    list($id, $operation) = explode('-', $form_state['values']['operation']);
    foreach ($form_state['values']['comments'] as $cid => $value) {
      if ($value) {
        // First, report the comments as spam to Mollom.com.
        if ($data = mollom_data_load('comment', $cid)) {
          _mollom_send_feedback($data->session);
        }

        // Second, perform the proper operation on the comments:
        if ($comment = _comment_load($cid)) {
          if ($operation == 'unpublish') {
            db_query("UPDATE {comments} SET status = %d WHERE cid = %d", COMMENT_NOT_PUBLISHED, $cid);
            _comment_update_node_statistics($comment->nid);
          }
          elseif ($operation == 'delete') {
            _comment_delete_thread($comment);
            _comment_update_node_statistics($comment->nid);
          }
        }
      }
    }

    // Flush caches so the content changes are visible for anonymous users.
    cache_clear_all();

    if ($operation == 'delete') {
      drupal_set_message(t('The selected comments have been reported as inappropriate and are deleted.'));
    }
    else {
      drupal_set_message(t('The selected comments have been reported as inappropriate and are unpublished.'));
    }
  }
}

/**
 * Form submit handler to mass-report and unpublish or delete nodes.
 */
function mollom_node_admin_overview_submit($form, &$form_state) {
  if (strpos($form_state['values']['operation'], 'mollom-') === 0) {
    list($id, $operation) = explode('-', $form_state['values']['operation']);
    foreach ($form_state['values']['nodes'] as $nid => $value) {
      if ($value) {
        // First, report the nodes as spam to Mollom.com.
        if ($data = mollom_data_load('node', $nid)) {
           _mollom_send_feedback($data->session);
        }

        if ($node = node_load($nid)) {
          if ($operation == 'unpublish') {
            db_query("UPDATE {node} SET status = 0 WHERE nid = %d", $nid);
          }
          elseif ($operation == 'delete') {
            node_delete($nid);
          }
        }
      }
    }

    // Flush caches so the content changes are visible for anonymous users.
    cache_clear_all();

    if ($operation == 'delete') {
      drupal_set_message(t('The selected posts have been reported as inappropriate and are deleted.'));
    }
    else {
      drupal_set_message(t('The selected posts have been reported as inappropriate and are unpublished.'));
    }
  }
}

/**
 * Menu callback; Displays the administrative reports page.
 */
function mollom_reports_page() {
  $embed_attributes = array(
    'src' => 'http://mollom.com/statistics.swf?key=' . check_plain(variable_get('mollom_public_key', '')),
    'quality' => 'high',
    'width' => '100%',
    'height' => '430',
    'name' => 'Mollom',
    'align' => 'middle',
    'play' => 'true',
    'loop' => 'false',
    'allowScriptAccess' => 'sameDomain',
    'type' => 'application/x-shockwave-flash',
    'pluginspage' => 'http://www.adobe.com/go/getflashplayer',
  );
  $form['chart'] = array(
    '#type' => 'item',
    '#title' => t('Statistics'),
    '#value' => '<embed' . drupal_attributes($embed_attributes) . '></embed>',
  );
  if (module_exists('dblog')) {
    $logs = array();
    $query = db_query_range("SELECT message, variables FROM {watchdog} WHERE type = 'mollom' AND severity <= %d ORDER BY wid DESC", WATCHDOG_WARNING, 0, 10);
    while ($log = db_fetch_object($query)) {
      $t_args = unserialize($log->variables);
      $logs[] = t($log->message, ($t_args ? $t_args : array()));
    }
    $form['watchdog'] = array(
      '#type' => 'item',
      '#title' => t('Recent Mollom messages'),
      '#value' => $logs ? theme('item_list', $logs) : t('None'),
    );
  }
  return $form;
}

