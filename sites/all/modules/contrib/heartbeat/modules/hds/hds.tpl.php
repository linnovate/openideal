<?php
// $Id: hds.tpl.php,v 1.1.2.2 2010/02/21 12:31:33 stalski Exp $

/**
 * @file
 * Template file for one row, rendered by heartbeat
 *
 * @var
 * - $message : after it was parsed by heartbeat (grouped)
 * - $row : All parts that are separately themeable
 * -   $row->message : string activity message
 * -   $row->time_info : information about the time of activity
 * -   $row->class : extra classes to use on the row
 * -   $row->attachments : attachment on the message id (of the grouped message)
 *
 * @remarks
 *   beat-item-<uaid> is necessairy. The beat item id is used to toggle
 *   visibility of the "number more" messages when grouping exceeded the
 *   maximum allowed grouped property.
 */
?>
<div class="heartbeat-message-block <?php print $message->message_id . ' ' . $zebra; ?>">

  <div class="beat-item" id="beat-item-<?php print $message->uaid ?>">

    <?php print $content; ?>

  </div>

</div>