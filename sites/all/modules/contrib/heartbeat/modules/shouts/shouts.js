
/**
 * The heartbeat object
 */
Drupal.heartbeat = Drupal.heartbeat || {};

/**
 * Handle default text of heartbeat area
 */
Drupal.behaviors.Shouts = function(context) {
  var submitField = $("#edit-shout");
  var defaultShout = submitField.val();
  submitField.focus(function() {
    if (submitField.val() == defaultShout) {
      submitField.val(""); 
    }
  });
  submitField.blur(function() {
    if (submitField.val() == "") {
       submitField.val(defaultShout);
    }
  });
}

/**
 * Shouts object
 */
Drupal.heartbeat.Shouts = Drupal.heartbeat.Shouts || {};

Drupal.heartbeat.Shouts.button = null;

/**
 * Shout function to actually shout a message into the world
 */
Drupal.heartbeat.Shouts.shout = function(element, callback_url) {

  Drupal.heartbeat.Shouts.button = $(element);
  
  // Throw in the throbber
  Drupal.heartbeat.wait($('.heartbeat-messages-throbber'), '.shouts-form-wrapper');
  Drupal.heartbeat.Shouts.button.attr("disabled", "disabled");
  
  var field = $(element).parents('form').find('.shout-message:first');
  var shout = field.val();
  field.val('');
  
  $.post(Drupal.settings.basePath + callback_url, {shout: shout}, Drupal.heartbeat.Shouts.afterShout);
  
}

/**
 * Callback triggered after a shout. 
 * We do a poll for newer messages to put the shout into the stream(s)
 */
Drupal.heartbeat.Shouts.afterShout = function(data) {

  Drupal.heartbeat.doneWaiting();
  Drupal.heartbeat.Shouts.button.removeAttr("disabled");
  
  $(".heartbeat-stream").each(function() {
    Drupal.heartbeat.pollMessages($(this).attr('id').replace("heartbeat-stream-", ""));
  });
  
}