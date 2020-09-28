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
        const year = new Intl.DateTimeFormat('en', {year: 'numeric'}).format(date)
        const month = new Intl.DateTimeFormat('en', {month: '2-digit'}).format(date)
        const day = new Intl.DateTimeFormat('en', {day: '2-digit'}).format(date)
        var localFormattedDate = `${day}/${month}/${year}`;
        $(this).html(Drupal.t('Your time is: @time. <br/> Leave blank to use the time of submission ("Now").',
        {
          '@time': localFormattedDate,
        })
        )
      })
    }
  };

  /**
   * Change html5 date input view format.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach dynamic format change to date.
   */
  Drupal.behaviors.openidealChallengeHMTL5Date = {
    attach: function () {
      $('input[type="date"]').once('openideal_challenge_html5_date').each(function () {
        $(this).on('change', function () {
          var attribute = this.value == '' ? 'dd/mm/yyyy' : moment(this.value, 'YYYY-MM-DD').format(this.getAttribute('data-date-format'))
          this.setAttribute('data-date', attribute);
        }).trigger("change")
      })
    }
  }
}
)(jQuery, Drupal);
