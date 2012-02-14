// $Id: flag-admin.js,v 1.1.2.2 2010/05/08 20:46:06 quicksketch Exp $

/**
 * Behavior to disable the "unflag" option if "flag" is not available.
 */
Drupal.behaviors.flagRoles = function(context) {
  $('#flag-roles input.flag-access', context).change(function() {
    var unflagCheckbox = $(this).parents('tr:first').find('input.unflag-access').get(0);
    if (this.checked) {
      // If "flag" is available, restore the state of the "unflag" checkbox.
      unflagCheckbox.disabled = false;
      if (typeof(unflagCheckbox.previousFlagState) != 'undefined') {
        unflagCheckbox.checked = unflagCheckbox.previousFlagState;
      }
      else {
        unflagCheckbox.checked = true;
      }
    }
    else {
      // Remember if the "unflag" option was checked or unchecked, then disable.
      unflagCheckbox.previousFlagState = unflagCheckbox.checked;
      unflagCheckbox.disabled = true;
      unflagCheckbox.checked = false;
    }
  });

  $('#flag-roles input.unflag-access', context).change(function() {
    if ($(this).parents('tr:first').find('input.unflag-access:checked:not(:disabled)').size() > 0) {
      $('#edit-unflag-denied-text-wrapper').slideUp();
    }
    else {
      $('#edit-unflag-denied-text-wrapper').slideDown();
    }
  });

  // Hide the link options by default if needed.
  if ($('#flag-roles input.unflag-access:checked:not(:disabled)').size() > 0) {
    $('#edit-unflag-denied-text-wrapper').css('display', 'none');
  }
};


/**
 * Behavior to make link options dependent on the link radio button.
 */
Drupal.behaviors.flagLinkOptions = function(context) {
  $('.flag-link-options input.form-radio', context).change(function() {
    var radioButton = this;
    $('#link-options').slideUp(function() {
      $('#link-options input').each(function() {
        $(this).parents('.form-item:first').css('display', 'none');
      });
      var linkOptionFields = $(radioButton).attr('rel');
      if (linkOptionFields) {
        linkOptionFields = linkOptionFields.split(' ');
        for (var n in linkOptionFields) {
          $('#link-options input[name=' + linkOptionFields[n] + ']').parents('.form-item:first').css('display', 'block');
        }
        $('#link-options').slideDown();
      }
    });
  });
  // Hide the link options by default if needed.
  if (!$('.flag-link-options input.form-radio:checked').attr('rel')) {
    $('#link-options').css('display', 'none');
  }
};

/**
 * Vertical tabs integration.
 */
Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.flag = function() {
  var flags = [];
  $('fieldset.vertical-tabs-flag input.form-checkbox').each(function() {
    if (this.checked) {
      flags.push(this.name.replace(/flag\[([a-z0-9]+)\]/, '$1'));
    }
  });

  if (flags.length) {
    return flags.join(', ');
  }
  else {
    return Drupal.t('No flags');
  }
}
