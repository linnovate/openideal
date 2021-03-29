(function ($, Drupal) {

    'use strict';

    /**
     * Openideal Toolbar behavior.
     *
     * @type {Drupal~behavior}
     *
     * @prop {Drupal~behaviorAttach} attach
     *   Attach icon for OI toolbar.
     */
    Drupal.behaviors.openidealToolbar = {
      attach: function (context, settings) {
        $('.toolbar-icon-openideal-toolbar-help').once('openideal_toolbar').each(function () {
          var logo = settings.openidealToolbar.logo;
          $(this).css('--toolbar-url', `url(${logo})`)
        })
      }
    }
}
)(jQuery, Drupal);
