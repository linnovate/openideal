Drupal service links module 2.x:
---------------------------------
Original Author: Fredrik Jonsson fredrik at combonet dot se
Ex Maintainer: Sivanandhan, P. apsivam .at. apsivam .dot. in
Current Mantainer and Starter of 2.x branch: Fabio Mucciante aka TheCrow
Current Co-Mantainer: Rob Loach
Requires - Drupal 7
License - GPL (see LICENSE)

Overview:
---------
Service Links provide an amount of 70+ social services
from around the World where submit the link of a given content;
below a short list:

* del.icio.us
* Digg
* Facebook
* Furl
* Google
* IceRocket
* LinkedIn
* ma.gnolia.com
* MySpace
* Newsvine
* PubSub
* Reddit
* StumbleUpon
* Technorati
* Twitter
* Yahoo Buzz
* Yahoo
* ...

The admin can decide:
- To show the links as text, image or both.
- To show only for certain node types or some categories
- To show in teaser view or full page view or both.
- If the links should be added after the body text or in the links
  section or in a block
- Decide what roles get to see/use the service links.

2.x version introduced:
- modular management of services grouped by different language area
- visual sort of Services through drag'n drop
- a block with Fisheye effect
- a block for not-node pages
- support for other Drupal modules: Forward, Views, Short Url, Sharethis, Share
- support for browser bookmark (Chrome, Firefox, IE, Opera)
- auto-hide for unpublished nodes (configurable)
- configurable label for the block shown in the node
- params can be stick to the url address
- custom icon's path

And plus, the support for aggregator2 has been removed (obsolete) but it work
well with aggregation

Installation and configuration:
------------------------------
Copy the whole 'service_links' folder under your 'modules' directory and then
enable the modules 'Service Links' and 'General Services' at 'administer >> modules'.

Go to 'administer >> access control' for allow users to watch the links.

For configurate the options go to at 'administer >> settings >> service_links'.
Under the tab 'Services' sort and enable the services needed.

Extend the list of services (for developers):
-------------------------------------------
2.x branch introduce a fast and less intrusive method for expand the number of services
supported:

1) Create your own module under 'services/' folder with standard
  '.info' and '.module' files (watch general_services as basic example).

  .module file must implement the hook_service_links() that return an array like:

  function myaddon_service_links() {
    $links = array();

    $links['myservice'] = array(
      'name' => 'My Service',
      'link' => 'http://myservice.com/?q=<encoded-url>&title=<encoded-title>',
      'description' => t('Bookmark it on My Service'),
    );

    ...

    return $links;
  }

  Notes:
  i) be sure that 'myservice' (know as 'service-id') is unique;
  ii) tags allowed: <encoded-url>, <encoded-title>, <encoded-teaser>, <encoded-short-url>, <encoded-query>, <query>, <source>, <teaser>, <node-id>, <short-url>

2) Put the related standard icon (myservice.png) under 'images/' folder .

  Notes:
  i) standard filename must be the same of service-id + .png extension
  ii) for overwrite the standard filename just include the key 'icon':
    $links['myservice'] = array(
      ...
      'icon' => drupal_get_path('module', 'myservice') .'/anothername.gif',
    );

3) Enable the module under admin >> modules page and under settings >> service links >> services
  complete the job!

Theme's customizations
----------------------
In the included template.php file there are examples about how to create
a PHPTemplate variable for your theme. Remember to place the
template.php file in the folder of your theme or integrate it with
the content of 'template.php' provided by your theme.

Themeable functions that could be useful to overwrite:

- service_links_node_format($links, $label):
  
  Get the links to print into the node and return a themed string.

- service_links_block_format($items, $style)

  Get the items to print into a block (node and not-node pages)
  and return a themed string.

Last updated:
------------
