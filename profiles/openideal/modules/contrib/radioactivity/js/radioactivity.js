
(function ($) {
Drupal.behaviors.radioactivity = {
  attach: function (context, settings) {
    // Do an ajax callback to the given callback addres {
    for (var url in settings.radioactivity.emitters) {
      
      var data = settings.radioactivity.emitters[url];

      // cookie based flood protection 
      if (settings.radioactivity.flood_protection.enabled) {
        if ($.cookie('radioactivity_' + data['checksum'])) {
          continue;
        } else {
          var exp = new Date();
          exp.setTime(exp.getTime() + (settings.radioactivity.flood_protection.timeout * 60 * 1000));
          $.cookie('radioactivity_' + data['checksum'], true, { expires: exp });
        }
      } else {
        // clear the possible cookie
        $.cookie('radioactivity_' + data['checksum'], null);
      }
     

      $.ajax({
        url: url,
        data: data,
        type: 'POST',
        cache: false,
        dataType: "html"
      });
      }
    }
  };
})(jQuery);

