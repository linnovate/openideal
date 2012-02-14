<?php
// $Id: mollom.pages.inc,v 1.1.2.16 2010/08/07 02:49:44 dries Exp $

/**
 * @file
 * Various non-administration page callbacks for the mollom module.
 */

/**
 * AJAX callback to retrieve a CAPTCHA.
 *
 * @param $type
 *   The new CAPTCHA type to retrieve, e.g. 'image' or 'audio'.
 * @param $session_id
 *   The last known Mollom session id contained in the form.
 *
 * @return
 *   A JSON array containing:
 *   - content: The HTML markup for the new CAPTCHA.
 *   - session_id: The Mollom session id for the new CAPTCHA.
 *
 * @todo Add error handling.
 */
function mollom_captcha_js($type, $session_id) {
  // Extract Mollom session id from form element value.
  @list($timestamp, $mollom_session_id) = explode('-', $session_id, 2);
  if (empty($mollom_session_id)) {
    watchdog('mollom', 'Bogus session id %session.', array('%session' => $session_id), WATCHDOG_WARNING);
    drupal_json();
    exit();
  }

  $captcha = mollom_get_captcha($type, array('session_id' => $mollom_session_id));

  // Update cached session id in the cached $form_state.
  if (!empty($captcha['response']['session_id'])) {
    if ($cache = cache_get($mollom_session_id, 'cache_mollom')) {
      $form_state['mollom'] = $cache->data;
      $form_state['mollom']['response']['session_id'] = $captcha['response']['session_id'];
      cache_set($form_state['mollom']['response']['session_id'], $form_state['mollom'], 'cache_mollom', $timestamp + 21600);
      // After successfully updating the cache, replace the original session id.
      $mollom_session_id = $captcha['response']['session_id'];
    }
  }

  // Return new content and new session_id via JSON.
  $data = array(
    'content' => $captcha['markup'],
    'session_id' => $timestamp . '-' . $mollom_session_id,
  );
  drupal_json($data);
  exit();
}

/**
 * Form builder for report to Mollom form.
 *
 * @param $entity
 *   The entity type of the data to report, e.g. 'node' or 'comment'.
 * @param $id
 *   The entity id the data belongs to. If 'session' is passed as $entity, then
 *   $id is assumed to be a Mollom session_id, as returned by Mollom servers,
 *   which should only be used to report session data that was not stored for an
 *   entity in the database (such as contact form submissions).
 */
function mollom_report_form(&$form_state, $entity, $id) {
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $form['feedback'] = array(
    '#type' => 'radios',
    '#title' => t('Optionally report this to Mollom'),
    '#options' => array(
      'none' => t("Don't send feedback to Mollom"),
      'spam' => t('Report as spam or unsolicited advertising'),
      'profanity' => t('Report as obscene, violent or profane content'),
      'low-quality' => t('Report as low-quality content or writing'),
      'unwanted' => t('Report as unwanted, taunting or off-topic content'),
    ),
    '#default_value' => 'none',
    '#description' => t("Mollom is a web service that helps you moderate your site's content: see <a href=\"http://mollom.com\">http://mollom.com</a> for more information. By sending feedback to Mollom, you teach Mollom about the content you like and dislike, allowing Mollom to do a better job helping you moderate your site's content. If you want to report multiple posts at once, you can use Mollom's bulk operations on the content and comment administration pages."),
  );

  // @todo "Delete" does not work for reporting mails to Mollom. In D7+, this
  //   form should be used solely for mails, as other entities are reported
  //   through existing delete confirmation forms instead. Perhaps there should
  //   be a dedicated form for reporting mails, as they are not really
  //   compatible with any of the standard processes either way.
  return confirm_form($form,
    t('Are you sure you want to delete and report the content as inappropriate?'),
    isset($_GET['destination']) ? $_GET['destination'] : '<front>',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Form submit handler for mollom_report_form().
 */
function mollom_report_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity = $form_state['values']['entity'];
    $id = $form_state['values']['id'];

    // Load the Mollom session data.
    if ($entity == 'session') {
      $data = new stdClass;
      $data->session = $id;
    }
    else {
      $data = mollom_data_load($entity, $id);
    }

    // Send feedback to Mollom, if we have session data.
    if (isset($data->session) && isset($form_state['values']['feedback']) && $form_state['values']['feedback'] != 'none') {
      // @todo Check the actual reponse.
      _mollom_send_feedback($data->session, $form_state['values']['feedback']);
      drupal_set_message(t('The content was successfully reported as inappropriate.'));
    }

    // Delete the content. The callback should take care of proper deletion and
    // cache clearing on its own.
    foreach (mollom_form_list() as $form_id => $info) {
      if (!isset($info['entity']) || $info['entity'] != $entity) {
        continue;
      }
      // If there is a 'report delete callback', invoke it.
      if (isset($info['report delete callback']) && function_exists($info['report delete callback'])) {
        $function = $info['report delete callback'];
        $function($entity, $id);
        break;
      }
    }

    $form_state['redirect'] = '<front>';
  }
}
