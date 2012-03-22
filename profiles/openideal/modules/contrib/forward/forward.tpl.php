<?php

/**
 * This template should only contain the contents of the body
 * of the email, what would be inside of the body tags, and not
 * the header.  You should use tables for layout since Microsoft
 * actually regressed Outlook 2007 to not supporting CSS layout.
 * All styles should be inline.
 *
 * For more information, consult this page:
 * http://www.anandgraves.com/html-email-guide#effective_layout
 *
 * If you are upgrading from an old version of Forward, be sure
 * to visit the Forward settings page to enable use of the new
 * template system.
 */
?>
<html>
  <body>
    <table width="<?php print $width; ?>" cellspacing="0" cellpadding="10" border="0">
      <thead>
        <tr>
          <td>
            <h1 style="font-family:Arial,Helvetica,sans-serif; font-size:18px;"><a href="<?php print $site_url; ?>" title="<?php print $site_name; ?>"><?php print $logo; ?> <?php print $site_name; ?></a></h1>
          </td>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $forward_message; ?>
            <?php if ($message) { ?>
            <p><?php print t('Message from Sender'); ?></p><p><?php print $message; ?></p>
            <?php } ?>
            <?php if ($title) { ?><h2 style="font-size: 14px;"><?php print $title; ?></h2><?php } ?>
            <?php if ($submitted) { ?><p><em><?php print $submitted; ?></em></p><?php } ?>
            <?php if ($node) { ?><div><?php print $node; ?></div><?php } ?><p><?php print $link; ?></p>
          </td>
        </tr>
        <?php if ($dynamic_content) { ?><tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $dynamic_content; ?>
          </td>
        </tr><?php } ?>
        <?php if ($forward_ad_footer) { ?><tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $forward_ad_footer; ?>
          </td>
        </tr><?php } ?>
        <?php if ($forward_footer) { ?><tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $forward_footer; ?>
          </td>
        </tr><?php } ?>
      </tbody>
    </table>
  </body>
</html>