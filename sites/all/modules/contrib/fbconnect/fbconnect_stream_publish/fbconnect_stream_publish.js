// $Id: fbconnect_stream_publish.js,v 1.1.2.2 2010/08/24 19:24:09 vectoroc Exp $

Drupal.fbconnect = Drupal.fbconnect || {}; 
Drupal.fbconnect.stream_publish = function() {
  Drupal.behaviors.fbconnect_stream_publish = Drupal.fbconnect.stream_publish;
  if (Drupal.settings['FB.streamPublish']) {
    FB.ui(Drupal.settings['FB.streamPublish']);
    delete Drupal.settings['FB.streamPublish'];
  }	
};

jQuery(document).bind('fb:init', Drupal.fbconnect.stream_publish);