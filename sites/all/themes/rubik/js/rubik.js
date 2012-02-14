/**
 * Implementation of Drupal behavior.
 */
Drupal.behaviors.rubik = function(context) {
  // If there are both main column and side column buttons, only show the main
  // column buttons if the user scrolls past the ones to the side.
  $('div.form:has(div.column-main div.buttons):not(.rubik-processed)').each(function() {
    var form = $(this);
    var offset = $('div.column-side div.buttons', form).height() + $('div.column-side div.buttons', form).offset().top;
    $(window).scroll(function () {
      if ($(this).scrollTop() > offset) {
        $('div.column-main div.buttons', form).show();
      }
      else {
        $('div.column-main div.buttons', form).hide();
      }
    });
    form.addClass('rubik-processed');
  });

  $('a.toggler:not(.rubik-processed)', context).each(function() {
    var id = $(this).attr('href').split('#')[1];
    // Target exists, add click handler.
    if ($('#' + id).size() > 0) {
      $(this).click(function() {
        toggleable = $('#' + id);
        toggleable.toggle();
        $(this).toggleClass('toggler-active');
        return false;
      });
    }
    // Target does not exist, remove click handler.
    else {
      $(this).addClass('toggler-disabled');
      $(this).click(function() { return false; });
    }
    // Mark as processed.
    $(this).addClass('rubik-processed');
  });
};
