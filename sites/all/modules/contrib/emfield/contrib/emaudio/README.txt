/* $Id: README.txt,v 1.1.4.5 2008/06/04 12:08:03 alexua Exp $ */

/********************/
 Embedded Audio Field
/********************/

Author: Aaron Winborn
Development Began 2008-01-06

Requires: Drupal 5, Content (CCK)
Optional: Views

This extensible module will create a field for node content types that can be used to display audio music and podcasts
from various third party video providers. When entering the content, the user will simply paste the URL or embed code
of the audio, and the module will automatically determine which content provider is being used. When displaying
the audio, the proper embedding format will be used.

The module already supports podOmatic, Odeo, and Last.FM audio formats. More are planned to be supported soon. An api allows
other third party video providers to be supported using simple include files and provided hooks. (Developers: examine the
documentation of /providers/podomatic.inc for help in adding support for new providers).

The administer of a site may decide whether to allow all content providers, or only a certain number of them. They may
further be limited when configuring the field.

On the Display Fields settings page, the administrator may further choose how to display the audio, for teasers and body.
Any necessary API calls to third party providers are cached.

Other features available are allowing a podcast to autoplay, or changing the size of the player. Those features will be set
when creating or editing the specific field. Note that not all options are supported by all providers. You can see a list
of what features are currently supported by a provider at admin/content/emfield.

Some providers may provide other features that are supported by Embedded Audio Field, such as affiliate programs or related
podcasts. You can find those settings at admin/content/emfield, in the fieldset for the specific provider.

Questions can be directed to winborn at advomatic dot com
