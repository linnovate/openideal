
/**
 * Custom theming for the popupsLoading.
 */
Drupal.theme.popupLoading = function() {
  var loading;
  loading += '<div id="popups-loading">';
  loading += '  <table>';
  loading += '    <tr>';
  loading += '      <td class="popups-tl popups-border"></td>';
  loading += '      <td class="popups-t popups-border"></td>';
  loading += '      <td class="popups-tr popups-border" id="popups-tr"></td>';
  loading += '    </tr>';
  loading += '    <tr>';
  loading += '      <td class="popups-cl popups-border"></td>';
  loading += '      <td class="popups-c">';
  loading += '        <span class="popups-loading-message">Loading...</span>';
  loading += '      </td>';
  loading += '      <td class="popups-cr popups-border"></td>';
  loading += '    </tr>';
  loading += '    <tr>';
  loading += '      <td class="popups-bl popups-border"></td>';
  loading += '      <td class="popups-b popups-border"></td>';
  loading += '      <td class="popups-br popups-border"></td>';
  loading += '    </tr>';
  loading += '  </table>';
  loading += '</div>';
  return loading;
};

Drupal.theme.popupTemplate = function (popupId) {
  var template;
  template += '<div id="' + popupId + '" class ="popups-box">';
  template += '  <table>';
  template += '    <tr>';
  template += '      <td class="popups-tl popups-border"></td>';
  template += '      <td class="popups-t popups-border"></td>';
  template += '      <td class="popups-tr popups-border"></td>';
  template += '    </tr>';
  template += '    <tr>';
  template += '      <td class="popups-cl popups-border"></td>';
  template += '      <td class="popups-c">';
  template += '        <div class="popups-container">';
  template += '          <div class="popups-title">';
  template += '            <div class="popups-close"><a href="#">' + Drupal.t('Close') + '</a></div>';
  template += '            <div class="title">%title</div>';
  template += '            <div class="clear-block"></div>';
  template += '          </div>';
  template += '          <div class="popups-body">%body</div>';
  template += '          <div class="popups-buttons">%buttons</div>';
  template += '          <div class="popups-footer"></div>';
  template += '        </div>';
  template += '      </td>';
  template += '      <td class="popups-cr popups-border"></td>';
  template += '    </tr>';
  template += '    <tr>';
  template += '      <td class="popups-bl popups-border"></td>';
  template += '      <td class="popups-b popups-border"></td>';
  template += '      <td class="popups-br popups-border"></td>';
  template += '    </tr>';
  template += '  </table>';
  template += '</div>';
  return template;
};

/**
 * We need to resize the popups-container as well as the popups if it scrolls
 */
Drupal.behaviors.resizePopupsContainer = function() {
  var popup = Popups.activePopup();
  if (popup) {
    var $popupsContainer = $('#' + popup.id + ' .popups-container');
    if ($popupsContainer.length) {
      var popupHeight = $popupsContainer.height();
      var windowHeight = $(window).height();
      if (popupHeight > (0.9 * windowHeight) ) { // Must fit in 90% of window.
        // we make this slightly smaller than popups so that it fits inside
        popupHeight = 0.85 * windowHeight;
        $popupsContainer.height(popupHeight);
      }
      // needs an extra 20px as the bottom dropshadow looks cutoff
      var $popup = popup.$popup();
      $popup.height($popup.height() + 20);
    }
  }
};