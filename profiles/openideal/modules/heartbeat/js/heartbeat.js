(function($) {
		
	/**
	 * The heartbeat object.
	 */
	Drupal.heartbeat = Drupal.heartbeat || {};
	
	Drupal.heartbeat.moreLink = null;
	
	/**
	 * wait().
	 * Function that shows throbber while waiting a response.
	 */
	Drupal.heartbeat.wait = function(element, parentSelector) {
	
	  // We wait for a server response and show a throbber 
	  // by adding the class heartbeat-messages-waiting.
	  Drupal.heartbeat.moreLink = $(element).parents(parentSelector);
	  // Disable double-clicking.
	  if (Drupal.heartbeat.moreLink.is('.heartbeat-messages-waiting')) {      
	    return false;
	  }
	  Drupal.heartbeat.moreLink.addClass('heartbeat-messages-waiting');
	  
	}
	
	/**
	 * doneWaiting().
	 * Function that is triggered if waiting period is over, to start
	 * normal behavior again.
	 */
	Drupal.heartbeat.doneWaiting = function() {
	  Drupal.heartbeat.moreLink.removeClass('heartbeat-messages-waiting');
	}
	
	/**
	 * pollMessages().
	 *   Function that checks and fetches newer messages to the
	 *   current stream.
	 */
	Drupal.heartbeat.pollMessages = function(stream) {
	
	  var stream_selector = '#heartbeat-stream-' + stream;
	  
	  if ($(stream_selector).length > 0) {
	    
	    var href = Drupal.settings.basePath + 'heartbeat/js/poll';
	    var uaids = new Array();
	    var beats = $(stream_selector + ' .beat-item');
	    var firstUaid = 0;
	    
	    if (beats.length > 0) {    
	      firstUaid = $(beats.get(0)).attr('id').replace("beat-item-", "");
	      
	      beats.each(function(i) {  
	        var uaid = parseInt($(this).attr('id').replace("beat-item-", ""));
	        uaids.push(uaid);
	      });
	    }
	    
	    var post = {latestUaid: firstUaid, language: Drupal.settings.heartbeat_language, stream: stream, uaids: uaids.join(',')};
		  
	    $.event.trigger('heartbeatBeforePoll', [post]);
	    
	    if (firstUaid) {
  		  $.ajax({
  		    url: href,
  		  	dataType: 'json',
  		    data: post,
  		    success: Drupal.heartbeat.prependMessages
  		  });
	    }
	    
	  }
	  
	}
	
	/**
	 * prependMessages().
	 *   Append messages to the front of the stream. This done for newer 
	 *   messages, often with the auto poller.
	 */
	Drupal.heartbeat.prependMessages = function(data) {
		  
	  if (data.data != '') {
	    var stream_selector = '#heartbeat-stream-' + data.stream;
	    // Append the messages
	    var new_messages = $(stream_selector + ' .heartbeat-messages-wrapper').prepend(data.data);
	  
	    // Update the times in the stream
	    var time_updates = data.time_updates;
	    for (uaid in time_updates) {
	      $(stream_selector + ' #beat-item-' + uaid).find('.heartbeat_times').text(time_updates[uaid]);
	    }
	    
	    // Reattach behaviors for new added html.
	    Drupal.attachBehaviors(new_messages);
	  }
	  
	}
	
	/**
	 * getOlderMessages().
	 *   Fetch older messages with ajax.
	 */
	Drupal.heartbeat.getOlderMessages = function(element, page) {
	  
	  Drupal.heartbeat.wait(element, '.heartbeat-more-messages-wrapper');
	  $.ajax({
	    url: element.href,
	    dataType: 'json',
	    data: {block: page ? 0 : 1, ajax: 1},
	    success: Drupal.heartbeat.appendMessages
	  });
	  
	}
	
	/**
	 * appendMessages().
	 * 
	 * Function that appends older messages to the stream.
	 */
	Drupal.heartbeat.appendMessages = function(data) {
	  
	  var wrapper = Drupal.heartbeat.moreLink.parents('.heartbeat-messages-wrapper');
	  Drupal.heartbeat.moreLink.remove();
	  var new_messages = wrapper.append(data.data);
	  Drupal.heartbeat.doneWaiting();
	    
	  // Reattach behaviors for new added html
	  Drupal.attachBehaviors(new_messages);
	  
	}

	/**
	 * jQuery.heartbeatRemoveElement().
	 * Remove element.
	 */
	$.fn.heartbeatRemoveElement = function (id, text) {
	  var element = $(this[0]);
	  var height = element.height();
	  
	  setTimeout(function() { element.css({height: height}).html(text); }, 600);
	  element.effect("highlight", {}, 2000, function() {
	    element.hide('blind', 1000, function() { element.remove(); });
	  });
	  
	}
	
	/**
	 * flagGlobalAfterLinkUpdate.
	 */
	$(document).bind('flagGlobalBeforeLinkUpdate', function(event, data) {
	  var newLine = $('.flag-message', data.newLink);
    $('.heartbeat-activity-' + data.contentId + ' .heartbeat-flag-count').html(newLine.html());
	});

  /**
   * Attach behaviours to the message streams
   */
  Drupal.behaviors.heartbeat = {
      
    attach: function (context, settings) {

      $('.beat-remaining', context).each(function() {
        $(this).click(function(e) {
          var id = $(this).attr('id');
          $('#' + id + '_wrapper').toggle('slow'); 
          return false;
        });
      });

    }
  
  };
	
	/**
	 * Document onReady().
	 * 
	 * This is a one-time on load event, not a behavior. It only delegates variables to 
	 * start intervals for polling new activity.
	 */
	$(document).ready(function() {
	  var span = 0;
	  if (Drupal.settings.heartbeatPollNewerMessages != undefined) {
	    for (n in Drupal.settings.heartbeatPollNewerMessages) {
	      if (parseInt(Drupal.settings.heartbeatPollNewerMessages[n]) > 0) {
	        var interval = (Drupal.settings.heartbeatPollNewerMessages[n] * 1000) + span;
	        var poll = setInterval('Drupal.heartbeat.pollMessages("' + n + '")', interval);
	        span += 100;
	      }
	    }  
	  }
	});
  
})(jQuery);


/**
* Provide the HTML to create the modal dialog.
*/
Drupal.theme.prototype.CToolsHeartbeatModal = function () {
  var html = ''

  html += '<div id="ctools-modal" class="popups-box">';
  html += '  <div class="ctools-modal-content ctools-heartbeat-modal-content">';
  html += '    <div class="popups-container">';
  html += '      <div class="modal-header popups-title">';
  html += '        <span id="modal-title" class="modal-title"></span>';
  html += '        <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
  html += '        <div class="clear-block"></div>';
  html += '      </div>';
  html += '      <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
  html += '      <div class="popups-buttons"></div>'; //Maybe someday add the option for some specific buttons.
  html += '      <div class="popups-footer"></div>'; //Maybe someday add some footer.
  html += '    </div>';
  html += '  </div>';
  html += '</div>';

  return html;

}  