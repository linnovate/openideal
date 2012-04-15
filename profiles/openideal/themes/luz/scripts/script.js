/*
 * ##### Sasson - advanced drupal theming. #####
 *
 * Open Ideal scripts.
 *
 */

(function($) {
   
Drupal.behaviors.categoryList = {
  attach: function(context) {
    $('.category-menu li').hover(
      function() {
        var level = $(this).parent().attr('class');
        var start = level.indexOf('level-');
        level = parseInt(level.substr(start+6, 1)) + 1;
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
    //var $watcherText = $('.watcherText_node a').html() ;
    $('.watcher_node a').html('Watcher') ;
    alert($watcherText);

    
  }
};

// DUPLICATE AND UNCOMMENT
//Drupal.behaviors.behaviorName = {
//  attach: function(context) {
//    // Do some magic...
//  }
//};

})(jQuery);
