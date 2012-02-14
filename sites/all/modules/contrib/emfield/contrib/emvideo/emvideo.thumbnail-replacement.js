// $Id: emvideo.thumbnail-replacement.js,v 1.1.2.2 2009/09/01 20:35:24 aaron Exp $

/**
 *  @file
 *  This will use jQuery AJAX to replace a thumbnail with its video version.
 */
Drupal.behaviors.emvideoThumbnailReplacement = function(context) {
  // On load, add a span for the play button to any required thumbnails.
  $('a.emvideo-thumbnail-replacement:not(.emvideo-thumbnail-replacement-processed)', context).addClass('emvideo-thumbnail-replacement-processed').each(function() {
    if (Drupal.settings.emvideo.thumbnail_overlay) {
      // Add the play button.
      $(this).prepend("<span></span>");
    }
  });

  // Inline videos will click through to the video.
  $('a.emvideo-modal-emvideo:not(.emvideo-modal-emvideo-processed)', context).addClass('emvideo-modal-emvideo-processed').each(function() {
    // When clicking the image, load the video with AJAX.
    // Note that this only happens if this is not a lightbox.
    $(this).click(function() {
      // 'this' won't point to the element when it's inside the ajax closures,
      // so we reference it using a variable.
      var element = this;
      $.ajax({
        url: element.href,
        dataType: 'html',
        success: function (data) {
          if (data) {
            // Success.
            $(element).parent().html(data);
          }
          else {
            // Failure.
            alert('An unknown error occurred.\n'+ element.href);
          }
        },
        error: function (xmlhttp) {
          alert('An HTTP error '+ xmlhttp.status +' occurred.\n'+ element.href);
        }
      });
      return false;
    });
  });
};
