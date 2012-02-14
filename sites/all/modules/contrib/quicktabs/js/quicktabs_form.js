Drupal.quicktabsShowHide = function() {
  $(this).parents('tr').find('td.qt-tab-' + this.value + '-content').show().siblings('td.qt-tab-content').hide();
};

Drupal.behaviors.quicktabsform = function(context) {
  $('#quicktabs-form tr').not('.quicktabs-form-processed').addClass('quicktabs-form-processed').each(function(){
    var currentRow = $(this);
    currentRow.find('div.form-item :input[name*="type"]').bind('click', Drupal.quicktabsShowHide);
    $(':input[name*="type"]:checked', this).trigger('click');
  })
};