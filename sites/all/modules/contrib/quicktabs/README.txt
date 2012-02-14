*******************************************************************************
$Id: README.txt,v 1.7.8.3 2010/09/11 17:45:46 katbailey Exp $

quicktabs

Description:
-------------------------------------------------------------------------------

  This module provides a form for admins to create a block of tabbed content by
selecting first the desired number of tabs and then selecting either an existing
view or an existing block as the content of each tab. Arguments can be passed if
a view is selected.



Installation & Use:
-------------------------------------------------------------------------------

1.  Enable module in module list located at administer > build > modules.
2.  Go to admin/settings/quicktabs to select a style for your tabs
3.  Go to admin/build/quicktabs and click on the "New QT block" local task tab.
4.  Add a title for your block and start entering information for your tabs
5.  Use the Add another tab button to add more tabs.
6.  Use the drag handles on the left to re-arrange tabs.
7.  Once you have defined all the tabs, click 'Next'.
8.  You will be taken to the admin/build/block screen where you should see yor new tabbed block listed.
9.  Configure & enable it as required.


Note:
-------------------------------------------------------------------------------
Because Quicktabs allows your tabbed content to be pulled via ajax, it has its
own menu callback for getting this content and returning it in JSON format. For
node content, it uses the standard node_access check to make sure the user has
access to this content. It is important to note that ANY node can be viewed
from this menu callback; if you go to it directly at quicktabs/ajax/node/[nid]
it will return a JSON text string of the node information. If there are certain 
fields in ANY of your nodes that are supposed to be private, these MUST be 
controlled at admin/content/node-type/MY_NODE_TYPE/display by setting them to 
be excluded on teaser and node view. Setting them as private through some other 
mechanism, e.g. Panels, will not affect their being displayed in an ajax Quicktab.



Author:
-------------------------------------------------------------------------------

Katherine Bailey <katherine@raincitystudios.com>
http://drupal.org/user/172987
Tab Styles provided by Hubert Florin and Steve Krueger

