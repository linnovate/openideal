/* $Id: README.txt,v 1.1.2.1 2008/07/18 20:22:44 alexua Exp $ */

/********************/
 Embedded Image Field
/********************/

Author: Aaron Winborn
Development Began 2007-06-13

Requires: Drupal 5, Content (CCK), Embedded Media Field
Optional: Views

This extensible module will create a field for node content types that can be used to display images from various third party
image providers. When entering the content, the user will simply paste the URL or embed code of the image, and the module will
automatically determine which content provider is being used. When displaying the image, the proper embedding format will be
used, calling any necessary API's.

The module already supports Flicker, Imageshack, and Photobucket image formats. More are planned to be supported soon. An api allows other
third party providers to be supported using simple include files and provided hooks. (Developers: examine the documentation of
/providers/flikr.inc for help in adding support for new providers).

The administer of a site may decide whether to allow all content providers, or only a certain number of them. They may
further be limited when configuring the field.

On the Display Fields settings page, the administrator may further choose how to display the image, for teasers and body.
Images may be displayed in a thumbnail, preview, or full size, each of configurable sizes. Unfortunately, for now, because
images are not stored locally, we don't have access to the powerful features of imagecache.

Some providers may provide other features that are supported by Image Neighborhood CCK. You can find those settings at
admin/content/emfield, in the fieldset for the specific provider.

Questions can be directed to winborn at advomatic dot com
