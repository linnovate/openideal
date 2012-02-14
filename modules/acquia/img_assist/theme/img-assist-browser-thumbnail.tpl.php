<?php
// $Id: img-assist-browser-thumbnail.tpl.php,v 1.1.2.1 2008/10/12 17:04:12 sun Exp $

/**
 * @file
 * This template outputs individual thumbnails in Image assist's image browser.
 * 
 * Available variables:
 *   object $view
 *   array $options
 *   object $row
 *   string $zebra
 *   int $id
 *   string $directory
 *   bool $is_admin
 *   bool $is_front
 *   bool $logged_in
 *   bool $db_is_active
 *   object $user
 * 
 *   string $thumbnail
 *   string $path
 *   string $title
 */
?>
<a href="<?php print $path; ?>" title="<?php print $title; ?>"><?php print $thumbnail; ?></a>
