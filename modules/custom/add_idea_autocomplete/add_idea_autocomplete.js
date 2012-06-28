(function($) {
  Drupal.behaviors.ideaAddFormAutocomplete = {
    attach: function(context, settings) {
      for (var i in settings.options) {
        settings.options[i].label = settings.options[i].node_title;
        settings.options[i].value = settings.options[i].node_title;
      }
      $("form#idea-node-form input#edit-title").autocomplete({
        source: settings.options,
        select: function(event, ui) {
          window.location.replace(window.location.origin + "/node/" + ui.item.nid);
//          window.location.replace(window.location.origin + "/openideal/node/" + ui.item.nid);
        }
      });    
    }
  };
})(jQuery);


