// $Id: README.txt,v 1.1.4.3 2009/09/24 13:41:05 aaron Exp $

README for Embedded Inline Media

This module provides the ability to parse URLs of third party media providers
from a node or comment content, and display the media appropriately.

Experimental; currently only works for video.

After enabling the module, you need to go to your Input Filters administration
at /admin/settings/filters, configure the format you wish (such as Full HTML at
/admin/settings/filters/2), check the box to allow Embedded Inline Media, and
then configure the filter (such as at /admin/settings/filters/2/configure).

In the filter's configuration, you'll then check the providers you wish to
allow, and any other desired settings. Finally, when submitting a node, you'll
need to ensure the proper filter is selected, and simply paste the URL (no
brackets). Note that this will conflict with the URL filter if that is enabled,
so you'll need to rearrange the filters to ensure the Embedded Inline Media
filter is above that.
