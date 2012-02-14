(function ($) {
  $(document).ready(function(){
    $('a.service-links-linkedin-share-button').each(function(){
      var script_obj = document.createElement('script');
      script_obj.type = 'IN/Share';
      script_obj.setAttribute("data-url", $(this).attr('href'));
      if (Drupal.settings.ws_lsb.countmode != '') {
        script_obj.setAttribute("data-counter", Drupal.settings.ws_lsb.countmode);
      }

      $(this).replaceWith(script_obj);
    });
  });
})(jQuery);
