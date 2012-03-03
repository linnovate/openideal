
/**
 * jQuery bindings for gRaphael
 * ----------------------------
 *
 * Basic usage:
 *
 *   $('div.my-graph').graphael('bar', [0, 5, 10]);
 *
 * Parameters:
 *
 * @method
 *   String key, one of the following: 'bar', 'hbar', 'dot', 'pie', 'line' or 'get'.
 *   The 'get' method may only be used against a previously instantiated
 *   graph and will retrieve the gRaphael graph object. All other methods will
 *   create a corresponding gRaphael graph of the specified type.
 *
 * @values
 *   A non-associative array of values that correspond to the parameters of
 *   the specified graph type. For example, the 'bar' method expects only one
 *   array of values, while the 'line' method requires two arrays (x and y,
 *   respectively).
 *
 * @params
 *   An associative array of parameters. Supported parameters:
 *
 *   'opts':    Options that are passed directly to the gRaphael method.
 *   'padding': Pixels of padding to add to the inside of the gRaphael element.
 *              Defaults to 10. Set to 0 to draw the chart from edge to edge.
 *    'label': Label information for barcharts, separated into 'values' 
 *              (non-associative array) and 'isBottom' (flag to indicate if the 
 *              labels must be located below the bars).
 *
 * Events
 * ------
 *
 * @init.graphael
 *   Triggered when a gRaphael graph is first created. By binding to this
 *   event, the graph object can be retrieved and different hover, etc. effects
 *   can be added to a graph.
 *
 *   $('div.my-graph').bind('init.graphael', function() {
 *     $(this).graphael('get').hover(function() {}, function() {});
 *   });
 *
 * Static methods
 * --------------
 *
 * $().graphael.labelShow(graph, element, params);
 *
 * Chart element hover in callback for displaying a label.
 *
 * Parameters:
 *
 * @graph
 *   The jQuery object for the gRaphael DOM element.
 *
 * @element
 *   The element for which labeling should occur. `this` in the context of a
 *   chart.hover() event.
 *
 * @params
 *   An associative array of parameters. Supported parameters:
 *
 *   'label':      The label type to be used. May be one of the following:
 *                 'tag', 'popup', 'flag', 'label', 'drop', 'blob'.
 *   'attrLabel':  An associative array of attributes supported by gRaphael.
 *   'attrText':   An associative array of attributes supported by gRaphael.
 *
 *
 * $().graphael.labelHide(graph, element, params);
 *
 * Chart element hover out callback for hiding a label.
 *
 * Parameters:
 *
 * @graph
 *   The jQuery object for the gRaphael DOM element.
 *
 * @element
 *   The element for which labeling should occur. `this` in the context of a
 *   chart.hover() event.
 *
 * @params
 *   An associative array of parameters. Currently no supported parameters.
 *
 *
 * $().graphael.elementPosition(graph, element);
 *
 * Helper method to retrieve the position of an element in a graph value
 * series, starting from 0. Useful for retrieving external data by index of
 * the element in question.
 *
 * Parameters:
 *
 * @graph
 *   The jQuery object for the gRaphael DOM element.
 *
 * @element
 *   The element for which the index should be retrieved. `this` in the context
 *   of a chart.hover() event.
 *
 *
 * $().graphael.elementValue(graph, element);
 *
 * Helper method to retrieve the value of an element in a graph value series.
 *
 * Parameters:
 *
 * @graph
 *   The jQuery object for the gRaphael DOM element.
 *
 * @element
 *   The element for which the value should be retrieved. `this` in the context
 *   of a chart.hover() event.
 */
