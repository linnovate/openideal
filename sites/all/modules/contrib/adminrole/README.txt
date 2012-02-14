// $Id: README.txt,v 1.1.2.1.2.2 2009/11/25 15:49:00 davereid Exp $

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Frequently Asked Questions (FAQ)
 * Known Issues
 * How Can You Contribute?


INTRODUCTION
------------

Creator: JacobSingh <http://drupal.org/user/68912>
Co-maintainer: Bevan <http://drupal.org/user/49989>
Co-maintainer: Dave Reid <http://drupal.org/user/53892>

This module is a little helper to maintain an administrator role which has all
available permissions. By default, Drupal only has one super user and this
module helps improve this drastically.

Note the update.php will still only work for the "real" admin (user #1).


INSTALLATION
------------

See http://drupal.org/getting-started/install-contrib for instructions on
how to install or update Drupal modules.

If you don't already have an existing role named 'admin' or 'administrator',
the module will automatically create one for you on install. Otherwise, you can
go to your Admin > Users > User Settings (admin/user/settings) page and select
your preferred admin role under the 'Administrator role' fieldset.

Now when you add a new module, your assigned role will automatically receive
any new available permissions!


KNOWN ISSUES
------------

- There are no known issues at this time.


HOW CAN YOU CONTRIBUTE?
---------------------

- Write a review for this module at drupalmodules.com.
  http://drupalmodules.com/module/admin-role

- Help translate this module.
  http://localize.drupal.org/translate/projects/adminrole

- Report any bugs, feature requests, etc. in the issue tracker.
  http://drupal.org/project/issues/adminrole
