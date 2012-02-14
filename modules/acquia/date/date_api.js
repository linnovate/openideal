/**
 * Show/hide custom format sections on the date-time settings page.
 */
Drupal.behaviors.dateDateTime = function(context) {
  // Show/hide custom format depending on the select's value.
  $('select.date-format:not(.date-time-processed)', context).change(function() {
    $(this).addClass('date-time-processed').parents("div.date-container").children("div.custom-container")[$(this).val() == "custom" ? "show" : "hide"]();
  });

  // Attach keyup handler to custom format inputs.
  $('input.custom-format:not(.date-time-processed)', context).addClass('date-time-processed').keyup(function() {
    var input = $(this);
    var url = Drupal.settings.dateDateTime.lookup +(Drupal.settings.dateDateTime.lookup.match(/\?q=/) ? "&format=" : "?format=") + Drupal.encodeURIComponent(input.val());
    $.getJSON(url, function(data) {
      $("div.description span", input.parent()).html(data);
    });
  });

  // Trigger the event handler to show the form input if necessary.
  $('select.date-format', context).trigger('change');
};
