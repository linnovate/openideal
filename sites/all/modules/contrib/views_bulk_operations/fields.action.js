// $Id: fields.action.js,v 1.1.2.5 2009/12/08 00:50:39 kratib Exp $
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};
Drupal.vbo.fieldsAction = Drupal.vbo.fieldsAction || {};

Drupal.vbo.fieldsAction.updateToggler = function(toggler, direct) {
  var parent = $(toggler).parents('tr')[0];
  if ((toggler.checked && direct) || (!toggler.checked && !direct)) {
    $('.fields-action-togglable :input', parent).removeAttr('disabled');
  }
  else {
    $('.fields-action-togglable :input', parent).attr('disabled', true);
  }
}

Drupal.behaviors.vbo_fieldsAction = function(context) {
  $('.fields-action-toggler', context).click(function() {
    Drupal.vbo.fieldsAction.updateToggler(this, true);
  });

  $('th.select-all', context).click(function() {
    $('.fields-action-toggler', context).each(function() {
      Drupal.vbo.fieldsAction.updateToggler(this, false);
    });
  });
  
  // Disable all those whose checkboxes are off.
  $('.fields-action-togglable', context).each(function() {
    if ($('.fields-action-toggler', $(this).parents('tr:first')).attr('checked')) {
      $(this).parents('tr:first').addClass('selected');
    }
    else {
      $(':input', this).attr('disabled', true);
    }
  });
}

// END jQuery
})(jQuery);

