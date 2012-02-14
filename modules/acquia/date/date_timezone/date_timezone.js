// $Id: date_timezone.js,v 1.1.4.2.2.1 2008/06/20 12:25:30 karens Exp $
/**
 * Set the client's system time zone as default values of form fields.
 */
Drupal.setDefaultTimezone = function() {
  var dateString = Date();
  // In some client environments, date strings include a time zone 
  // abbreviation which can be interpreted by PHP.
  var matches = Date().match(/\(([A-Z]{3,5})\)/);
  var abbreviation = matches ? matches[1] : 0;

  // For all other client environments, the abbreviation is set to "0" 
  // and the current offset from UTC and daylight saving time status are 
  // used to guess the time zone.
  var dateNow = new Date();
  var offsetNow = dateNow.getTimezoneOffset() * -60;

  // Use January 1 and July 1 as test dates for determining daylight 
  // saving time status by comparing their offsets.
  var dateJan = new Date(dateNow.getFullYear(), 0, 1, 12, 0, 0, 0);
  var dateJul = new Date(dateNow.getFullYear(), 6, 1, 12, 0, 0, 0);
  var offsetJan = dateJan.getTimezoneOffset() * -60;
  var offsetJul = dateJul.getTimezoneOffset() * -60;

  // If the offset from UTC is identical on January 1 and July 1, 
  // assume daylight saving time is not used in this time zone.
  if (offsetJan == offsetJul) {
    var isDaylightSavingTime = '';
  }
  // If the maximum annual offset is equivalent to the current offset, 
  // assume daylight saving time is in effect.
  else if (Math.max(offsetJan, offsetJul) == offsetNow) {
    var isDaylightSavingTime = 1;
  }
  // Otherwise, assume daylight saving time is not in effect.
  else {
    var isDaylightSavingTime = 0;
  }

  // Submit request to the user/timezone callback and set the form field 
  // to the response time zone.
  var path = 'user/timezone/' + abbreviation + '/' + offsetNow + '/' + isDaylightSavingTime;
  $.getJSON(Drupal.settings.basePath, { q: path, date: dateString }, function (data) {
    if (data) {
      $("#edit-date-default-timezone, #edit-user-register-timezone, #edit-timezone-name").val(data);
    }
  });
};