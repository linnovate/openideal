$(document).ready(function(){
    // Show only first 10 comments
  if ($("#comments").children(".comment").size() > 10) {
    $("#comments").append(
      '<div class="show-all-comments">' + Drupal.t('Show all comments') + '</div><div class="show-all-comments less hidden">' + Drupal.t('Show less comments') + '</div>');
    $("#comments").children(".comment").slice(10).addClass("js-hidden");
  }
  $(".show-all-comments").click(function() {
    $(".comment.js-hidden").slideToggle("slow");
    $(".show-all-comments").toggleClass("hidden");
  });


    // Add a link to show nested comments
  $("#comments .indented").each(function() {
    var child_count = $(this).children(".comment").size();
    $(this).prev().find(".content-inner p:last").after('<div class="comment_show right">' + child_count + ' ' + Drupal.t('Comments to this comment') + '</div>');
  });
    // Show nested comments
  $("#comments .comment_show").click(function() {
    $(this).parents().nextAll(".indented:first").slideToggle("slow");
    $(this).toggleClass("showing");
  });

    // Show form comment to comment
  $(".comment li.comment_reply a").click(function() {
    $(this).parents().nextAll("form:first").slideToggle("slow");
    return false; 
  });

  //if url has hash - aka - # sign make sure it's visible
  if(window.location.hash) {
          $(window.location.hash).parents().show();
  } else {
    // Fragment doesn't exist
  }
});
