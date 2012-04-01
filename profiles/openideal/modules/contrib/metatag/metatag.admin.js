(function ($) {

Drupal.behaviors.metatagUIConfigListing = {
  attach: function (context) {
    // Hide elements to be visible if JavaScript is enabled.
    $('.js-show').show();

    // Show or hide the summary
    $('table.metatag-config-overview a.toggle-details', context).click(function() {
      $(this).parent('div').siblings('div.metatag-config-details').each(function() {
        if ($(this).hasClass('js-hide')) {
          $(this).slideDown('slow').removeClass('js-hide');
        }
        else {
          $(this).slideUp('slow').addClass('js-hide');
        }
      });

      // Change the expanded or collapsed state of the instance label.
      if ($(this).parent('div').hasClass('collapsed')) {
        $(this).parent('div').removeClass('collapsed').addClass('expanded');
      }
      else {
        $(this).parent('div').removeClass('expanded').addClass('collapsed');
      }
    });
  }
}

})(jQuery);
