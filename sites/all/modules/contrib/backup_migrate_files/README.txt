// $Id: README.txt,v 1.1 2009/08/22 20:57:01 ronan Exp $

-------------------------------------------------------------------------------
Backup and Migrate Files for Drupal 6.x
  by Ronan Dowling, Gorton Studios - ronan (at) gortonstudios (dot) com
-------------------------------------------------------------------------------

DESCRIPTION:
This module extends the Backup and Migrate module adding the ablilty to back up
files from within that module.

** READ ALL OF THE INSTALLATION INSTRUCTIONS BELOW BEFORE PROCEEDING. **

-------------------------------------------------------------------------------

INSTALLATION:
* Download and install Backup and Migrate (http://drupal.org/project/backup_migrate)
* Put this module in your modules directory and enable it at admin/build/modules.
* If necessary, install PEAR and Archive_Tar as described below.
* Configure and use the module at admin/content/backup_migrate.

-------------------------------------------------------------------------------

INSTALL PEAR and Archive_Tar:
In order to operate, this module requires the PEAR Archive_Tar class. You may
install it in one of the following ways:

1. WITH THE PEAR COMMAND LINE COMMAND
  If you have PEAR on your server you may be able to just run:
    pear install Archive_Tar-1.3.3
  at the command line to install the Archive_Tar library.

2. WITH PEAR INSTALLED BUT NO PEAR COMMAND
  You may have the PEAR library already installed but the 'pear' command line 
  program is not available. If this is the case, you can download Archive_Tar at
    http://download.pear.php.net/package/Archive_Tar-1.3.3.tgz
  Expand this archive and place the Tar.php file in the 
    includes/
  directory of this module.

3. WITHOUT PEAR INSTALLED
  If PEAR is not installed on your server, you can either install it using the
  instructions listed on the pear website:
    http://pear.php.net/manual/en/installation.introduction.php
  or you can simply download the PEAR package at
    http://download.pear.php.net/package/PEAR-1.8.1.tgz
  Expand this archive and copy PEAR.php and PEAR5.php into the
    includes/
  directory of this module. Once you have installed PEAR, you can follow the 
  instructions in option 2 above.

