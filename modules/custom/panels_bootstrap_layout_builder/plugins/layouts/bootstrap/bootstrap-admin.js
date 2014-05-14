(function ($) {

  Drupal.bootstrap = Drupal.bootstrap || {};

  Drupal.bootstrap.classSelectors = [];

  Drupal.behaviors.bootstrapAdmin = {
    attach: function(context) {
      // Show/hide layout manager button
      $('input#panels-bootstrap-toggle-layout:not(.panels-bootstrap-processed)', context)
        .addClass('panels-bootstrap-processed')
        .click(function() {
          $('.panel-bootstrap-admin')
            .toggleClass('panel-bootstrap-no-edit-layout')
            .toggleClass('panel-bootstrap-edit-layout');

          if ($('.panel-bootstrap-admin').hasClass('panel-bootstrap-edit-layout')) {
            $(this).val(Drupal.t('Hide layout designer'));
          }
          else {
            $(this).val(Drupal.t('Show layout designer'));
          }
          return false;
        });

      // Window splitter behavior.
      $('div.bootstrap-class-selector:not(.bootstrap-class-selector-processed)')
        .addClass('bootstrap-class-selector-processed')
        .each(function() {
          Drupal.bootstrap.classSelectors.push(new Drupal.bootstrap.classSelector($(this).find('select')));
        });
    }
  };


  Drupal.bootstrap.classSelector = function($classSelector) {
    var $this = this;
    $classSelector.each(function(){
      var $selectorFor = $("#"+$(this).attr('data-for'));
      var screenSize = $(this).attr('data-screen-size');
      $(this).bind('change',function(){
        Drupal.bootstrap.removeClasses($selectorFor,screenSize);
        $selectorFor.addClass($(this).val());
        Drupal.ajax['bootstrap-class-selector-ajax'].options.data = {
          'item': $(this).attr('data-for-id'),
          'bootstrap_class': $(this).val(),
          'bootstrap_screen': $(this).attr('data-screen-size')
        };
        jQuery('.panel-bootstrap-edit-layout').trigger('updateBootstrapClass');
      });
    })
  }

  Drupal.bootstrap.removeClasses = function($row,screenSize){
    console.log($row)
     $row.removeClass(function() { /* Matches even table-col-row */
       var classesToRemove = '',
       classes = this.className.split(' '),
       regex = new RegExp('col-'+screenSize);
       for(var i = 0; i < classes.length; i++ ) {
           if( regex.test( classes[i] ) ) { /* Filters */
               classesToRemove += classes[i] + ' ';
           }
       }
       console.log(classesToRemove)
       return classesToRemove ; /* Returns all classes to be removed */
    });
  }

  $(function() {

    // Create a generic ajax callback for use with the splitters which
    // background AJAX to store their data.
    var element_settings = {
      url: Drupal.settings.bootstrap.resize,
      event: 'updateBootstrapClass',
      keypress: false,
      // No throbber at all.
      progress: { 'type': 'throbber' }
    };

    Drupal.ajax['bootstrap-class-selector-ajax'] = new Drupal.ajax('bootstrap-class-selector-ajax', $('.panel-bootstrap-admin').get(0), element_settings);

    // Prevent ajax goo from doing odd things to our layout.
    Drupal.ajax['bootstrap-class-selector-ajax'].beforeSend = function() { };
    Drupal.ajax['bootstrap-class-selector-ajax'].beforeSerialize = function() { };

  });

})(jQuery);
