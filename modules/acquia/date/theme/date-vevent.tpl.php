<?php
// $Id: date-vevent.tpl.php,v 1.1.2.2 2008/10/10 20:59:15 karens Exp $
/**
 * $event
 *   An array with the following information about each event:
 * 
 *   $event['uid'] - a unique id for the event (usually the url).
 *   $event['summary'] - the name of the event.
 *   $event['start'] - the formatted start date of the event.
 *   $event['end'] - the formatted end date of the event.
 *   $event['rrule'] - the RRULE of the event, if any.
 *   $event['timezone'] - the formatted timezone name of the event, if any.
 *   $event['url'] - the url for the event.
 *   $event['location'] - the name of the event location, or a vvenue location id.
 *   $event['description'] - a description of the event.
 *   $event['alarm'] - an optional array of alarm values.
 *    @see date-valarm.tpl.php.
 */
?>
BEGIN:VEVENT
UID:<?php print($event['uid'] . "\n") ?>
SUMMARY:<?php print($event['summary'] . "\n") ?>
DTSTAMP;TZID=<?php print $site_timezone ?>;VALUE=DATE-TIME:<?php print($current_date . "\n") ?>
DTSTART;<?php print $event['timezone'] ?>VALUE=DATE-TIME:<?php print($event['start'] . "\n") ?>
<?php if (!empty($event['end'])): ?>
DTEND;<?php print $event['timezone'] ?>VALUE=DATE-TIME:<?php print($event['end'] . "\n") ?>
<?php endif; ?>
<?php if (!empty($event['rrule'])) : ?>
RRULE;<?php print($event['rrule'] . "\n") ?>
<?php endif; ?>
<?php if (!empty($event['url'])): ?>
URL;VALUE=URI:<?php print($event['url'] . "\n") ?>
<?php endif; ?>
<?php if (!empty($event['location'])): ?>
LOCATION:<?php print($event['location'] . "\n") ?>
<?php endif; ?>
<?php if (!empty($event['description'])) : ?>
DESCRIPTION:<?php print($event['description'] . "\n") ?>
<?php endif; ?>
END:VEVENT