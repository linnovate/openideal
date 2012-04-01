
/**
 * @file
 * Heartbeat Node.js javascript functions.
 */

(function($) {

 /**
  * Grab the messages from the Node.js service and append it to the streams.
  */
 Drupal.Nodejs.callbacks.heartbeatNotify = {
   callback: function (message) {

     // Remove message from stream.
     if (message.data.subject == 'heartbeat-delete-message') {
       $('.heartbeat-messages-wrapper .heartbeat-activity-' + message.data.body).remove();
     }
     else {
       // Append the messages
       var new_messages = $('.heartbeat-messages-wrapper').prepend(message.data.body);
  
       // Reattach behaviors for new added html.
       Drupal.attachBehaviors(new_messages);
     }
   }
 };

})(jQuery);