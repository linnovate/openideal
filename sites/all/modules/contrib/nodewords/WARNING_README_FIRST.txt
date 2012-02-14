# $Id: WARNING_README_FIRST.txt,v 1.1.2.3 2010/03/23 16:35:30 kiam Exp $
#

Before to install or update to a new version, read what reported in the project
page, in particular read what reported in
http://drupal.org/project/nodewords#version_notes,
http://drupal.org/project/nodewords#update, and
http://drupal.org/project/nodewords#update_branch_6_1.

1. If you are updating to an alpha, beta, or release candidate, test the version on
a test site, preferably a test site with the same modules installed in the
production site. It's always suggested to backup the database tables before to
install/update the module on a production site.

2. When you update the project files to a later version, always delete the old
files before to copy the new ones.
Some files could have been renamed, and you would have old files together new
files. This would create problems, and would make update.module report you still
installed the older version.

3. Updating between different releases of the development snapshot of the same
branch is not supported; if you install it, you need to uninstall the previous
release before to install the new release.
Development snapshot are thought to be used to help with the development, and
they should never be installed on a production site. Even if the fix for an
issue has been applied to a development snapshot, don't install it in a
production site; wait for a new official release, and if the official release is
a alpha release, beta release, or a release candidate, follow what reported at
point #1.