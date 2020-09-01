/**
 * @file
 * Attaches behaviors for the Openideal Challenge module's schedule widget.
 */

(function ($, Drupal, window, document) {

  /**
   * Schedule behaviours.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach local time to the schedule widget.
   */
  Drupal.behaviors.openidealChallengeSchedule = {
    attach: function (context, settings) {
      $('.challenge-schedule-local-machine-time').once('openideal_challenge_schedule').each(function () {
        $(this).html(Drupal.t('Your time is: @time. <br/> Leave blank to use the time of submission ("Now").',
          {'@time': new Date().toLocaleTimeString()})
        )
      })
    }
  };
}
)(jQuery, Drupal);
