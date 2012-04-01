(function ($) {
  $(document).ready(function(){
    $('a.service-links-google-plus-one').each(function(){
      var counter = Drupal.settings.ws_gpo.counter ? '' : 'count="false" ';
      var type = (Drupal.settings.ws_gpo.type == '') ? '' : 'size="'+ Drupal.settings.ws_gpo.type + '" ';
      var g_text = '<g:plusone ' + counter + type + 'href="' + $(this).attr('href') + '" >' + '</g:plusone>';
      $(this).replaceWith(g_text);
    });
  });
})(jQuery);
