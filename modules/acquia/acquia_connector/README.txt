Acquia Network Connector modules
================================================================================

The Acquia Network [1] enhances the Drupal experience by providing the support
and network services to operate a trouble-free Drupal website. Subscribers to
the Acquia Network gain access to remote network services, documentation and
the Acquia Network's subscriber forums. Premium subscriptions provide
web-based ticket management, as well as email and telephone support.

These modules allow you to connect any Drupal 6.x site to the Acquia Network.
Acquia also has a distribution of Drupal called Acquia Drupal which is
composed of purely open source GPL licensed components. If you are looking
for a quick start with Drupal, Acquia Drupal [2] might be of great use for you.
(Note that a few Acquia Network services, such as the update notification and
code modification detection services, currently only work with Acquia Drupal.)

[1] http://acquia.com/products-services/acquia-network
[2] http://acquia.com/products-services/acquia-drupal

Modules in this project
--------------------------------------------------------------------------------

Acquia agent: Enables secure communication between your Drupal sites and
the Acquia Network to monitor uptime, check for updates, and collect site
information.

Acquia Site profile: Automates the collection of site information -
operating system, database, webserver, and PHP versions, installed modules,
and site modifications - to speed support communication and issue resolution.

Installation
--------------------------------------------------------------------------------

If you just start using Drupal, we suggest you consider starting with Acquia
Drupal, which includes these modules and provides an easier start up experience
with the Acquia Network.

If you choose to install the Acquia Network Connector modules to an existing
Drupal 6 site, please do the following:

 1. Copy the acquia_connector directory to under sites/all/modules or one of
    the other places where Drupal finds modules.
 2. Go to the Administer >> Site building >> Modules page, and enable both
    submodules.
 3. You will be prompted to enter your Acquia Network connection details.
    If you did not set up an Acquia Network subscription yet, go to
    http://acquia.com/ and choose an appropriate option.
 4. Ready. Enjoy using your Acquia Network subscription at
    http://acquia.com/network.
    
Read more in Acquia's Getting Started Guide at http://acquia.com/downloads

Maintainers
--------------------------------------------------------------------------------

These modules are maintained by developers at Acquia. For more information on
the company and our offerings, see http://acquia.com/

Issues
--------------------------------------------------------------------------------

Contact Acquia Support if you have support questions regarding your site.

If you have issues with the submodules included in the Acquia Network
Connector package, you are also welcome to submit issues at
http://drupal.org/project/acquia_connector (all submitted issues are public).
