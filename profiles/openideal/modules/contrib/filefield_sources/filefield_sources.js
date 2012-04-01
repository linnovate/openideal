(function ($) {

/**
 * Behavior to add source options to configured fields.
 */
Drupal.behaviors.fileFieldSources = {};
Drupal.behaviors.fileFieldSources.attach = function(context, settings) {
  $('div.filefield-sources-list a', context).click(function() {
    $fileFieldElement = $(this).parents('div.form-managed-file:first');

    // Remove the active class.
    $(this).parents('div.filefield-sources-list').find('a.active').removeClass('active');

    // Find the unique FileField Source class name.
    var fileFieldSourceClass = this.className.match(/filefield-source-[0-9a-z]+/i)[0];

    // The default upload element is a special case.
    if ($(this).is('.filefield-source-upload')) {
      $fileFieldElement.find('div.filefield-sources-list').siblings('input.form-file, input.form-submit').css('display', '');
      $fileFieldElement.find('div.filefield-source').css('display', 'none');
    }
    else {
      $fileFieldElement.find('div.filefield-sources-list').siblings('input.form-file, input.form-submit').css('display', 'none');
      $fileFieldElement.find('div.filefield-source').not('div.' + fileFieldSourceClass).css('display', 'none');
      $fileFieldElement.find('div.' + fileFieldSourceClass).css('display', '');
    }

    // Add the active class.
    $(this).addClass('active');
    Drupal.fileFieldSources.updateHintText($fileFieldElement.get(0));
  });

  // Hide all the other upload mechanisms on page load.
  $('div.filefield-source', context).css('display', 'none');
  $('div.filefield-sources-list', context).each(function() {
    $(this).find('a:first').addClass('active');
  });
  $('form#node-form', context).submit(function() {
    Drupal.fileFieldSources.removeHintText();
  });
};

/**
 * Helper functions used by FileField Sources.
 */
Drupal.fileFieldSources = {
  /**
   * Update the hint text when clicking between source types.
   */
  updateHintText: function(fileFieldElement) {
    // Add default value hint text to text fields.
    $(fileFieldElement).find('div.filefield-source').each(function() {
      var matches = this.className.match(/filefield-source-([a-z]+)/);
      var sourceType = matches[1];
      var defaultText = '';
      var textfield = $(this).find('input.form-text:first').get(0);
      var defaultText = (Drupal.settings.fileFieldSources && Drupal.settings.fileFieldSources[sourceType]) ? Drupal.settings.fileFieldSources[sourceType].hintText : '';

      // If the field doesn't exist, just return.
      if (!textfield) {
        return;
      }

      // If this field is not shown, remove its value and be done.
      if (!$(this).is(':visible') && textfield.value == defaultText) {
        textfield.value = '';
        return;
      }

      // Set a default value:
      if (textfield.value == '') {
        textfield.value = defaultText;
      }

      // Set a default class.
      if (textfield.value == defaultText) {
        $(textfield).addClass('hint');
      }

      $(textfield).focus(hideHintText);
      $(textfield).blur(showHintText);

      function showHintText() {
        if (this.value == '') {
          this.value = defaultText;
          $(this).addClass('hint');
        }
      }

      function hideHintText() {
        if (this.value == defaultText) {
          this.value = '';
          $(this).removeClass('hint');
        }
      }
    });
  },

  /**
   * Delete all hint text from a form before submit.
   */
  removeHintText: function() {
    $('div.filefield-element input.hint').val('').removeClass('hint');
  }
};

})(jQuery);