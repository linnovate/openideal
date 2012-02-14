// $Id: editor_plugin.js,v 1.3.4.5 2008/12/27 12:42:53 sun Exp $

(function() {
  // Load plugin specific language pack.
  tinymce.PluginManager.requireLangPack('img_assist');

  tinymce.create('tinymce.plugins.ImageAssistPlugin', {
    /**
     * Initialize the plugin, executed after the plugin has been created.
     *
     * This call is done before the editor instance has finished it's
     * initialization so use the onInit event of the editor instance to
     * intercept that event.
     *
     * @param ed
     *   The tinymce.Editor instance the plugin is initialized in.
     * @param url
     *   The absolute URL of the plugin location.
     */
    init : function(ed, url) {
      // Register the ImageAssist execCommand.
      ed.addCommand('ImageAssist', function() {
        // captionTitle and captionDesc for backwards compatibility.
        var data = {nid: '', title: '', captionTitle: '', desc: '', captionDesc: '', link: '', url: '', align: '', width: '', height: '', id: ed.id, action: 'insert'};
        var node = ed.selection.getNode();
        if (node.name == 'mceItemDrupalImage') {
          data.width = node.width;
          data.height = node.height;
          data.align = node.align;
          // Expand inline tag in alt attribute
          node.alt = decodeURIComponent(node.alt);
          var chunks = node.alt.split('|');
          for (var i in chunks) {
            chunks[i].replace(/([^=]+)=(.*)/g, function(o, property, value) {
              data[property] = value;
            });
          }
          data.captionTitle = data.title;
          data.captionDesc = data.desc;
          data.action = 'update';
        }
        
        ed.windowManager.open({
          file : Drupal.settings.basePath + 'index.php?q=img_assist/load/tinymce&textarea=' + ed.id,
          width : 700 + parseInt(ed.getLang('img_assist.delta_width', 0)),
          height : 500 + parseInt(ed.getLang('img_assist.delta_height', 0)),
          inline : 1
        }, data);
      });

      // Register Image Assist button.
      ed.addButton('img_assist', {
        title : 'img_assist.desc',
        cmd : 'ImageAssist',
        image : url + '/images/drupalimage.gif'
      });

      // Load Image Assist's CSS for editor contents on startup.
      ed.onInit.add(function() {
        if (!ed.settings.drupalimage_skip_plugin_css) {
          ed.dom.loadCSS(url + "/css/img_assist.css");
        }
      });

      // Replace images with inline tags in editor contents upon data.save.
      // @todo Escape regular | pipes.
      ed.onBeforeGetContent.add(function(ed, data) {
        if (!data.save) {
          return;
        }
        jQuery.each(ed.dom.select('img', data.content), function(node) {
          if (this.name != 'mceItemDrupalImage') {
            return;
          }
          var inlineTag = '[img_assist|' + decodeURIComponent(this.alt) + '|align=' + this.align + '|width=' + this.width + '|height=' + this.height + ']';
          ed.dom.setOuterHTML(this, inlineTag);
        });
      });

      // Replace inline tags in data.content with images.
      ed.onBeforeSetContent.add(function(ed, data) {
        data.content = data.content.replace(/\[img_assist\|([^\[\]]+)\]/g, function(orig, match) {
          var node = {}, chunks = match.split('|');
          for (var i in chunks) {
            chunks[i].replace(/([^=]+)=(.*)/g, function(o, property, value) {
              node[property] = value;
            });
          }
          node.name = 'mceItemDrupalImage';
          node.src = Drupal.settings.basePath + 'index.php?q=image/view/' + node.nid;
          node.alt = 'nid=' + node.nid + '|title=' + node.title + '|desc=' + node.desc;
          if (node.link.indexOf(',') != -1) {
            var link = node.link.split(',', 2);
            node.alt += '|link=' + link[0] + '|url=' + link[1];
          }
          else {
            node.alt += '|link=' + node.link;
          }
          if (typeof node.url != 'undefined') {
            node.alt += '|url=' + node.url;
          }
          node.alt = encodeURIComponent(node.alt);
          return ed.dom.createHTML('img', node);
        });
        return;
      });

      // Add a node change handler, selects the button in the UI when an image is selected.
      ed.onNodeChange.add(function(ed, command, node) {
        command.setActive('img_assist', node.nodeName == 'IMG' && ed.dom.getAttrib(node, 'name').indexOf('mceItemDrupalImage') != -1);
      });
    },

    /**
     * Return information about the plugin as a name/value array.
     */
    getInfo : function() {
      return {
        longname : 'Image Assist',
        author : 'Daniel F. Kudwien',
        authorurl : 'http://www.unleashedmind.com',
        infourl : 'http://drupal.org/project/img_assist',
        version : "2.0"
      };
    }
  });

  // Register plugin.
  tinymce.PluginManager.add('img_assist', tinymce.plugins.ImageAssistPlugin);
})();
