(function ($) {

  Drupal.behaviors.google_plusone_preview = {
    attach: function() {

      var $preview = $('#google_plusone_preview').show();

      // Selectors with complex matches to be able to re-use it
      // in admin page and block settings page.
      var $sizeForm = $('div[id$="-size"]');
      var $widthInput = $('input[id$="-width"]');

      var size = $sizeForm.find(':checked').val();
      $preview.find('#google_plusone_' + size).addClass('active_size').show();

      // Bind changes in the size select form to update preview.
      $sizeForm.bind('change', function(){
         var size = $(this).find(':checked').val();
         $preview.find('.active_size').hide();
         $preview.find('#google_plusone_' + size).addClass('active_size').show();
      });

      $widthInput.bind('keyup', function(){
        var newWidth = $(this).val();
        var $container = $preview.find('.g-inline').empty();
        var sizes = ['small','medium','standard','tall'];
        $container.each(function(i){
          gapi.plusone.render(this, {'size':sizes[i],'annotation':'inline','href':'http://drupal.org','width': parseInt(newWidth)});
        });
      });
    }
  };

})(jQuery);