(function ($) {
  Drupal.behaviors.userpointsAdminFieldsetSummaries = {
    attach: function (context) {
      // Moderation status.
      $('fieldset#edit-status', context).drupalSetSummary(function (context) {
        if ($('#edit-userpoints-points-moderation-0').is(':checked')) {
          return Drupal.t('Approved by default.');
        }
        else {
          return Drupal.t('Moderated by default.');
        }
      });

      // Message.
      $('fieldset#edit-misc', context).drupalSetSummary(function (context) {
        if ($('#edit-userpoints-display-message-0').is(':checked')) {
          return Drupal.t('No message is displayed by default.');
        }
        else {
          return Drupal.t('Message is displayed by default.');
        }
      });

      // Listings.
      $('fieldset#edit-reports', context).drupalSetSummary(function (context) {
        var limit = $('select#edit-userpoints-report-limit :selected').val();
        var usercount = $('select#edit-userpoints-report-usercount :selected').val();

        return Drupal.t('%limit transactions, %usercount users per page.', {'%limit' : limit, '%usercount' : usercount});
      });

      // Listings.
      $('fieldset#edit-renaming', context).drupalSetSummary(function (context) {
        var branding = {
        '@ucpoints' : $('#edit-userpoints-trans-ucpoints').val(),
        '@lcpoints' : $('#edit-userpoints-trans-lcpoints').val(),
        '@ucpoint' : $('#edit-userpoints-trans-ucpoint').val(),
        '@lcpoint' : $('#edit-userpoints-trans-lcpoint').val()
        }

        return Drupal.t('@ucpoints, @lcpoints, @ucpoint, @lcpoint.', branding);
      });

      // Category.
      $('fieldset#edit-category', context).drupalSetSummary(function (context) {
        var category = $('select#edit-userpoints-category-default-tid :selected').text();
        var display_selection = $('div.form-item-userpoints-category-profile-display-tid input:checked').siblings('label');
        var display_categories;
        if (display_selection.length > 0) {
            display_categories = jQuery.trim(display_selection[0].firstChild.nodeValue);
            for (var i = 1; i < display_selection.length; i++) {
                if (i > 2) {
                    display_categories += ', ...';
                    break;
                }
                display_categories += ', ' + jQuery.trim(display_selection[i].firstChild.nodeValue);
            }
        }
        else {
            display_categories = Drupal.t('none');
        }

        return Drupal.t('Default: %category<br />Displayed: %display_categories', {'%category' : category, '%display_categories' : display_categories});
      });

      // Stamping.
      $('fieldset#edit-stamping', context).drupalSetSummary(function (context) {
        if ($('input#edit-userpoints-transaction-timestamp').is(':checked')) {
          return Drupal.t('Always use system time for transactions.');
        }
        else {
          return Drupal.t('Allow customization of transaction time.');
        }
      });

      // Expiration.
      $('fieldset#edit-points-expiration', context).drupalSetSummary(function (context) {

        var year = $('select#edit-userpoints-expireon-date-year :selected').val();
        var month = $('select#edit-userpoints-expireon-date-month :selected').val();
        var day = $('select#edit-userpoints-expireon-date-day :selected').val();
        var date = new Date(year, month, day);

        // If expiration time is in the future, use that.
        if (date.getTime() > new Date().getTime()) {
          return Drupal.t('Expiration at %date.', {'%date' : date.toLocaleDateString()});
        }
        else if ($('select#edit-userpoints-expireafter-date :selected').val() > 0) {
          return Drupal.t('Expiration in %date.', {'%date' : $('select#edit-userpoints-expireafter-date :selected').text()});
        }
        else {
          return Drupal.t('No expiration.');
        }
      });
    }
  };

})(jQuery);