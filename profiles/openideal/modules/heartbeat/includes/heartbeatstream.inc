<?php

/**
 * @file
 *   HeartbeatStream object is the object that takes stream
 *   configuration to create a stream of activity objects.
 *   It is the controlling organ at the pre-query, query and
 *   post-query phases.
 */


/**
 * Abstract class HeartbeatStream
 *   This base class has final template methods which are
 *   used by the derived concretes. The HeartbeatStream is a
 *   state object that is given to the HeartbeatStreamBuilder to
 *   set the access to the current request.
 */
abstract class HeartbeatStream {

  // Query object for this stream.
  protected $query = NULL;

  // Configuration object for this stream.
  public $config = NULL;

  // Well-formed activity messages.
  public $messages = array();

  // Templates available to show.
  public $templates = array();

  // Denied templates.
  protected $templates_denied = array();

  // Language at display time.
  protected $language = LANGUAGE_NONE;

  // The stream owner or activity watcher.
  protected $_uid = 0;

  // Array of runtime notices, warnings and errors.
  protected $_errors = array();

  // Indicates if there are runtime errors.
  protected $_has_errors = FALSE;

  // Indicates whether the page has modal requirement.
  protected $needsModal = TRUE;

  // Time where activity starts.
  protected $_offset_time = 0;

  // Maximum time where activity must end.
  protected $oldest_date = 604800;

  // Maximum number of activity messages to show.
  protected $messages_max = 0;

  // Latest user activity id fetched.
  protected $latest_activity_id = 0;

  // Indicates if the stream is displayed on a page or not.
  protected $_page = FALSE;

  // Indicates if this is an ajax request.
  protected $ajax = 0;

  // Can page means if we can show more messages
  protected $canPage = FALSE;

  // User viewer types.
  const VIEWER = 0;
  const VIEWED = 1;

  // User view type of the stream instance.
  protected $_whoisuser_type = self::VIEWER;

  // The user who is viewing the activity stream.
  protected $viewer = null;

  // The user who's activity stream is viewed.
  protected $viewed = null;


  protected $_whoisuser_types = array(
    self::VIEWER => 'Viewing user',
    self::VIEWED => 'Viewed user'
  );

  /**
   * Constructor
   *
   * @param $stram HeartbeatStreamConfig object with the neccesairy parameters
   * @param $page Boolean to indicate if this is a page view
   */
  final public function __construct(HeartbeatStreamConfig $streamConfig, $page = FALSE, $account = NULL) {

    $this->_page = $page;
    $this->setConfig($streamConfig);
    $this->setAjax();

    if (empty($this->_offset_time)) {
      $this->setOffsetTime();
    }

    $this->setViewer($GLOBALS['user']);
    $this->setViewed($account);

    $this->setAvailableTemplates();

    $this->construct();

  }

  /**
   * Fake constructor to hook this method instead of the constructor.
   */
  public function construct() {

  }

  /**
   * setError
   * Alias for addError.
   *
   * @param $text String of found error
   * @return void
   */
  protected function setError($text) {
    $this->_addError($text);
  }

  /**
   * addError
   *
   * @param $text String of found error
   * @return void
   */
  protected function addError($error) {
    $this->_errors[] = $error;
    $this->_has_errors = TRUE;
  }

  /**
   *
   * @param $text
   * @return unknown_type
   */
  public function getErrors() {
    return $this->_errors;
  }

  /**
   * hasErrors
   *
   * @return boolean has errors
   */
  public function hasErrors() {
    return variable_get('heartbeat_debug', 0) && $this->_has_errors;
  }

  /**
   * hasAccess
   *
   * @param $text
   * @return boolean to indicate the access to the stream
   */
  protected function hasAccess() {
    return TRUE;
  }

  /**
   * getCurrentLanguages().
   *
   * Get the current global language.
   */
  public function getCurrentLanguages() {

    global $language;
    $this->language = $language->language;
    $languages = array('und' => 'und');
    // TODO check if we are going to use no languages at all at log time
    $languages[$language->language] = $language->language;

    return $languages;

  }

  /**
   * skipActiveUser().
   *
   * Return whether you want to skip the active user (being
   * the logged-in user and NOT the displayed user) from display.
   * Typical private will not skip this one ofcourse where most
   * other will skip active user since you don't want to watch
   * your own activity.
   */
  public function skipActiveUser() {
    return $this->config->skip_active_user;
  }

  /**
   * getViewer().
   *
   * Function to retrieve the viewer of the stream.
   */
  public function getViewer() {
    return $this->viewer;
  }

  /**
   * setViewer().
   */
  public function setViewer($account) {
    $this->viewer = $account;
  }

  /**
   * getViewed().
   *
   * Function to retrieve the user acting in the stream.
   */
  public function getViewed() {
    return $this->viewed;
  }

