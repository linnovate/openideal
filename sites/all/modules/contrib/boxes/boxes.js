// $Id: boxes.js,v 1.2.2.4 2010/08/05 20:16:28 yhahn Exp $

Drupal.behaviors.boxes = function(context) {
  Drupal.CTools.AJAX.commands.getBlock = function(data) {
    $.ajax({
      type: "GET",
      url: data.url,
      data: { 'boxes_delta': data.delta },
      global: true,
      success: Drupal.CTools.AJAX.respond,
      error: function(xhr) {
        Drupal.CTools.AJAX.handleErrors(xhr, url);
      },
      dataType: 'json'
    });
  };
  $('div.boxes-box-controls a:not(.boxes-processed)')
    .addClass('boxes-processed')
    .click(function() {
      var box = $(this).parents('.boxes-box');
      if (box.is('.boxes-box-editing')) {
        box.removeClass('boxes-box-editing').find('.box-editor').remove().end().find('.boxes-box-content').show();
      }
      else {
        // Show editing form - the form itself gets loaded via CTools ajax..
        box.find('.boxes-box-content').hide().end().addClass('boxes-box-editing').append('<div class="box-editor"><div class="swirly"></div></div>');
      }
      return false;
    });
  $('.boxes-ajax').click(function() {
      if ($(this).hasClass('boxes-ajaxing')) {
        return false;
      }
      // Put our button in.
      this.form.clk = this;
      var object = $(this), form = this.form, url = $(form).attr('action');
      $(this).addClass('boxes-ajaxing').parents('.box-editor').html('<div class="swirly"></div>').end();
      $(form).ajaxSubmit({
        type: "POST",
        url: url,
        data: { 'js': 1, 'ctools_ajax': 1 },
        global: true,
        success: Drupal.CTools.AJAX.respond,
        error: function(xhr) {
          Drupal.CTools.AJAX.handleErrors(xhr, url);
        },
        complete: function() {
          object.removeClass('boxes-ajaxing');
        },
        dataType: 'json'
      });
      return false;
  });

  Drupal.CTools.AJAX.commands.preReplaceContextBlock = function(data) {
    Drupal.settings.boxes = Drupal.settings.boxes || {};
    var e = $('#' + data.id + ' a.context-block:first').clone();
    Drupal.settings.boxes[data.id] =  e;
  };

  Drupal.CTools.AJAX.commands.postReplaceContextBlock = function(data) {
    $('#' + data.id).append(Drupal.settings.boxes[data.id]);
    $('form.context-editor.context-editing').each(function() {
      var id = $(this).attr('id');
      if (Drupal.contextBlockEditor[id]) {
        Drupal.contextBlockEditor[id].initBlocks($('#' + data.id));
      }
    });
  };
};

