/*
 * ##### Sasson - advanced drupal theming. #####
 *
 * Open Ideal scripts.
 *
 */

(function($) {
   
Drupal.behaviors.categoryList = {
  attach: function(context) {
//    alert('hi');
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

// DUPLICATE AND UNCOMMENT
//Drupal.behaviors.behaviorName = {
//  attach: function(context) {
//    // Do some magic...
//  }
//};

})(jQuery);
