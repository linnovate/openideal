$Id: README.txt,v 1.1 2010/07/01 12:31:50 mcjim Exp $

The bookblock module can generate an individual menu block for each of your site's books.
These blocks can then be administered as any other block to appear on the pages you choose. You aren't limited to making them appear on book pages only.

Hopefully this will help such problems as http://drupal.org/node/44648 and http://drupal.org/node/838728.

Background
----------

The core book module provides one book navigation block.
You can configure it to contain the automatically generated menus for all of the site's books (the all or nothing approach).
Alternatively, you can configure it to contain only the one menu corresponding to the current page's book. In this case, if the current page is not in a book, no block will be displayed.

This is useful, but occasionally you may need an individual book's navigation menu to appear on non-book pages as well, or to show up in an additional region (e.g. in the footer).

This is where this module comes in handy.

Installation
------------

See http://drupal.org/node/70151 for more information on installing modules.

How to use
----------

Once the module is enabled, you can use it to create a navigation block for each book you have created.
If you haven't yet created any books, you will need to do so first. See http://drupal.org/handbook/modules/book for more information about books.

The administration page for bookblock is found at Administer > Content Management > Books > Book Blocks (http://example.com/admin/content/book/blocks).

The books you have created on your site will be listed. Select which ones you would like to create a book navigation block for and they will then be available to you on the blocks administration page, where you can control on which pages they appear and in which region (or use Context or Panels if you prefer).

N.B. Only books with child pages will actually display anything!

Permissions
-----------

There is no specific permission for bookblock. It is available to administrators with the "administer site configuration" permission.

Credits
-------

Authored by James Panton (mcjim).

This project has been sponsored by menusandblocks ltd, specialists in Drupal development and training.
Visit http://menusandblocks.co.uk for more information.