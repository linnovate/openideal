********************************************************************
                P A G E    T I T L E    M O D U L E
********************************************************************
Original Author: Robert Douglass
Current Maintainers: Nicholas Thompson and John Wilkins

********************************************************************
DESCRIPTION:

   This module gives you control over the page title. It gives you the chance
   to provide patterns for how the title should be structured, and on node
   pages, gives you the chance to specify the page title rather than defaulting
   to the node title.

********************************************************************
PERMISSIONS:

   This module defines the "set page title" and "administer page titles"
   permissions. The "set page title" permission determines whether a user will
   be able to edit the "Page title" field on node edit forms (if visible.) The
   "administer page titles" permission determines whether a user will be able to
   edit the "Page title" administration pages.

********************************************************************
INSTALLATION:

1. Place the entire page_title directory into your Drupal modules/
   directory or the sites modules directory (eg site/default/modules)


2. Enable this module by navigating to:

     Administer > Build > Modules

   At this point the Drupal install system will attempt to create the database
   table page_title. You should see a message confirming success or
   proclaiming failure. If the database table creation did not succeed,
   you will need to manually add the following table definition to your
   database:

    CREATE TABLE `page_title` (
      `nid` INT NOT NULL ,
      `page_title` VARCHAR( 128 ) NOT NULL ,
      PRIMARY KEY ( `nid` )
    ) /*!40100 DEFAULT CHARACTER SET utf8 */;

3. Optionally configure the two variations of page title by visiting:

    Administer > Content management > Page titles

4. The page title is ultimately set at the theme level. To let your PHPTemplate
   based theme interact with this module, you need to add some code to the template.php
   file that comes with your theme. If there is no template.php file, you can simply
   use the one included with this download. Here is the code:

function _phptemplate_variables($hook, $vars) {
  $vars = array();
  if ($hook == 'page') {

    // These are the only important lines
    if (module_exists('page_title')) {
      $vars['head_title'] = page_title_page_get_title();
    }

  }
  return $vars;
}

  As you can see from the code comment, there are only three important lines
  of code:

  if (module_exists('page_title')) {
    $vars['head_title'] = page_title_page_get_title();
  }

  These lines need to be added to the 'page' hook of the _phptemplate_variables
  function.

  Alternately, you can call page_title_page_get_title() from page.tpl.php
  directly at the place where the title tag is generated.
