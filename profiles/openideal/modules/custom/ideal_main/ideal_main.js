(function($) {
  Drupal.behaviors.ideasProjectsManager = {
    attach: function(context, settings) {
      $('#add').click(function() {  
        return !$('#edit-ideas option:selected').remove().appendTo('#edit-ideas-in-project');  
      });  
      $('#remove').click(function() {  
        return !$('#edit-ideas-in-project option:selected').remove().appendTo('#edit-ideas'); 
      });
    }
  };
})(jQuery);


