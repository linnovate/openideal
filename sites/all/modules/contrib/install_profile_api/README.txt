# $Id: README.txt,v 1.1.4.2.2.1 2009/03/20 07:23:58 dww Exp $

Original author: Boris Mann (http://drupal.org/user/4426)
Co Maintainers:
  Nathan Haug "quicksketch" (http://drupal.org/user/35821)
  Simon Hobbs "sime" (http://drupal.org/user/35266)
  Jean-Sebastien Senecal "tatien" (http://drupal.org/user/1323)
  Moshe Weitzman (http://drupal.org/user/23)
  Derek Wright "dww" (http://drupal.org/user/46549)


This is the start of helper functions for people creating install profiles.


== Instructions ==

1. Put "install_profile_api" in your profile's hook_profile_modules() array.
2. In your profile's hook_profile_tasks(), call install_include() like so:

function foo_profile_tasks() {
  install_include(foo_profile_modules());
  // Whatever else you need to do...
}


== Discussion ==

* First announcement: http://groups.drupal.org/node/3179
* "Distributions" Group: http://groups.drupal.org/distributions
* Support, feature requests, etc.:
  http://drupal.org/project/issues/install_profile_api
