// $Id: xmlsitemap.js,v 1.1.2.2 2010/01/26 19:38:27 davereid Exp $

(function ($) {

Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.xmlsitemap = function() {
  var vals = [];

  // Inclusion select field.
  var status = $('#edit-xmlsitemap-status option:selected').text();
  vals.push(Drupal.t('Inclusion: @value', { '@value': status }));

  // Priority select field.
  var priority = $('#edit-xmlsitemap-priority option:selected').text();
  vals.push(Drupal.t('Priority: @value', { '@value': priority }));

  return vals.join('<br />');
};

})(jQuery);
