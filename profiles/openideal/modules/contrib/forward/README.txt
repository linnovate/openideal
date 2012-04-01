

        FORWARD MODULE - README
______________________________________________________________________________

NAME:       Forward
AUTHORS:    Sean Robertson <seanr@ngpsoftware.com>
            Peter Feddo
______________________________________________________________________________


DESCRIPTION

Adds a "forward this page" link to each node. This module allows users to
forward a link to a specific node on your site to a friend.  You can customize
the default form field values and even view a running count of the emails sent
so far using the forward module.


INSTALLATION

Step 1) Download latest release from http://drupal.org/project/forward

Step 2)
  Extract the package into your 'modules' directory.


Step 3)
  Enable the forward module.

  Go to "/admin/modules" and put a checkmark in the 'Enabled' column next to
  'Forward'.


Step 4)
  Go to "/admin/config/user-interface/forward" to configure the module.
  This path is also linked from the Configuration page and the Modules list
  page within site administration.

  If you wish to customize the emails, copy 'forward.tpl.php' into your theme
  directory. Then you can customize the function as needed and those changes
  will only appear went sent by a user using that theme.

  If you check the 'custom display' box on the configuration page, the Forward
  view mode which defines the fields that will be sent in Forward emails can
  be configured here:

  "/admin/structure/types/manage/[machine-name]/display/forward"

  where [machine-name] is replaced by the machine name of the content type
  being configured.

  For example, for articles, the full path to the link is:
  "/admin/structure/types/manage/article/display/forward"


Step 5)
  Enable permissions appropriate to your site.

  Go to "/admin/people/permissions#module-forward" to configure permissions.
  This path is also linked from the Modules list page, click on the 
  Permissions link next to Forward.

  The forward module provides several permissions:
   - 'access forward': allow user to forward pages.
   - 'access epostcard': allow user to send an epostcard.
   - 'override email address': allow logged in user to change sender address.
   - 'administer forward': allow user to configure forward.
   - 'override flood control': allow user to bypass flood control on send.

  Note that you need to enable 'access forward' for users who should be able
  to send emails using the forward module.


Step 6)
  Go to "/admin/reports/forward" to view forward usage statistics.
  There is also a link on the Reports page within site administration.

  Statistics are captured when emails are sent and when recipients click on
  links within the sent emails.

Step 7)
  If the Views module is enabled for your site, go to "admin/structure/views"
  to optionally enable and configure Forward related views.  These views
  are available:

  forward_forwards - most popular nodes to forward
  forward_recent - recently forwarded nodes or pages
  forward_clickthroughs - forwarded nodes with most clicks from sent emails

Step 8)
  Go to "admin/structure/block" to optionally enable and configure Forward
  blocks for your theme.  Several blocks are available:

  Forward: Form - places the forward link or forward form in a block
  Forward: Statistics - most recently emailed or most emailed of all time

  If you enabled views in step 7, these blocks are also available:

  forward_forwards:Block - a Views list of most popular nodes to forward
  forward_recent:Block - a Views list of recently forwarded nodes or pages
  forward_clickthroughs:Block - a Views list of nodes with most clicks


DYNAMIC BLOCK ACCESS CONTROL

The 7.x-1.3 release of the Forward module added a new security field
for administators on the Forward configuration page named Dynamic Block
Access Control.  This field allows the administrator to control which
permissions are used when Drupal applies access control checks to the nodes,
comments or users listed in the Dynamic Block.  Several access control
options are available, including a bypass option.  The bypass option allows
the email recipient to possibly view node titles, comment titles, or user
names that only privileged users should see. The bypass option should not
normally be selected, but is provided for sites that used prior versions
of Forward and rely on the access bypass to operate correctly.

IMPORTANT: Because the default for the new field is to apply access control,
administrators of sites that rely on the access bypass to operate correctly
need to visit the Forward configuration page and explicitly select the bypass
option after upgrading from versions of Forward prior to 7.x-1.3.


CLICKTHROUGH COUNTER FLOOD CONTROL

The Forward module tracks clicks from links in sent emails to determine which
nodes get the most clickthroughs.  The method used could allow someone to
manipulate clickthrough counts via CSRF - for example, placing an image on
a website with a src tag that points to the clickthrough counter link.  The
module uses flood control to limit the number of clickthroughs from a given
IP address in a given time period to migitate this possibility.


CREDITS & SUPPORT

Special thanks to Jeff Miccolis of developmentseed.org for supplying the
tracking features and various other edits.  Thanks also to Nick White for his
EmailPage module, some code from which was used in this module, as well as the
numerous other users who have submitted issues and patches for forward.

John Oltman of sitebasin.com assisted with development of the Drupal 7 version.

All issues with this module should be reported via the following form:
http://drupal.org/node/add/project_issue/forward

______________________________________________________________________________
http://www.ngpsystems.com
