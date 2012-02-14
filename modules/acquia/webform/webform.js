// $Id: webform.js,v 1.3.2.1 2008/11/19 22:24:39 quicksketch Exp $

/**
 * Webform node form interface enhancments.
 */

Drupal.behaviors.webform = function(context) {
  // Apply special behaviors to fields with default values.
  Drupal.webform.defaultValues(context);
  // On click or change, make a parent radio button selected.
  Drupal.webform.setActive(context);
}

Drupal.webform = new Object();

Drupal.webform.defaultValues = function(context) {
  var $fields = $('.webform-default-value', context);
  var $forms = $('.webform-default-value', context).parents('form:first');
  $fields.each(function() {
    this.defaultValue = this.value;
    $(this).focus(function() {
      if (this.value == this.defaultValue) {
        this.value = '';
        $(this).removeClass('webform-default-value');
      }
    });
    $(this).blur(function() {
      if (this.value == '') {
        $(this).addClass('webform-default-value');
        this.value = this.defaultValue;
      }
    });
  });

  // Clear all the form elements before submission.
  $forms.submit(function() {
    $fields.focus();
  });
};

Drupal.webform.setActive = function(context) {
  var setActive = function() { $('.form-radio', $(this).parent().parent()).attr('checked', true); };
  $('.webform-set-active', context).click(setActive).change(setActive);
};
