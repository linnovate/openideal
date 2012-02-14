<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
   <style type="text/css" media="screen">
      body, table.htmlmail-background {
         background-color: #162f45;
      }
    
      a {
        color:#386690
      }

      h1, h2, h3, h4, h5, h6 {
      color:#f4793a;
      }

      table.htmlmail-main {
         background-color: #ffffff;
      }

      td.htmlmail-header {
         background-color:#5f707f;
         font-family:Arial, Helvetica, sans-serif;
         color: #ffffff;
         font-size: 14px;
         padding: 10px 20px 10px 20px;
      }

      td.htmlmail-header a,
      td.htmlmail-footer a {
         color: #aebac5;
      }

      td.htmlmail-body {
         font-family:Arial, Helvetica, sans-serif;
         font-size: 12px;
         padding: 10px 20px 10px 20px;
         background-color: #ffffff;
      }

      td.htmlmail-footer {
         font-family:Arial, Helvetica, sans-serif;
         font-size: 11px;
         color: #ffffff;
         line-height: 16px;
         padding: 10px 20px 10px 20px;
         background-color: #5f707f;
         vertical-align: middle;
      }
   <?php print $css; ?>
   </style>
</head>

<body>
  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="htmlmail-background">
    <tr>
      <td align="center">
        <table width="600" border="0" cellspacing="0" cellpadding="0" class="htmlmail-main">
          <tr>
            <td valign="top" align="left">
              <img src="<?php print $path; ?>/htmlmail_images/htmlmail-header.png" height="76px" width="600px">
            </td>
          </tr>
          <?php if ($header): ?>
          <tr>
            <td valign="top" align="left" class="htmlmail-header">
              <?php print $header; ?>
            </td>
          </tr>
          <?php endif; ?>

          <tr>
            <td valign="top" align="left" class="htmlmail-body">
              <?php print $body; ?>

            </td>
          </tr>

          <tr>
            <td valign="middle" align="left" class="htmlmail-footer">
              <?php print $footer; ?>
            </td>
          </tr>
        </table>
      </td>
    </tr>
  </table>
</body>
</html>
