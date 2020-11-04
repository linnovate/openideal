(function ($, Drupal) {

  /**
   * Attach behaviours on user login.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Add svg underline for <em> tag.
   */
  Drupal.behaviors.openidealLoginUndeliner = {
    attach: function (context, settings) {
      if (settings.loginPage.underliner) {
        var $span = $('.user-login-form--container--right__text--section__first span').once('openideal_login_undeliner');
        if ($span.length) {
          var $em = $('.user-login-form--container--right__text--section__first em').get(0);
          $span.css('width', $em.offsetWidth)
          $span.css('height', $em.offsetHeight)
          new Underliner('.user-login-form--container--right__text--section__first em', '#27c0ff', '#27c0ff', 4, 6);
        }
      }
    }
  }

}
)(jQuery, Drupal);
