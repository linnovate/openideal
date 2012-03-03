/**
 * ideaL comments javascript
 *
 */
 
(function($) {
  Drupal.behaviors.idealComments = {
    attach: function(context, settings) {
      // Show only first X (settings.comments_fold) comments
      if ($("#comments").children(".comment").size() > settings.comments_fold) {
        $("#comments").append(
          '<div class="show-all-comments">' + Drupal.t('Show all comments') + '</div><div class="show-all-comments less hidden">' + Drupal.t('Show less comments') + '</div>');
        $("#comments").children(".comment").slice(settings.comments_fold).addClass("js-hidden");
      }
      $(".show-all-comments").click(function() {
        $(".comment.js-hidden").slideToggle("slow");
        $(".show-all-comments").toggleClass("hidden");
      });
      
      // Add a link to show nested comments
      $("#comments .indented").each(function() {
        var child_count = $(this).children(".comment").size();
        $(this).prev().find(".content").after('<div class="comment_show right">' + child_count + ' ' + Drupal.t('Comments to this comment') + '</div>');
      });
      
        // Show nested comments
      $("#comments .comment_show").click(function() {
        $(this).parents().nextAll(".indented:first").slideToggle("slow");
        $(this).toggleClass("showing");
      });

        // Show form comment to comment
      $(".comment li.comment-reply a").click(function() {
        $(this).parents('.links').siblings('.content').find('.comment-form').slideToggle("slow");
        return false; 
      });

      // If url has hash '#' sign make sure it's visible
      if(window.location.hash) {
        $(window.location.hash).parents().show();
      } 
    }
  };
  Drupal.behaviors.hideTips = {
    attach: function(context) {      
      /* 
       *  Hide and show formatting tips.
       */
      var tips = $("#comments .filter-wrapper .fieldset-wrapper > *");
      // Hide formating tips by default
      $(tips).not(".filter-help, .form-type-select").hide();
      // Show Tips On Click and unbind for the next click
      $("a", tips).click(function() {
        $(this).parents(".filter-help").siblings().show();
        $(this).unbind('click');
        return false;
      });            
    }
  };
})(jQuery);
