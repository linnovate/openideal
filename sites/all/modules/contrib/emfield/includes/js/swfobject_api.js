
// $Id: swfobject_api.js,v 1.1.2.1 2009/10/05 19:05:11 aaron Exp $

/**
 * This function looks for swfobject class items and loads them
 * as swfobjects.
 */
Drupal.behaviors.swfobjectInit = function (context) {
  $('.swfobject:not(.swfobjectInit-processed)', context).addClass('swfobjectInit-processed').each(function () {
    var config = Drupal.settings.swfobject_api['files'][$(this).attr('id')];
    swfobject.embedSWF(config.url, $(this).attr('id'), config.width, config.height, config.version, config.express_redirect, config.flashVars, config.params, config.attributes);
  });
};
