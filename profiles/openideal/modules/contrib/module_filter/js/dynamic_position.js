(function($) {
  Drupal.behaviors.moduleFilterDynamicPosition = {
    attach: function() {
      $(window).scroll(function() {
        // Vertical movement.
        var top = $('#module-filter-tabs').offset().top;
        var bottom = top + $('#module-filter-tabs').height();
        var windowHeight = $(window).height();
        if (((bottom - windowHeight) > ($(window).scrollTop() - $('#module-filter-submit').height())) && $(window).scrollTop() + windowHeight - $('#module-filter-submit').height() - $('#all-tab').height() > top) {
          $('#module-filter-submit').removeClass('fixed-top').addClass('fixed fixed-bottom');
        }
        else if (bottom < $(window).scrollTop()) {
          $('#module-filter-submit').removeClass('fixed-bottom').addClass('fixed fixed-top');
        }
        else {
          $('#module-filter-submit').removeClass('fixed fixed-bottom fixed-top');
        }

        // Horizontal movement.
        if ($('#module-filter-submit').hasClass('fixed-bottom') || $('#module-filter-submit').hasClass('fixed-top')) {
          var left = $('#module-filter-tabs').offset().left - $(window).scrollLeft();
          if (left != $('#module-filter-submit').offset().left - $(window).scrollLeft()) {
            $('#module-filter-submit').css('left', left);
          }
        }
      });
      $(window).trigger('scroll');
      $(window).resize(function() {
        $(window).trigger('scroll');
      });
    }
  }
})(jQuery);
