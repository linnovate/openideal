/**
 * @file
 * Global utilities.
 *
 */
(function ($, Drupal) {

  'use strict';

  /**
   * Attach behaviours on homepage.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Implement cascading grid layout library.
   */
  Drupal.behaviors.openidealContentHomePage = {
    attach: function (context, settings) {
      $('.view-frontpage .view-content', context).once('openideal_content_home_page').each(function () {
        var $this = $(this);
        // Ensure that images are loaded.
        $this.imagesLoaded(function () {
          $this.masonry({
            itemSelector: '.views-row',
            horizontalOrder: true,
          });
        })
      })
    }
  }
}
)(jQuery, Drupal);
