// $Id: views_bulk_operations.action.js,v 1.1.2.5 2010/06/07 14:50:10 kratib Exp $
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};
Drupal.vbo.action = Drupal.vbo.action || {};

Drupal.vbo.action.updateOperations = function(vid, trigger) {
  var options = "";
  if (Drupal.settings.vbo.action.views_operations[vid] == undefined) {
    options += "<option value=\"0\">" + Drupal.t("- No operation found in this view -") + "</option>";
  }
  else {
    options += "<option value=\"0\">" + Drupal.t("- Choose an operation -") + "</option>";
    $.each(Drupal.settings.vbo.action.views_operations[vid], function(value, text) {
      options += "<option value=\"" + value + "\">" + text + "</option>\n";
    });
  }
  operation = $("#edit-operation-callback").val();
  $("#edit-operation-callback").html(options).val(operation);
  if (trigger) {
    $("#edit-operation-callback").trigger('change');
  }
}

Drupal.behaviors.vbo_action = function(context) {
  vid = $("#edit-view-vid").val();
  Drupal.vbo.action.updateOperations(vid, false);
}

// END jQuery
})(jQuery);

