/**
 * Watcher JavaScript
 * 
 * By Jakob Persson of NodeOne
 */

/**
 * Register Watcher behavior
 */
Drupal.behaviors.watcher = function (context) {
	$('.watcher_binder_send_email_status_icon:not(.watcher-processed)', context)
    .click( function () { 
      ajaxToggleEmailNotifications(this, this.href); 
      return false;
    })
		.addClass('watcher-processed');
  $('.watcher_node_toggle_watching_link:not(.watcher-processed)', context)
    .click( function () {
      ajaxToggleWatchFade(this, this.href);
      return false;
    })
		.addClass('watcher-processed');
}


/********************************************************************
 * EMAIL NOTIFICATION TOGGLE
 ********************************************************************/

/**
 * Handles the AJAX toggle request for email notifications
 * @param {Object} el The element that triggered the request.
 * @param {Object} url The URL to call.
 */
function ajaxToggleEmailNotifications(el, url) {
  toggleEmailIconStatusAni(el, false);
  $.ajax({
  	url: url,
	type: 'GET',
	data: { async : 'true' },
	dataType: 'json',
	timeout: 4000, //set very high to prevent time outs for users with high latency connections
	success: function (json) {
	  toggleNotificationStatus(el, json);
	},
	complete: function (json) {
	  toggleEmailIconStatusAni(el, true);
	}
  });
}

/**
 * Toggle email notification status icon
 * 
 * @param {Object} el The element to modify
 * @param {Object} rdata A JSON object containing the results of the AJAX request
 */
function toggleNotificationStatus(el, rdata) {
  var string_text_enabled = Drupal.settings.watcher.binder_notif_text_enabled;
  var string_text_disabled = Drupal.settings.watcher.binder_notif_text_disabled;
  var string_title_enabled = Drupal.settings.watcher.binder_notif_title_enabled;
  var string_title_disabled = Drupal.settings.watcher.binder_notif_title_disabled;

  if (rdata['status'] == 'enabled') {
    $(el).empty().append(string_text_enabled).attr('email_status','enabled').attr('title', string_title_enabled);
  } else {
	$(el).empty().append(string_text_disabled).attr('email_status','disabled').attr('title', string_title_disabled);
  }
}

/**
 * Display an animation to convey to the user that the action has not gone unnoticed
 * 
 * @param {Object} el The element to modify
 */
function toggleEmailIconStatusAni(el, stop) {
  if(stop) {
    $(el).removeClass('watcher_binder_send_email_status_icon_from_disabled_to_enabled'); 
	   $(el).removeClass('watcher_binder_send_email_status_icon_from_enabled_to_disabled');
    if($(el).attr('email_status')=='enabled') {
	     $(el).addClass('watcher_binder_send_email_status_icon_enabled');
  	   $(el).removeClass('watcher_binder_send_email_status_icon_disabled');
	   } else {
	     $(el).addClass('watcher_binder_send_email_status_icon_disabled');
   	  $(el).removeClass('watcher_binder_send_email_status_icon_enabled');
	   }
    return true;  	
  } 
  
		if($(el).attr('email_status')=='enabled') {
	   $(el).addClass('watcher_binder_send_email_status_icon_from_enabled_to_disabled');
  } else {
	  $(el).addClass('watcher_binder_send_email_status_icon_from_disabled_to_enabled');  	
  }
}


/********************************************************************
 * WATCH TOGGLE
 ********************************************************************/

/**
 * Handles the AJAX toggle request for watching
 * @param {Object} el The element that triggered the request.
 * @param {Object} url The URL to call.
 */
function ajaxToggleWatchFade(el, url) {
  // Fade out the element
  $(el).fadeTo(400, 0.01, function(){
  	ajaxToggleWatch(el, url)
  });
}

/**
 * Handles the AJAX toggle request for email notifications
 * @param {Object} el The element that triggered the request.
 * @param {Object} url The URL to call.
 */
function ajaxToggleWatch(el, url) {
  $.ajax({
  	url: url,
	type: 'GET',
	data: { async : 'true' },
	dataType: 'json',
	timeout: 4000, //set very high to prevent time outs for users with high latency connections
	success: function (json) {
	  toggleWatch(el, json);
	},
    error: function(json){
      $(el).fadeTo(400, 1);
    }
  });
}

/***
 * Toggle the watching of a node
 */
function toggleWatch(el, rdata) {
  var string_text_enabled = Drupal.settings.watcher.watch_toggle_enabled;
  var string_title_enabled = Drupal.settings.watcher.watch_toggle_enabled_title;
  var string_text_disabled = Drupal.settings.watcher.watch_toggle_disabled;
  var string_title_disabled = Drupal.settings.watcher.watch_toggle_disabled_title;
  var string_watched_posts_link = Drupal.settings.watcher.watch_watched_posts_link;

  if (rdata['status'] == 'enabled') {
	$(el).empty().append(string_text_enabled).attr('title', string_title_enabled).addClass('watcher_node_toggle_watching_link_watched');
    $(el).parent().addClass('watcher_node_watched');
  } 
  else if (rdata['status'] == 'disabled') {
	$(el).empty().append(string_text_disabled).attr('title', string_title_disabled).removeClass('watcher_node_toggle_watching_link_watched');
    $(el).parent().removeClass('watcher_node_watched');
  }
  else {
	  $(el).parent().fadeOut('fast').replaceWith(rdata['data']).fadeIn('fast');
  }
  
  //Append link to user's list of watched posts if it doesn't already exist
  var parent = $(el).parent();
  if ($(parent).find('a.watcher_node_help_link_to_binder').length == 0) {
    $(parent).addClass('watcher_node_toggle_watching_link_with_link_to_binder').append(string_watched_posts_link);    
  }
 
  
  // Fade in, display the element again
  $(el).fadeTo(400, 1);
}