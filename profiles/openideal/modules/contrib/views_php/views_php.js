(function ($) {

/**
 * Attach views php clickable variables behavior.
 */
Drupal.behaviors.viewsPHPVariables = {
  attach: function (context) {
    $('.views-php-variables', context).delegate('a', 'click', function() {
      var textarea = $(this.href.replace(/^.*#/, '#'))[0];
      var text = $(this).text();
      textarea.focus();
      if (!isNaN(textarea.selectionStart)) {
        textarea.value = textarea.value.substring(0, textarea.selectionStart) + text + textarea.value.substring(textarea.selectionEnd);
        textarea.selectionStart = textarea.selectionStart + text.length;
        textarea.selectionEnd = textarea.selectionEnd + text.length;
      }
      // IE support.
      else if (document.selection) {
        document.selection.createRange().text = text;
      }
      else {
        textarea.value += text;
      }
      textarea.focus();

      return false;
    });
  }
};

})(jQuery);