  /**
   * setViewed().
   */
  public function setViewed($account) {

    // If an account is given, then the stream is build for that user.
    if (isset($account)) {
      $this->_whoisuser_type = self::VIEWED;
      $this->viewed = $account;
    }
    else {
      $this->_whoisuser_type = self::VIEWER;
      $this->viewed = $this->viewer;
    }
    $this->_uid = $this->viewed->uid;

  }

  /**
   * isPage().
   *
   * Getter function for heartbeat page/blocks.
   */
  public function isPage() {
    return $this->_page;
  }

  /**
   * setIsPage().
   *
   * @param Boolean $isPage
   */
  public function setIsPage($isPage) {
    $this->_page = $isPage;
  }

  /**
   * getLatestActivityId()
   */
  public function getLatestActivityId() {
    return $this->latest_activity_id;
  }

  /**
   * setLatestActivityId().
   */
  public function setLatestActivityId($id) {
    $this->latest_activity_id = $id;
  }

  /**
   * getAjax().
   */
  public function getAjax() {
    return $this->ajax;
  }

  /**
   * isAjax().
   *
   * Alias for getAjax.
   */
  public function isAjax() {
    return $this->ajax;
  }

  /**
   * setAjax().
   */
  public function setAjax($ajax = NULL) {

    if (isset($ajax)) {
      $this->ajax = $ajax;
    }
    elseif ($this->_page) {
      $this->ajax = $this->config->page_pager_ajax;
    }
    else {
      $this->ajax = ($this->config->block_show_pager == 2 || $this->config->block_show_pager == 3);
    }

  }

  /**
   * needsModal().
   * Returns a boolean to indicate whether modal window is needed on the page.
   */
  public function needsModal($needsModal = NULL) {
    if (isset($needsModal)) {
      $this->needsModal = $needsModal;
    }
    return $this->needsModal;
  }

  /**
   * Set the maximum number of items to fetch.
   */
  public function setItemsMax() {

    $this->oldest_date = variable_get('heartbeat_activity_log_cron_delete', 2678400);

    if ($this->_page) {
      $this->messages_max = $this->config->page_items_max;
    }
    else {
      $this->messages_max = $this->config->block_items_max;
    }
  }

  /**
   * getConfig().
   *
   * Get HeartbeatStreamConfig object with all configurations
   *
   * @return HeartbeatStreamConfig object
   */
  public function getConfig() {
    return $this->config;
  }

  /**
   * setConfig().
   *
   * @param HeartbeatStreamConfig object
   */
  public function setConfig(HeartbeatStreamConfig $config) {
    $this->config = $config;
    $this->setItemsMax();
  }

  /**
   * Get the messages.
   */
  public function getMessages() {
    return $this->messages;
  }

  /**
   * setLanguage().
   */
  public function setLanguage($language) {
    $this->language = $language;
  }

  /**
   * setOffsetTime
   *
   * @param $offset integer to set the offset time
   * @return void
   */
  final public function setOffsetTime($offset = 0) {
    if ($offset == 0) {
      $offset = $_SERVER['REQUEST_TIME'];
    }
    $this->_offset_time = $offset;
  }

  /**
   * getOffsetTime
   *
   * @param $offset integer to set the offset time
   * @return void
   */
  final public function getOffsetTime() {
    return $this->_offset_time;
  }

  /**
   * getTemplate().
   */
  final public function getTemplate($template_id, $access = NULL) {

    if (isset($access) && isset($this->templates[$access][$template_id])) {
      return $this->templates[$access][$template_id];
    }
    else {
      foreach ($this->templates as $perm => $templates) {
        if (isset($templates[$template_id])) {
          return $templates[$template_id];
        }
      }
      return NULL;
    }

  }

  /**
   * result
   * Prepares a query, makes it available to alter it and finally executes it.
   * @return Array of messages as a result of the query.
   */
  public function result() {

    if (!$this->hasAccess()) {
      drupal_access_denied();
      exit();
    }
    //$this->prepareArguments();
    $this->createQuery();
    $this->queryAlter();

    $messages = $this->executeQuery();

    // Check on xss attack before other modules can change messages.
    // Delegate the time.
    foreach ($messages as $key => $message) {
      $messages[$key]->message = filter_xss($messages[$key]->message);
      $messages[$key]->message_concat = filter_xss($messages[$key]->message_concat);
      $messages[$key]->show_message_times_grouped = $this->config->show_message_times_grouped;
    }

    return $messages;

  }

