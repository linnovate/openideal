(function ($) {
  
/**
 * Behavior for initializing gRaphael graphs generated through the
 * theme('graphael') theme function in Drupal.
 */
Drupal.behaviors.graphael = {
  attach: function (context, settings) {
    // Allow others to bind to graphael DOM objects before the graph is inited
    // using the attach.drupal event.
    $(window).trigger('attach.drupal');

    $('.graphael-graph:not(.graphaelProcessed)').each(function() {
      $(this).addClass('graphaelProcessed');
      var id = $(this).attr('id');
      if (settings.graphael && settings.graphael[id]) {
        var d = settings.graphael[id];
        // Attach the original graph settings to the DOM element if they need
        // be used by any extending functionality.
        $(this)
          .data('settings', d)
          .graphael(d.method, d.values, d.params);
      }
    });
  }
};

$(window).bind('attach.drupal', function() {
  $('.graphael-graph:not(.graphaelBind)').bind('init.graphael', function(event, data) {
    var d = $(this).data('settings');
    if (d.extend) {
      // Extension: Basic labeling.
      if (d.extend.label) {
        var graph = $(this);
        var chart = data.chart;
        var params = d.extend.label.params || {};
        chart.hover(
          function() {
            var position = $().graphael.elementPosition(graph, this);
            if (position !== false && d.extend.label.values && d.extend.label.values[position]) {
              params.value = d.extend.label.values[position];
            }
            $().graphael.labelShow(graph, this, params);
          },
          function() { $().graphael.labelHide(graph, this, params); }
        );
      }
      // Extension: Clickable elements.
      if (d.extend.url) {
        var graph = $(this);
        var chart = data.chart;
        chart.each(function() {
          if (this.node) {
            var elem = this.node;
            $(elem).css({cursor:'pointer'});
          }
          else if (this.cover && this.cover.node) {
            var elem = this.cover.node;
            $(elem).css({cursor:'pointer'});
          }
        });
        chart.click(
          function() {
            var position = $().graphael.elementPosition(graph, this);
            if (position !== false && d.extend.url.values && d.extend.url.values[position]) {
              window.location = d.extend.url.values[position];
            }
          }
        );
      }
    }
  }).addClass('graphaelBind');
});

})(jQuery);
