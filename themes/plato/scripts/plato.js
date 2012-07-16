/*
 * ##### Sasson - advanced drupal theming. #####
 *
 * SITENAME scripts.
 *
 */

(function($) {

  Drupal.behaviors.categoryList = {
    attach: function(context) {
      $('.category-menu li').hover(
        function() {
          var level = $(this).parent().attr('class');
          var start = level.indexOf('level-');
          level = parseInt(level.substr(start+6, 1), 10) + 1;
          $(this).children('ul.level-' + level).show('fast');
        },
        function() {
          $(this).children('ul').hide('fast');
        }
      );
    }
  };

  Drupal.behaviors.watcher = {
    attach: function(context) {
      $('.watcher_node:not(.watcher_node_watched) a').html(Drupal.t('Follow'));
      $('.watcher_node.watcher_node_watched a').html(Drupal.t('Unfollow'));
    }
  };

  Drupal.behaviors.offCanvas = {
    attach: function (context, settings) {

      var headWidth = $('head').width();

      $(window).resize(function () {
        if ($('head').width() != headWidth) {
          headWidth = $('head').width();
          triggerOffCanvas(headWidth);
          log(headWidth);
        }
      });

      var triggerOffCanvas = function(headWidth) {

        if (headWidth > 2) {
          $('ul#secondary-menu-links').once().prepend('<li class="off-canvas menu-item hidden"><a class="off-canvas" href="#">≡</a></li>');
          $('body').addClass('off-canvas');
          $('li.off-canvas').removeClass('hidden');
        } else {
          $('body').removeClass('off-canvas');
          $('li.off-canvas').addClass('hidden');
        }
      };

      triggerOffCanvas(headWidth);

      $('li.off-canvas a').live('click', function(e) {
        e.preventDefault();
        $('body').toggleClass('on-canvas');
      });
    }
  };

  // DUPLICATE AND UNCOMMENT
  // Drupal.behaviors.behaviorName = {
  //   attach: function (context, settings) {
  //     // Do some magic...
  //   }
  // };

})(jQuery);
