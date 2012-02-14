/**
 * Heartbeat comments object
 */
Drupal.heartbeat = Drupal.heartbeat || {};

Drupal.heartbeat.comments = Drupal.heartbeat.comments || {};

Drupal.heartbeat.comments.button = null;

/**
 * Attach behaviours to the message streams
 */
Drupal.behaviors.heartbeat_comments = function (context) {
  $('.heartbeat-comment-submit', context).each(function() {
    $(this).bind('click', function(e) {
      return Drupal.heartbeat.comments.submit(this); 
    });
  });
}

Drupal.heartbeat.comments.submit = function(element) {

  // Throw in the throbber
  Drupal.heartbeat.comments.button = $(element);
  Drupal.heartbeat.wait(Drupal.heartbeat.comments.button, '.heartbeat-comments-wrapper');
  Drupal.heartbeat.comments.button.attr("disabled", "disabled");
  
  var formElement = $(element).parents('form');
  
  var url = Drupal.settings.basePath + 'heartbeat/comment/post';
  var args = {
    message: formElement.find('.heartbeat-message-comment').val(), 
    uaid: formElement.find('.heartbeat-message-uaid').val(), 
    nid: formElement.find('.heartbeat-message-nid').val(), 
    node_comment: formElement.find('.heartbeat-message-node-comment').val(),
    first_comment: !$('#heartbeat-comments-list-' + formElement.find('.heartbeat-message-uaid').val()).length
  };
  $.post(url, args, Drupal.heartbeat.comments.submitted,'json');
  
  return false;
}

Drupal.heartbeat.comments.submitted = function(data) {

  if (data.id != undefined) {
    
    if (!$('#heartbeat-comments-list-' + data.id).length) {
      if (Drupal.settings.heartbeat_comments_position == 'up') {
        $('#heartbeat-comments-wrapper-' + data.id).append(data.data);
      }
      else {
        $('#heartbeat-comments-wrapper-' + data.id).prepend(data.data);
      }
    }
    else {    
      if (Drupal.settings.heartbeat_comments_order == 'oldest_on_top') {
        $('#heartbeat-comments-list-' + data.id).append(data.data);
      }
      else {
        $('#heartbeat-comments-list-' + data.id).prepend(data.data);
      }
      
      $('#heartbeat-comments-list-' + data.id).parents('.heartbeat-comments').find('.heartbeat-message-comment').val('');
    } 
    //Drupal.attachBehaviors($('.heartbeat-stream'));
    
    Drupal.heartbeat.doneWaiting();
    Drupal.heartbeat.comments.button.removeAttr("disabled");
  }
}

Drupal.heartbeat.comments.load = function (uaid, node_comment, nid) {
  var url = Drupal.settings.basePath + 'heartbeat/comments/load/js';
  $.post(url, {uaid: uaid, node_comment: node_comment, nid: nid}, Drupal.heartbeat.comments.loaded, 'json');
}

Drupal.heartbeat.comments.loaded = function(data) {

  if (data.data != undefined) {
    $('#heartbeat-comments-wrapper-' + data.uaid).html(data.data);
  }
  
}