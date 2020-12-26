/**
 * @file
 * Autocomplete based on jQuery UI.
 */

(function ($, Drupal, window) {
  'use strict';

  /**
   * Attaches the autocomplete behavior to all required fields.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attaches the autocomplete behaviors.
   */
  Drupal.behaviors.openidealIdeaAutocomplete = {
    attach: function (context, settings) {
      var $autocomplete = $(context).find('.field--name-title input.form-autocomplete').once('openideal_idea_autocomplete');

      if ($autocomplete.length) {
        $autocomplete.on('autocompleteselect', function (event, ui) {
          window.location.replace(ui.item.url);
        })
      }
    }
  }

})(jQuery, Drupal, window);
