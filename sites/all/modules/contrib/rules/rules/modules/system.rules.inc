<?php
// $Id: system.rules.inc,v 1.1.2.17 2009/07/20 16:22:24 fago Exp $


/**
 * @file rules integration for the system module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_event_info().
 */
function system_rules_event_info() {
  return array(
    'init' => array(
      'label' => t('User is going to view a page'),
      'module' => 'System',
      'arguments' => rules_events_global_user_argument(),
      'help' => t("Be aware that some actions might initialize the theme system. After that, it's impossible for any module to change the used theme."),
    ),
    'cron' => array(
      'label' => t('Cron maintenance tasks are performed'),
      'module' => 'System',
      'arguments' => rules_events_global_user_argument(),
    ),
  );
}

/**
 * Implementation of hook_rules_action_info().
 */
function system_rules_action_info() {
  return array(
    'rules_action_drupal_message' => array(
      'label' => t('Show a configurable message on the site'),
      'module' => 'System',
      'eval input' => array('message'),
    ),
    'rules_action_set_breadcrumb' => array(
      'label' => t('Set breadcrumb'),
      'module' => 'System',
      'eval input' => array('titles', 'paths'),
    ),
    'rules_action_mail_to_user' => array(
      'label' => t('Send a mail to a user'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('Recipient')),
      ),
      'module' => 'System',
      'eval input' => array('subject', 'message', 'from'),
    ),
    'rules_action_mail' => array(
      'label' => t('Send a mail to an arbitrary mail address'),
      'module' => 'System',
      'eval input' => array('subject', 'message', 'from', 'to'),
    ),
    'rules_action_mail_to_users_of_role' => array(
      'label' => t('Send a mail to all users of a role'),
      'module' => 'System',
      'eval input' => array('subject', 'message', 'from'),
    ),
    'rules_action_drupal_goto' => array(
      'label' => t('Page redirect'),
      'module' => 'System',
      'eval input' => array('path', 'query', 'fragment'),
      'help' => t('Enter a Drupal path, path alias, or external URL to redirect to. Enter (optional) queries after "?" and (optional) anchor after "#".'),
    ),
    'rules_action_watchdog' => array(
      'label' => t('Log to watchdog'),
      'module' => 'System',
      'eval input' => array('type', 'message', 'link'),
    ),
  );
}

function rules_action_drupal_message($settings) {
  drupal_set_message($settings['message'], $settings['error'] ? 'error' : 'status');
}

/**
 * Action Implementation: Set breadcrumb.
 */
function rules_action_set_breadcrumb($settings) {
  $titles = explode("\n", $settings['titles']);
  $paths = explode("\n", $settings['paths']);

  $trail = array(l(t('Home'), ''));
  for ($i = 0; $i < count($titles); $i++) {
    // Skip empty titles
    if ($title = trim($titles[$i])) {
      // Output plaintext instead of a link if there is a title
      // without a path.
      $path = trim($paths[$i]);
      if (strlen($path) > 0 && $path != '<none>') {
        $trail[] = l($title, trim($paths[$i]));
      }
      else {
        $trail[] = check_plain($title);
      }
    }
  }
  drupal_set_breadcrumb($trail);
}

/**
 * Action Implementation: Send a mail to a user.
 */
function rules_action_mail_to_user($user, $settings) {
  // We also handle $settings['to'] if it's set.
  $to = isset($settings['to']) ? str_replace(array("\r", "\n"), '', $settings['to']) : $user->mail;
  $from = ($settings['from']) ? str_replace(array("\r", "\n"), '', $settings['from']) : NULL;

  $message = drupal_mail('rules', 'rules_action_mail', $to, language_default(), $settings, $from);
  if ($message['result']) {
    watchdog('rules', 'Successfully sent email to %recipient', array('%recipient' => $to));
  }
}

/**
 * Implementation of hook_mail().
 *
 * Set's the message subject and body as configured in the $settings of the action.
 */
function rules_mail($key, &$message, $settings) {

  $message['subject'] .= str_replace(array("\r", "\n"), '', $settings['subject']);
  $message['body'][] = $settings['message'];
}

/**
 * Action Implementation: rules_action_mail
 * This action makes use of the rules_action_mail_to_user action implementation.
 */
function rules_action_mail($settings) {
  rules_action_mail_to_user(NULL, $settings);
}

/**
 * Action: Send mail to all users of a specific role group(s).
 */
function rules_action_mail_to_users_of_role($settings) {
  $recipients = $settings['recipients'];
  $from = ($settings['from']) ? str_replace(array("\r", "\n"), '', $settings['from']) : NULL;

  // All authenticated users, which is everybody.
  if (in_array(DRUPAL_AUTHENTICATED_RID, $recipients)) {
    $result = db_query('SELECT mail FROM {users} WHERE uid > 0');
  }
  else {
    $rids = implode(',', $recipients);
    // Avoid sending emails to members of two or more target role groups.
    $result = db_query('SELECT DISTINCT u.mail FROM {users} u INNER JOIN {users_roles} r ON u.uid = r.uid WHERE r.rid IN ('. $rids .')');
  }

  // Now, actually send the mails.
  $message = array('result' => TRUE);
  while (($account = db_fetch_object($result)) && $message['result']) {
    $message = drupal_mail('rules', 'rules_action_role_mail', $account->mail, language_default(), $settings, $from);
  }
  if ($message['result']) {
    $roles = array_intersect_key(user_roles(TRUE), drupal_map_assoc($recipients));
    watchdog('rules', 'Successfully sent email to the role(s) %roles.', array('%roles' => implode(', ', $roles)));
  }
}

/**
 * Action Implementation: Page redirect.
 */
function rules_action_drupal_goto($settings) {
  // For settings backward compatibility respect the override setting as default
  $settings += array('immediate' => isset($settings['override']) && !$settings['override']);
  extract($settings);

  if ($force) {
    if (strpos($query, 'destination') === FALSE) {
      if (!empty($query)) {
        $query .= '&';
      }
      // Keep the current destination parameter if there is one.
      $query .= isset($_REQUEST['destination']) ? 'destination='. urlencode($_REQUEST['destination']) : NULL;
    }
    unset($_REQUEST['destination']);
  }
  if ($immediate) {
    drupal_goto($path, $query ? $query : NULL, $fragment ? $fragment : NULL);
  }
  else {
    // If someone else issues a drupal_goto(), we hijack it by setting the destination parameter
    // But obey any already set destination parameter.
    if (!isset($_REQUEST['destination'])) {
      $_REQUEST['destination'] = urlencode($path) . ($query ? '?'. urlencode($query) : '') . ($fragment ? '#'. urlencode($fragment) : '');
    }
    // This lets _rules_action_drupal_goto_handler() invoke drupal_goto before the page is output.
    $GLOBALS['_rules_action_drupal_goto_do'] = TRUE;
  }
}

/**
 * Page template preprocess handler for the drupal goto action.
 *
 * @see rules_theme_registry_alter().
 */
function _rules_action_drupal_goto_handler($variables) {
  if (isset($GLOBALS['_rules_action_drupal_goto_do'])) {
    unset($GLOBALS['_rules_action_drupal_goto_do']);
    // We have already set $_REQUEST['destination'] which drupal_goto() is going to respect.
    drupal_goto('');
  }
}

/**
 * Action Implementation: Log to watchdog.
 */
function rules_action_watchdog($settings) {
  watchdog($settings['type'], $settings['message'], NULL, $settings['severity'], $settings['link']);
}

/**
 * @}
 */
