$(document).ready(function() {

  var $preview = $('#google_plusone_preview').show();

  // In the first load.
  var $sizeForm = $('#edit-google-plusone-button-settings-size-small-wrapper').parent();
  var size = $sizeForm.find(':checked').val();
  $preview.find('#google_plusone_' + size).addClass('active_size').show();

  // Bind changes in the size select form to update preview.
  $sizeForm.bind('change', function(){
     var size = $(this).find(':checked').val();
     $preview.find('.active_size').hide();
     $preview.find('#google_plusone_' + size).addClass('active_size').show();
  });

});