
Drupal.wysiwyg = Drupal.wysiwyg || { 'instances': {} };

Drupal.wysiwyg.editor = Drupal.wysiwyg.editor || { 'init': {}, 'attach': {}, 'detach': {}, 'instance': {} };

Drupal.wysiwyg.plugins = Drupal.wysiwyg.plugins || {};

(function ($) {
  // See if the current editor requires a global basepath variable
  // to be set before loading.
  if (Drupal.settings.wysiwyg) {
    $.each(Drupal.settings.wysiwyg.configs, function(editor_index, editor_value) {
      $.each(editor_value, function(format_index, format_value){
        if (format_value.global_basepath_var) {
          window[format_value.global_basepath_var] = Drupal.settings.wysiwyg.configs[editor_index].global.editorBasePath + '/';
        }
      });
    });
  }
  
  // Determine support for queryCommandEnabled().
  // An exception should be thrown for non-existing commands.
  // Safari and Chrome (WebKit based) return -1 instead.
  try {
    document.queryCommandEnabled('__wysiwygTestCommand');
    $.support.queryCommandEnabled = false;
  }
  catch (error) {
    $.support.queryCommandEnabled = true;
  }
})(jQuery);