(function($) {
  $.fn.graphael = function(method, values, params) {
    var r = Raphael(this.get(0));
    var chart = false;
    params = params || {};
    params.opts = params.opts || {};
    params.padding = parseFloat(params.padding || "10");

    var w = this.width() - (params.padding * 2);
    var h = this.height() - (params.padding * 2);
    var x = params.padding;
    var y = params.padding;

    // Set supported parameters.

    // 'color': array of CSS hex color values, example: '#fff'.
    if (params.colors) {
      r.g.colors = params.colors;
    }
    // 'font': string of CSS font shorthand, example: '12px Arial'.
    if (params.font) {
      r.g.txtattr.font = params.font;
    }

    switch (method) {
      case 'bar':
        chart = r.g.barchart(x, y, w, h, values, params.opts);
        if (params.label) {
          chart.label(params.label.values, params.label.isBottom);
        }
        break;
      case 'hbar':
        chart = r.g.hbarchart(x, y, w, h, values, params.opts);
        if (params.label) {
          chart.label(params.label.values, params.label.isRight);
        }
        break;
      case 'dot':
        chart = r.g.dotchart(x, y, w, h, values[0], values[1], values[2], params.opts);
        break;
      case 'pie':
        var pie_x = (w * 0.5) + x;
        var pie_y = (h * 0.5) + y;
        var radius = w > h ? h * 0.5 : w * 0.5;

        // If a legend has been provided, attempt to position the pie chart
        // intelligently in relation to the legend position.
        if (params.opts.legend) {
          switch (params.opts.legendpos) {
            case 'north':
              pie_y = (h - radius) + y;
              break;
            case 'south':
              pie_y = (radius) + y;
              break;
            case 'west':
              pie_x = (w - radius) + x;
              break;
            // East is the default position.
            default:
              pie_x = (radius) + x;
              break;
          }
        }

        chart = r.g.piechart(pie_x, pie_y, radius, values, params.opts);
        break;
      case 'line':
        chart = r.g.linechart(x, y, w, h, values[0], values[1], params.opts);
        break;
    }
    if (chart) {
      this.data('method', method);
      this.data('raphael', r);
      this.data('chart', chart);
      this.trigger('init.graphael', {method: method, raphael: r, chart: chart});
    }
    return this;
  };

  $.fn.graphael.labelShow = function(graph, element, params) {
    var r          = graph.data('raphael');
    var method     = graph.data('method');
    var label      = params.label || 'popup';
    var attrLabel  = params.attrLabel || { fill:'#fff' };
    var attrText   = params.attrText || { fill:'#666', font: '12px Arial' };
    var value      = params.value || $.fn.graphael.elementValue(graph, element);

    switch (method) {
      case 'pie':
        element.label = r.g[label](element.mx, element.my, value).attr([attrLabel, attrText]);
        break;
      case 'bar':
      case 'hbar':
        element.label = r.g[label](element.bar.x, element.bar.y, value).attr([attrLabel, attrText]).insertBefore(element);
        break;
      case 'line':
        element.label = r.g[label](element.x, element.y, value).attr([attrLabel, attrText]).insertBefore(element);
        break;
      case 'dot':
        element.label = r.g[label](element.x, element.y, value).attr([attrLabel, attrText]).insertBefore(element);
        break;
    }
  };

  $.fn.graphael.labelHide = function(graph, element, params) {
    var method = graph.data('method');
    element.label.animate({opacity: 0}, 200, function () {this.remove();});
  };

  $.fn.graphael.elementPosition = function(graph, element) {
    var method = graph.data('method');
    switch (method) {
      case 'pie':
        return element.value.order;
      case 'bar':
      case 'hbar':
        for (var i in graph.data('chart').bars) {
          if (graph.data('chart').bars[i].id === element.bar.id) {
            return i;
          }
        }
        return false;
      case 'dot':
        for (var i in graph.data('chart').series) {
          if (graph.data('chart').series[i].id === element.dot.id) {
            return i;
          }
        }
        return false;
    }
    return false;
  };

  $.fn.graphael.elementValue = function(graph, element) {
    var method = graph.data('method');
    switch (method) {
      case 'pie':
        return element.value;
      case 'bar':
      case 'hbar':
        return element.bar.value;
      case 'line':
        return element.value;
      case 'dot':
        return element.value;
    }
    return false;
  };
})(jQuery);
