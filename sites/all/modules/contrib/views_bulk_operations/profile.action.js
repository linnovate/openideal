// $Id: profile.action.js,v 1.1.2.1 2009/10/08 21:37:55 kratib Exp $
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};
Drupal.vbo.profileAction = Drupal.vbo.profileAction || {};

Drupal.vbo.profileAction.updateField = function(checkbox, direct) {
  var id = checkbox.id.replace('-check', '');
  if ((checkbox.checked && direct) || (!checkbox.checked && !direct)) {
    $('#'+id).removeAttr('disabled');
  }
  else {
    $('#'+id).attr('disabled', true);
  }
}

Drupal.behaviors.vbo_profileAction = function(context) {
  $('.profile-action-toggler', context).each(function() {
    Drupal.vbo.profileAction.updateField(this, true);
  });

  $('th.select-all', context).click(function(e) {
    $('.profile-action-toggler').each(function() {
      Drupal.vbo.profileAction.updateField(this, false);
    });
  });
}

// END jQuery
})(jQuery);

