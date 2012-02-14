// $Id$

Drupal.behaviors.tao = function (context) {
  $('fieldset.collapsible:not(.tao-processed) > legend > .fieldset-title').each(function() {
    var fieldset = $(this).parents('fieldset').eq(0);
    fieldset.addClass('tao-processed');

    // Expand any fieldsets containing errors.
    if ($('input.error, textarea.error, select.error', fieldset).size() > 0) {
      $(fieldset).removeClass('collapsed');
    }

    // Add a click handler for toggling fieldset state.
    $(this).click(function() {
      if (fieldset.is('.collapsed')) {
        $(fieldset).removeClass('collapsed').children('.fieldset-content').show();
      }
      else {
        $(fieldset).addClass('collapsed').children('.fieldset-content').hide();
      }
      return false;
    });
  });
};