  /**
   * setAvailableTemplates()
   */
  protected function setAvailableTemplates() {

    // Load the available templates for this stream.
    ctools_include('export');
    $templates = ctools_export_crud_load_all('heartbeat_messages');

    // Deny messages that have been denied through stream configuration.
    if (!empty($this->config->messages_denied)) {
      foreach ($this->config->messages_denied as $denied_message) {
        $this->templates_denied[$denied_message] = $denied_message;
      }
    }

    // Prepare the allowed templates for this stream.
    foreach ($templates as $key => $template) {

      // Restrict templates to roles.
      if ($template->hasRoleRestrictions()) {
        foreach ($template->roles as $rid) {
          if (!isset($this->viewer->roles[$rid])) {
            $this->templates_denied[$key] = $key;
            break;
          }
        }
        if (isset($this->templates_denied[$key])) {
          continue;
        }
      }

      $this->templates[$template->perms][$template->message_id] = $template;
      //$this->templates[$template->message_id] = $template;

    }

  }

  /**
   * createQuery
   *
   * @param $text
   * @return HeartbeatParser object
   */
  protected function createQuery() {

    $this->query = db_select('heartbeat_activity', 'ha')->extend('PagerActivity');
    $this->query->fields('ha');
    $this->query->addField('ha', 'variables', 'variables');
    $this->query->where(" ha.language IN (:languages) ", array(':languages' => $this->getCurrentLanguages()));

    // Condition on time of creation.
    if ($this->latest_activity_id > 0) {
      // Calls with an offset uaid will fetch newer messages
      $this->query->setLastActivityId($this->latest_activity_id);
    }
    else {
      // Otherwise we'll set oldest and newest date.
      $this->query->setOffsetTime($this->_offset_time, $this->oldest_date);
    }

    // Condition to skip the active user or not.
    if ($this->skipActiveUser()) {
      $this->query->condition("ha.uid", $this->viewer->uid);
    }

    // Add access checks for each template permission specific.
    $or = db_or();
    foreach ($this->templates as $perm => $templates) {
      switch ($perm) {
        case HEARTBEAT_PUBLIC_TO_CONNECTED:
          $this->viewed->relations = heartbeat_related_uids($this->viewed->uid);
          //drupal_set_message('HEARTBEAT_PUBLIC_TO_CONNECTED: ' . implode(', ', array_keys($templates)) . ' ____ ' . implode(' - ', $this->viewed->relations));
          $or->condition(db_and()
            ->condition('ha.uid', $this->viewed->relations, 'IN')
            ->condition('ha.access', $perm, '>=')
            ->condition('ha.message_id', array_keys($templates), 'IN')
          );
          break;
        case HEARTBEAT_PUBLIC_TO_ADDRESSEE:
          //drupal_set_message('HEARTBEAT_PUBLIC_TO_ADDRESSEE: ' . implode(', ', array_keys($templates)));
          $or->condition(db_and()
            ->condition(db_or()
              ->condition('ha.uid', $this->viewer->uid)
              ->condition('ha.uid_target', $this->viewer->uid)
            )
            ->condition('ha.uid', 0, '!=')
            ->condition('ha.uid_target', 0, '!=')
            ->condition('ha.access', $perm)
            ->condition('ha.message_id', array_keys($templates), 'IN')
          );
          break;
        case HEARTBEAT_PRIVATE:
          //drupal_set_message('HEARTBEAT_PRIVATE: ' . implode(', ', array_keys($templates)));
          $or->condition(db_and()
            ->condition('ha.uid', $this->viewer->uid)
            ->condition('ha.access', $perm)
            ->condition('ha.message_id', array_keys($templates), 'IN')
          );
          break;
        case HEARTBEAT_PUBLIC_TO_ALL:
        default:
          //drupal_set_message('HEARTBEAT_PUBLIC_TO_ALL: ' . implode(', ', array_keys($templates)));
          $or->condition(db_and()
            ->condition('ha.access', $perm)
            ->condition('ha.message_id', array_keys($templates), 'IN')
          );
      }
    }
    if (count($or->conditions())) {
      $this->query->condition($or);
    }

    if (!empty($this->templates_denied)) {
      $this->query->where("ha.message_id NOT IN (:messages)", array(':messages' => array_unique($this->templates_denied)));
    }

    // Order by.
    $this->query->orderBy('ha.timestamp', 'DESC')->limit($this->config->num_load_max);
    $this->query->orderBy('ha.uaid', 'DESC');

  }

  /**
   * executeQuery
   *
   * @return array results
   */
  protected function executeQuery() {

    //drupal_set_message(strtr((string) $this->query, $this->query->getArguments()));
    $result = $this->query->execute()->fetchAll();
    $messages = array();

    foreach ($result as $row) {

      if ($template = $this->getTemplate($row->message_id)) {

        $heartbeatactivity = new HeartbeatActivity($row, $template);
        $heartbeatactivity->count = 1;

        $messages[$row->uaid] = $heartbeatactivity;
        HeartbeatMessagePool::getInstance()->addMessage($heartbeatactivity);

      }
    }

    return $messages;
  }

