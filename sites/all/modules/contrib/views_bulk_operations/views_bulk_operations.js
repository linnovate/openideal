// $Id: views_bulk_operations.js,v 1.1.4.19 2010/09/15 17:23:35 kratib Exp $
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};

Drupal.vbo.selectAll = function() {
  var table = this;
  var form = $(table).parents('form');

  var select = $('th.select-all', table).click(function() {
    setSelectAll(false);
  });
  $('input#vbo-select-all-pages', table).click(function() {
    setSelectAll(true);
  });
  $('input#vbo-select-this-page', table).click(function() {
    setSelectAll(false);
  });
  var checkboxes = $('td input:checkbox.vbo-select', form).click(function() {
    if (checkboxes.length > $(checkboxes).filter(':checked').length) {
      $('input#edit-objects-select-all', form).val(0);
    }
    setSelectAll($('input#edit-objects-select-all', form).val() == 1);
  }).each(function() {
    $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
  });

  var setSelectAll = function(all) {
    $('input#edit-objects-select-all', form).val(all ? 1 : 0);
    $('th.select-all input:checkbox', table).each(function() {
      if (this.checked) {
        $('td.view-field-select-all', table).css('display', $.browser.msie ? 'inline-block' : 'table-cell');
        $('span#vbo-this-page', table).css('display', all ? 'none' : 'inline');
        $('span#vbo-all-pages', table).css('display', all ? 'inline' : 'none');
      }
      else {
        $('td.view-field-select-all', table).css('display', 'none');
      }
    });
  }

  // Update UI based on initial values.
  if ($('input#edit-objects-select-all', form).val() == 1 || checkboxes.length == $(checkboxes).filter(':checked').length) {
    $('th.select-all input:checkbox', table).each(function() {
      this.checked = true;
    });
    setSelectAll($('input#edit-objects-select-all', form).val() == 1);
  }
}

Drupal.vbo.startUp = function(context) {
  // Reset the form action that Views Ajax might destroy.
  $('form[id^=views-bulk-operations-form]').each(function() {
    $(this).attr('action', Drupal.settings.basePath+Drupal.settings.vbo.url);
  });

  // Set up the VBO table for select-all functionality.
  $('form table:has(th.select-all)', context).each(this.selectAll);

  // Set up the ability to click anywhere on the row to select it.
  $('tr.rowclick', context).click(function(event) {
    if (event.target.nodeName.toLowerCase() != 'input' && event.target.nodeName.toLowerCase() != 'a') {
      $(':checkbox.vbo-select', this).each(function() {
        var checked = this.checked;
        // trigger() toggles the checkmark *after* the event is set, 
        // whereas manually clicking the checkbox toggles it *beforehand*.
        // that's why we manually set the checkmark first, then trigger the
        // event (so that listeners get notified), then re-set the checkmark
        // which the trigger will have toggled. yuck!
        this.checked = !checked;
        $(this).trigger('click');
        this.checked = !checked;
      });
    }
  });

  // Set up pager handling to store the current selection.
  $('ul.pager a', context).click(function() {
    var selection = [];
    $(':checkbox.vbo-select', context).filter(':checked').each(function() {
      selection.push($(this).val());
    });
    var select_all = $('#edit-objects-select-all', context).val();
    var page = location.href.match(/page=(\d+)/) || [0,0];
    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath+'views-bulk-operations/js/pager',
      data: 'url='+escape(Drupal.settings.vbo.url)+'&selection='+selection+'&select_all='+select_all+'&page='+page[1],
      datatype: 'json',
      async: false,
    });
    return true;
  });
}

Drupal.behaviors.vbo = function(context) {
  // Force Firefox to reload the page if Back is hit.
  // https://developer.mozilla.org/en/Using_Firefox_1.5_caching
  window.onunload = function(){}

  // Set up VBO UI.
  if (Drupal.settings.vbo) {
    Drupal.vbo.startUp(context);
  }
}

// END jQuery
})(jQuery);

