/**
 * ideaL comments javascript
 *
 */
 
(function($) {
  Drupal.behaviors.idealComments = {
    attach: function(context, settings) {
      // Show only first X (settings.comments_fold) comments
      if ($("#comments").children(".comment").size() > settings.comments_fold) {
        $("#comments").once().append(
          '<div class="show-all-comments">' + Drupal.t('Show all comments') + '</div><div class="show-all-comments less hidden">' + Drupal.t('Show less comments') + '</div>');
        $("#comments").children(".comment").slice(settings.comments_fold).addClass("js-hidden");
      }
      $(".show-all-comments").once().click(function() {
        $(".comment.js-hidden").slideToggle("slow");
        $(".show-all-comments").toggleClass("hidden");
      });
      
      // Add a link to show nested comments
      $("#comments .indented").once(function() {
        var child_count = $(this).children(".comment").size();
        $(this).prev().find(".content").after('<div class="comment_show right">' + child_count + ' ' + Drupal.t('Comments to this comment') + '</div>');
      });
      
        // Show nested comments
      $("#comments .comment_show").once().click(function() {
        $(this).closest('.comment').next(".indented").slideToggle("slow");
        $(this).toggleClass("showing");
      });

        // Show form comment to comment
      $(".comment li.comment-reply a").once().click(function(e) {
        $(this).closest('.comment').find('.comment-form').slideToggle("slow");
        e.preventDefault();
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
      $("a", tips).click(function(e) {
        $(this).parents(".filter-help").siblings().show();
        $(this).unbind('click');
        e.preventDefault();
      });            
    }
  };
})(jQuery);