  /**
   * processViewsQuery().
   *
   * Method to represent the messages if comes from views.
   */
  public function processViewsQuery($values = NULL) {

    $activities = heartbeat_activity_load_multiple($values);
    if (empty($activities)) {
      return;
    }

    // Filter messages by permission, skipped for views for now.
    // $this->checkAccess($activities);

    // Give contributes modules the opportunity to load
    // additions for the heartbeat activity message object
    $hook = 'heartbeat_load';
    foreach (module_implements($hook) as $module) {
      $function = $module .'_'. $hook;
      // $messages should be implemented by reference!!!
      $function($activities, $this);
    }

    // Group the activity messages as configured
    $messages = $this->parseMessages($activities);

    // Let other modules retheme or completely rebuild messages
    $hook = 'heartbeat_theme_alter';
    foreach (module_implements($hook) as $module) {
      $function = $module .'_'. $hook;
      $result = $function($messages, $this);
    }

    $this->messages = $messages;

    $messages = array();
    foreach ($this->messages as $message) {
      $messages[$message->uaid] = $message;
    }

    return $messages;

  }

  /**
   * Alter the query object.
   */
  protected function queryAlter() {}

  /**
   * viewsQueryAlter
   *
   * @param $view Object View
   * @return void
   */
  protected function viewsQueryAlter(&$view) {}

  /**
   * Function to check access on messages
   * This behaviour is set by a heartbeat message configuration
   * to overrule the chosen display access type
   */
  protected function checkAccess(& $messages) {

    // First check access on message perms, roles and node access.
    foreach (array_keys($messages) as $key) {

      $message = & $messages[$key];

      try {

        // Skip messages with no reason given.
        if (!$message->hasAccess($this->viewer, $this)) {
          $this->addError('Activity message #'. $message->uaid . ' is not allowed in the stream. No reason given.');
          unset($messages[$key]);
          continue;
        }

      }
      // Skip messages while adding the error.
      catch (HeartbeatInvalidStreamException $e) {
        $this->addError($e->getMessage());
        unset($messages[$key]);
        continue;
      }

    }

    // This hook is invoked BEFORE calculating the maximum
    // number of messages to show,
    // giving other modules the opportunity to remove messages
    // based on their own parameters and custom reasons.
    $hook = 'heartbeat_messages_alter';
    foreach (module_implements($hook) as $module) {
      $function = $module .'_'. $hook;
      $result = $function($messages, $this->_heartbeatState);
    }

    // The difficulty remains in having the possibility to leave the
    // user with no site activity at display. Any propositions?
    // In short: problem when querying to few messages, having lots
    // of them denied and leaving the user with no messages left.

  }

  /**
   * Function to check if more/older messages can be loaded
   *
   * @return Boolean has more messages
   */
  public function hasMoreMessages() {

    if ($this->isPage()) {
      $display_pager = $this->config->page_show_pager;
    }
    else {
      $display_pager = $this->config->block_show_pager;
    }

    return $this->canPage && $display_pager;
  }

  /**
   * Create the well-formed activity messages from a result.
   * HeartbeatParser will do most of the work here.
   */
  public function parseMessages($result) {

    $heartbeatparser = new HeartbeatParser();
    $heartbeatparser->set_timespan_gap($this->config->grouping_seconds);
    $heartbeatparser->build_sets($result);
    $heartbeatparser->merge_sets();
    $messages = $heartbeatparser->get_messages();

    // $messages = $heartbeatparser->remove_broken_messages();
    $num_total_messages = count($messages);

    // From here we know the number of messages actualy loaded (and allowed)
    $messages = array_slice($messages, 0, $this->messages_max);

    // Set the possibility of a pager appearence
    if ($num_total_messages > $this->messages_max) {
      $this->canPage = TRUE;
    }

    return $messages;
  }

  /**
   * Function that reorganizes a query result of messages
   *   into a stream of heartbeat activity objects.
   *
   * @return $messages
   *   array of messages
   */
  public function execute() {

    // Fetch the messages from database for current Stream
    $result = $this->result();

    if (empty($result)) {
      return array();
    }

    // Filter messages by permission
    $this->checkAccess($result);

    // Give contributes modules the opportunity to load
    // additions for the heartbeat activity message object
    $hook = 'heartbeat_load';
    foreach (module_implements($hook) as $module) {
      $function = $module .'_'. $hook;
      // $messages should be implemented by reference!!!
      $function($result, $this);
    }

    // Group the activity messages as configured
    $messages = $this->parseMessages($result);

    // Let other modules retheme or completely rebuild messages
    $hook = 'heartbeat_theme_alter';
    foreach (module_implements($hook) as $module) {
      $function = $module .'_'. $hook;
      $result = $function($messages, $this);
    }

    $this->messages = $messages;

    return $this->messages;

  }

}

/**
 * eof().
 */