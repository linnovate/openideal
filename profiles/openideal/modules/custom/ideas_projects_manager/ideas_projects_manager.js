(function($) {
  Drupal.behaviors.ideasProjectsManager = {
    attach: function(context, settings) {
      $('#add').click(function() {  
        return !$('#edit-ideas option:selected').remove().appendTo('#edit-ideas-projects-manager');  
      });  
      $('#remove').click(function() {  
        return !$('#edit-ideas-projects-manager option:selected').remove().appendTo('#edit-ideas'); 
      });
    }
  };
})(jQuery);


