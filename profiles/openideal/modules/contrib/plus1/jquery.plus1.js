(function ($) {
  // Documentation on Drupal JavaScript behaviors can be found here: http://drupal.org/node/114774#javascript-behaviors
  Drupal.behaviors.plus1 = {
    attach: function(context){
      $('.plus1-widget', context).once('plus1', function(){
        var plus1_widget = $(this);
        plus1_widget.find('.plus1-link').attr('href', function(){ return $(this).attr('href') + '&json=true'; }).click(function(){
          $.getJSON($(this).attr('href'), function(json){
            if (json) {
              var newWidget = $(json.widget);
              newWidget.hide();
              plus1_widget.replaceWith(newWidget);
              newWidget.fadeIn('slow');
              Drupal.attachBehaviors();
            }
          });
          return false;
        });
      });
    }
  };
})(jQuery)
