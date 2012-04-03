/**
 *
 */
(function($){
  Drupal.ajax.prototype.commands.isusefulUpdate = function (ajax, response, status) {
    response.selector = $('.is-useful', ajax.element.form);
    ajax.commands.insert(ajax, response, status);
  };
})(jQuery);

(function($){

  Drupal.behaviors.isuseful = {
    attach: function (context) {
      $('div.form-item-is-useful').once('is_useful', function() {
        var $this = $(this);
        var $container = $('<div class="is-useful-widget clearfix"></div>');
        var $select = $('select', $this);

        // Setup the Yes/No/Maybe buttons
        var $options = $('option', $this).not('[value="-"], [value="0"]');
        var index = -1;
        $options.each(function(i, element) {
          var classes = 'is-useful-' + element.value;
          var text = element.text.replace(/\([0-9]*\) /ig, '');
          var count = element.text.replace(text, '');
          if (element.value == $select.val()) {
            classes += ' selected';
          }
          $('<div class="is-useful-link"><span class="count">'+ count +'</span><a href="#' + element.value + '" title="' + text + '">' + text + '</a></div>')
            .addClass(classes)
            .appendTo($container);
        });
        $container.find('a')
          .bind('click', $this, Drupal.behaviors.isuseful.vote);
        // Attach the new widget and hide the existing widget.
        $select.after($container).css('display', 'none');
        $('.is-useful-form .form-submit').css('display', 'none');
      });
    },
    vote: function(event) {
      var $this = $(this);
      var $widget = event.data;
      var value = this.hash.replace('#', '');
      $('select', $widget).val(value).change();
      event.preventDefault();
    },
  };
})(jQuery);