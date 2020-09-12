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
        // Format the date.
        var date = new Date();
        const year = new Intl.DateTimeFormat('en', { year: 'numeric' }).format(date)
        const month = new Intl.DateTimeFormat('en', { month: '2-digit' }).format(date)
        const day = new Intl.DateTimeFormat('en', { day: '2-digit' }).format(date)
        var localFormattedDate = `${year}-${month}-${day}`;
        $(this).html(Drupal.t('Your time is: @time. <br/> Leave blank to use the time of submission ("Now").',
          {
            '@time': localFormattedDate,
          })
        )
      })
    }
  };
}
)(jQuery, Drupal);
