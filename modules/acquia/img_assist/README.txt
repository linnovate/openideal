/* $Id: README.txt,v 1.15.2.5 2009/01/18 04:04:19 sun Exp $ */

-- SUMMARY --

Image Assist provides a user interface for adding images to any textarea that is
aware of input formats.

For a full description visit the project page:
  http://drupal.org/project/img_assist
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/img_assist


-- REQUIREMENTS --

* Image module <http://drupal.org/project/image>

* Views module <http://drupal.org/project/views>

* Token module (optional) <http://drupal.org/project/token>

* Wysiwyg API module (optional) <http://drupal.org/project/wysiwyg>


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure an input format and enable the filter 'Inline images' by visiting:

  Administer >> Site configuration >> Input formats

  If you want to enable Inline images for the input format 'Filtered HTML',
  you additionally need to click the 'Configure' tab and add

  <span> <img>

  to the text field 'Allowed HTML tags'.

* Configure user permissions in Administer >> User management >> Access control
  >> Image assist.

* Optionally go to the Views administration page and edit or clone Image
  Assist's default 'img_assist_browser' view for the image browser. If you
  intend to make major changes to the default view, you should click on 'Clone'
  and modify the new (duplicated) view instead.  When you are done, enable this
  view on Image Assist's settings page.

* Optionally fine tune how Image Assist operates by navigating to:

  Administer >> Site configuration >> Image assist

  If Taxonomy module is enabled, you use a gallery module like Acidfree, and you
  want your users to be able to easily choose images from their galleries, select
  for example "Acidfree albums" as the vocabulary to use for Image assist.

* If Wysiwyg API module is installed, you need to edit your Wysiwyg profiles
  and enable the plugin for Image assist.


-- USAGE --

* Using this module with TinyMCE via Wysiwyg API module:
  1. Click the camera icon on the TinyMCE toolbar.
  2. Upload a new photo or choose an existing image.
  3. Set the properties for how you want the image to display.
  4. Click the Insert button.

* Using this module with any textarea:
  1. Click the "Add image" link or camera icon under any textarea box. 
  2. Upload a new photo or choose an existing image.
  3. Set the properties for how you want the image to display.
  4. Click the Insert button.

* Most browsers, such as Internet Explorer and Mozilla clones, will insert the
  image exactly where you place your cursor in the textarea of your content
  form. Otherwise the image will be appended to the end of your entry.

* Adding images with Image module

  Users with the 'access img_assist' permission will see the 'add image' link
  or icon (configurable). Access to img_assist via the TinyMCE plugin is
  controlled by the Wysiwyg API module.  

  Users with the 'create images' permission will be able to upload images using
  img_assist. All users will be able to see and insert their own pictures, even
  if the image nodes are unpublished. Users with the 'access all images'
  permission will also be able to use other images, but only if they are
  published.

  One possible workflow would be to set images to be UNPUBLISHED by default.
  That way users can upload, categorize, and use images in img_assist without
  the images showing up anyway else on the site. Images that should  also be
  shown elsewhere on the site can manually be published by going to
  administer > content.

* Image presentation

  A full img_assist tag looks like this:

  [img_assist|nid=2|title=My title|desc=My description|align=right|width=200|height=150|link=url,http://www.google.com]


-- TROUBLESHOOTING --

* If your site is accessible from multiple domains, embedded image tags and
  links in contents will point to the domain/URL that was accessed upon first
  view of a content.  In most cases, you want to re-generate this cached content
  after staging from a development server to a live server.  For this purpose,
  go to Image Assist's settings page and click on the link (pointing to
  'img_assist/cache/clear') in the help text at the beginning of the page.

  To circumvent this permanently, as with any other module, you need to setup
  separate database tables for Drupal's cache.  Image Assist primarily works in
  the cache_filter table.  See sites/default/settings.php for more information.

* In front of submitting a new bug report, support or feature request, please
  search the issue queue for already existing issues.

  Note: FAILURE TO DO SO WILL EXTEND THE TIME FOR ISSUES TO BE SOLVED.

* If you are successfully using this module, please consider helping out on
  support requests in the issue queue.

* If you have a development site in progress, please test patches needing review.
  See http://drupal.org/patch/apply for further information.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) <http://www.unleashedmind.com>
* Hannes Lilljequist (zoo33) <http://www.perrito.se>
* Darren Oh (darrenoh) <http://darrenoh.dnsalias.com>

Previous maintainers:
* Benjamin Shell (benshell) <http://www.benjaminshell.com>
* Matt Westgate (matt westgate) <http://drupal.org/user/2275>

This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

