<?php

/**
 * @file
 * Hooks provided by the Video Embed Field module.
 */

/**
 * Define a video handler that decodes a video provider URL into a video player or thumbnail
 * 
 * You can define a handler like this:
 *
 * @code
 *   $handler['vimeo'] = array(
 *     'title' => 'Vimeo',
 *     'function' => 'vimeo_handler',
 *     'thumbnail_function' => 'vimeo_thumbnail',
 *     'form' => 'vimeo_form',
 *     'domains' => array(
 *       'vimeo.com'
 *     ),
 *     'defaults' => array(
 *        'width' => 640,
 *        'height' => 360',
 *     ),
 *   );
 * @endcode
 *
 * @return
 *   Embed handlers:
 *   - "title": The name of the handler - will be wrapped with t().
 *   - "function": Function to return embed code from a URL and video style array.
 *   - "thumbnail_function": Function to return a video frame image.
 *   - "form": Function to create a settings form (optional).
 *   - "domains": Array of domains this handler will created embed codes for
 *   - "defaults": Array of default settings.
 */
function hook_video_embed_handler_info() {

}