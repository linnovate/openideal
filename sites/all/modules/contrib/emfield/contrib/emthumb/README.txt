// $Id: README.txt,v 1.1.2.1 2009/11/08 13:26:13 aaron Exp $

Embedded Media Thumbnail

This allows the server to store a local copy of a provider's thumbnail, for use
with ImageCache and faster retrieval of images for the browser. You can also
override a thumbnail with a custom upload by the editor.

Note that you must have the PHP variable allow_url_fopen set to ON for this
module to function, and it will not function in PHP Safe Mode.
