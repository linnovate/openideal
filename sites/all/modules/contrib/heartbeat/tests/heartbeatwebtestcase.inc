<?php
// $Id $

/**
 * Class HeartbeatAdminTools
 *
 */
class HeartbeatWebTestCase extends DrupalWebTestCase {

  protected $testRoles = array(
    'administer content types',
    'access content',
    'view revisions',
    'administer nodes',
    'access user profiles',
    'view heartbeat messages',
    'configure heartbeat messages',
    'view personal heartbeat activity'
  );
  protected $templatesLoaded = FALSE;

  /**
   * Implementation of tearDown().
   */
  function tearDown() {
    parent::tearDown();
  }

  /**
   * Debug helper function. Writes values away to a text file in the files directory.
   */
  protected function debugHelper($value, $mode = 'a+') {
    $debug = fopen($this->originalFileDirectory .'/simpletestdebug.txt', $mode);
    fwrite($debug, print_r($value, TRUE) ."\n");
    fclose($debug);
  }

  /**
   * Configure a stream to start off as the administer/webmaster would do.
   */
  protected function configureStream($stream_name, $args) {

    if (!$this->templatesLoaded) {
      // Make sure message templates are imported. This should not be needed.
      $this->drupalGet('admin/build/heartbeat');
      $this->templatesLoaded = TRUE;
    }

    // Test the configuration form for private heartbeat.
    $this->drupalPost('admin/build/heartbeat/stream/'. $stream_name, $args, t('Submit'));

  }

}