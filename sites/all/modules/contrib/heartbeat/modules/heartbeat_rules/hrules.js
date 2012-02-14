/**
 * Callbadk to load the variables
 */
function heartbeat_rule_get_variables(element) {
  var message_id = $(element).val();
  var callback = Drupal.settings.basePath + 'heartbeat/heartbeat_activity_rules_default/js';
  $.getJSON(callback, {message_id: message_id}, function(data) {
    $('#message-variables').hide('slow');
    $('#message-variables').find('textarea').val(data.data);
    $('#message-message').text(data.message);
    $('#message-variables').show('slow');
  });
}
