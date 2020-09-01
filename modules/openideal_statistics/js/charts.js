(function ($, Drupal) {

  'use strict';

  /**
   * Attach c3 entity by charts behavior.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach c3 entity by charts.
   */
  Drupal.behaviors.OpenidealStatisticsC3EntityByCharts = {
    attach: function (context, settings) {
      // @Todo: combine this and OpenidealStatisticsC3PerDayCharts behavior?
      let chartsSettings = settings.charts.byEntity;
      for (var id in chartsSettings) {
        if (chartsSettings.hasOwnProperty(id)) {
          $(chartsSettings[id].bindTo, context).once('openideal_statistics_c3_entity_by_charts').each(function () {
            c3.generate({
              bindto: $(this).get(0),
              padding: {
                left: 30,
                right: 30,
              },
              size: {
                height: 300,
                width: 500,
              },
              point: {
                show: false
              },
              data: {
                json: JSON.parse(chartsSettings[id].data),
                type: 'pie',
              },
            })
          })
        }
      }
    }
  }

  /**
   * Attach c3 per day charts behavior.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach c3 per day charts.
   */
  Drupal.behaviors.OpenidealStatisticsC3PerDayCharts = {
    attach: function (context, settings) {
      let chartsSettings = settings.charts.perDay;
      for (var id in chartsSettings) {
        if (chartsSettings.hasOwnProperty(id)) {
          $(chartsSettings[id].bindTo, context).once('openideal_statistics_c3_per_day_charts').each(function () {
            c3.generate({
              bindto: $(this).get(0),
              padding: {
                left: 30,
                right: 30,
              },
              size: {
                height: 300,
                width: 500,
              },
              point: {
                show: false
              },
              data: {
                json: JSON.parse(chartsSettings[id].data),
                keys: {
                  x: 'date',
                  value: ['total'],
                },
                names: {
                  'total': chartsSettings[id].label
                },
                type: 'spline',
              },
              grid: {
                y: {
                  show: true
                }
              },
              axis: {
                y: {
                  min: 0,
                  max: +chartsSettings[id].max + 1,
                  tick: {
                    format: function (d) {
                      return d % 1 === 0 ? d : '';
                    }
                  },
                },
                x: {
                  type: 'timeseries',
                  tick: {
                    count: 22,
                    rotate: -33,
                    format: function (x) {
                      return x.toLocaleDateString();
                    }
                  }
                }
              }
            })
          });
        }
      }
    }
  }

  /**
   * Attach charts behaviour.
   *
   * @see https://bl.ocks.org/syntagmatic/482706e0638c67836d94b20f0cb37122
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach d3 charts and created the parallel charts.
   */
  Drupal.behaviors.OpenidealStatisticsCharts = {
    attach: function (context, settings) {
      $('.charts').once('openideal_statistics_charts', context).each(function (name) {
        var devicePixelRatio = window.devicePixelRatio || 1;
        var margin = {top: 40, right: 70, bottom: 20, left: 50},
        width = 1200 - margin.left - margin.right,
        height = 420 - margin.top - margin.bottom,
        innerHeight = height - 2;

        // The colors are taken and applied for the first "column" of the chart.
        var color = d3.scaleOrdinal()
        .domain(['male', 'female', 'other'])
        .range(['#32889e', '#923fac', '#7f6464']);

        var types = {
          'String': {
            key: 'String',
            coerce: String,
            extent: function (data) {
              return data.sort();
            },
            within: function (d, extent, dim) {
              return extent[0] <= dim.scale(d) && dim.scale(d) <= extent[1];
            },
            defaultScale: d3.scalePoint().range([0, innerHeight])
          },
          'Date': {
            key: 'Date',
            coerce: function (d) {
              return new Date(d);
            },
            extent: d3.extent,
            within: function (d, extent, dim) {
              return extent[0] <= dim.scale(d) && dim.scale(d) <= extent[1];
            },
            defaultScale: d3.scaleTime().range([innerHeight, 0])
          },
          // Left the number for possible feature extending..
          'Number': {
            key: 'Number',
            coerce: function (d) {
              return +d;
            },
            extent: d3.extent,
            within: function (d, extent, dim) {
              return extent[0] <= dim.scale(d) && dim.scale(d) <= extent[1];
            },
            defaultScale: d3.scaleLinear().range([innerHeight, 0])
          },
        };

        // To add new dimensions into the charts, please add new array key to $data
        // @see Drupal\openideal_statistics\Plugin\Block\Charts
        // add into the dimensions array bellow, there are three types of data see
        // object "types" above.
        var dimensions = [
          {
            key: 'gender',
            description: 'Gender',
            type: types['String'],
            axis: d3.axisLeft()
            .tickFormat(function (d) {
              return d;
            })
          },
          {
            key: 'age',
            description: 'Age group',
            type: types['Date'],
            axis: d3.axisRight()
            .ticks(7)
            .tickFormat(function (d) {
              var date = new Date(d).getFullYear();
              switch (date) {
                case 1930:
                  date = Drupal.t('Before 1940');
                  break;

                case 2000:
                  date = Drupal.t('After 2000');
                  break;

                default:
                  date = String(date) + '-' + (date + 9);
              }
              return date;
            })
          },
        ];

        // Parse the Data.
        var data = JSON.parse(settings.charts.data);

        var xscale = d3.scalePoint()
        .domain(d3.range(dimensions.length))
        .range([0, width]);

        var yAxis = d3.axisLeft();

        var container = d3.select('.charts').append('div')
        .attr('class', 'parcoords')
        .style('width', width + margin.left + margin.right + 'px')
        .style('height', height + margin.top + margin.bottom + 'px');

        var svg = container.append('svg')
        .attr('width', width + margin.left + margin.right)
        .attr('height', height + margin.top + margin.bottom)
        .append('g')
        .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')');

        var canvas = container.append('canvas')
        .attr('width', width * devicePixelRatio)
        .attr('height', height * devicePixelRatio)
        .style('width', width + 'px')
        .style('height', height + 'px')
        .style('margin-top', margin.top + 'px')
        .style('margin-left', margin.left + 'px');

        var ctx = canvas.node().getContext('2d');
        ctx.globalCompositeOperation = 'darken';
        ctx.globalAlpha = 0.15;
        ctx.lineWidth = 1.5;
        ctx.scale(devicePixelRatio, devicePixelRatio);

        var axes = svg.selectAll('.axis')
        .data(dimensions)
        .enter().append('g')
        .attr('class', function (d) {
          return 'axis ' + d.key;
        })
        .attr('transform', function (d, i) {
          return 'translate(' + xscale(i) + ')';
        });

        data.forEach(function (d) {
          dimensions.forEach(function (p) {
            // In case if user didn't set one of field, and to not add unneeded empty "tick",
            // set default values.
            var getDefaultKey = function () {
              // Todo: check how it works with translation.
              switch (p.key) {
                case 'gender':
                  return 'other';

                case 'age':
                  return new Date("1960");

                default:
                  return null;
              }
            };

            // Change the actual data for 1939 year to 1930 because it's a
            // before 1940, and tics are every 10 years.
            if (p.key == 'age' && d[p.key] && d[p.key] == '1939') {
              return d[p.key] = p.type.coerce("1930");
            }

            d[p.key] = !d[p.key] ? getDefaultKey() : p.type.coerce(d[p.key]);
          });
        });

        // Type/dimension default setting happens here.
        dimensions.forEach(function (dim) {
          if (!('domain' in dim)) {
            // Detect domain using dimension type's extent function.
            dim.domain = d3_functor(dim.type.extent)(data.map(function (d) {
              return d[dim.key];
            }));
          }
          if (!('scale' in dim)) {
            // Use type's default scale for dimension.
            dim.scale = dim.type.defaultScale.copy();
          }

          // Change domain range from 1929 to 2000.
          if (dim.key === 'age') {
            dim.domain[0] = new Date("1929");
            dim.domain[1] = new Date("2000");
          }
          dim.scale.domain(dim.domain);
        });

        var render = renderQueue(draw).rate(30);

        ctx.clearRect(0, 0, width, height);
        ctx.globalAlpha = d3.min([1.15 / Math.pow(data.length, 0.3), 1]);
        render(data);

        axes.append('g')
        .each(function (d) {
          var renderAxis = 'axis' in d
          ? d.axis.scale(d.scale)  // custom axis
          : yAxis.scale(d.scale);  // default axis
          d3.select(this).call(renderAxis);
        })
        .append('text')
        .attr('class', 'title')
        .attr('text-anchor', 'start')
        .text(function (d) {
          return 'description' in d ? d.description : d.key;
        });

        // Add and store a brush for each axis.
        axes.append('g')
        .attr('class', 'brush')
        .each(function (d) {
          d3.select(this).call(d.brush = d3.brushY()
          .extent([[-10, 0], [10, height]])
          .on('start', brushStart)
          .on('brush', brush)
          .on('end', brush)
          )
        })
        .selectAll('rect')
        .attr('x', -8)
        .attr('width', 16);

        d3.selectAll('.axis.gender .tick text')
        .style('fill', color);

        /**
         * Map the dimensions axis.
         *
         * @param {array} d
         *   The "Data" cell information to map.
         *
         * @return {array}
         *   Mapped cell.
         */
        function project(d) {
          return dimensions.map(function (p, i) {
            // check if data element has property and contains a value
            if (
            !(p.key in d) ||
            d[p.key] === null
            ) {
              return null;
            }

            return [xscale(i), p.scale(d[p.key])];
          });
        }

        /**
         * Draw the graph lines.
         *
         * @param {array} d
         *   The "Data" cell information to draw.
         */
        function draw(d) {
          ctx.strokeStyle = color(d.gender);
          ctx.beginPath();
          var coords = project(d);
          coords.forEach(function (p, i) {
            // this tricky bit avoids rendering null values as 0
            if (p === null) {
              // this bit renders horizontal lines on the previous/next
              // dimensions, so that sandwiched null values are visible
              if (i > 0) {
                var prev = coords[i - 1];
                if (prev !== null) {
                  ctx.moveTo(prev[0], prev[1]);
                  ctx.lineTo(prev[0] + 6, prev[1]);
                }
              }
              if (i < coords.length - 1) {
                var next = coords[i + 1];
                if (next !== null) {
                  ctx.moveTo(next[0] - 6, next[1]);
                }
              }
              return;
            }

            if (i == 0) {
              ctx.moveTo(p[0], p[1]);
              return;
            }

            ctx.lineTo(p[0], p[1]);
          });
          ctx.stroke();
        }

        /**
         * Start the brash.
         */
        function brushStart() {
          d3.event.sourceEvent.stopPropagation();
        }

        /**
         * Handles a brush event, toggling the display of foreground lines.
         */
        function brush() {
          render.invalidate();

          var actives = [];
          svg.selectAll('.axis .brush')
          .filter(function (d) {
            return d3.brushSelection(this);
          })
          .each(function (d) {
            actives.push({
              dimension: d,
              extent: d3.brushSelection(this)
            });
          });

          var selected = data.filter(function (d) {
            if (actives.every(function (active) {
              var dim = active.dimension;
              // test if point is within extents for each active brush
              return dim.type.within(d[dim.key], active.extent, dim);
            })) {
              return true;
            }
          });

          svg.selectAll('.axis')
          .filter(function (d) {
            return actives.indexOf(d) > -1 ? true : false;
          })
          .classed('active', true)
          .each(function (dimension, i) {
            var extent = extents[i];
            d3.select(this)
            .selectAll('.tick text')
            .style('display', function (d) {
              var value = dimension.type.coerce(d);
              return dimension.type.within(value, extent, dimension) ? null : 'none';
            });
          });

          // reset dimensions without active brushes
          svg.selectAll('.axis')
          .filter(function (d) {
            return actives.indexOf(d) > -1 ? false : true;
          })
          .classed('active', false)
          .selectAll('.tick text')
          .style('display', null);

          ctx.clearRect(0, 0, width, height);
          ctx.globalAlpha = d3.min([0.85 / Math.pow(selected.length, 0.3), 1]);
          render(selected);

        }

        /**
         * Resolve the function.
         *
         * @param {object} v
         *   The function to resolve.
         * @return {*|(function(): *)}
         */
        function d3_functor(v) {
          return typeof v === 'function' ? v : function () {
            return v;
          };
        }

        /**
         * Prepare render queue function.
         *
         * @param {object} func
         *   The function that render the graph.
         *
         * @return {rq}
         *   Function to render queue.
         */
        function renderQueue(func) {
          var _queue = [],                  // data to be rendered
          _rate = 1000,                 // number of calls per frame
          _invalidate = function () {
          },  // invalidate last render queue
          _clear = function () {
          };       // clearing function

          var rq = function (data) {
            if (data) {
              rq.data(data);
            }
            _invalidate();
            _clear();
            rq.render();
          };

          rq.render = function () {
            var valid = true;
            _invalidate = rq.invalidate = function () {
              valid = false;
            };

            function doFrame() {
              if (!valid) {
                return true;
              }
              var chunk = _queue.splice(0, _rate);
              chunk.map(func);
              timer_frame(doFrame);
            }

            doFrame();
          };

          rq.data = function (data) {
            _invalidate();
            _queue = data.slice(0);   // creates a copy of the data
            return rq;
          };

          rq.add = function (data) {
            _queue = _queue.concat(data);
          };

          rq.rate = function (value) {
            if (!arguments.length) {
              return _rate;
            }
            _rate = value;
            return rq;
          };

          rq.remaining = function () {
            return _queue.length;
          };

          // clear the canvas
          rq.clear = function (func) {
            if (!arguments.length) {
              _clear();
              return rq;
            }
            _clear = func;
            return rq;
          };

          rq.invalidate = _invalidate;

          var timer_frame = window.requestAnimationFrame
          || window.webkitRequestAnimationFrame
          || window.mozRequestAnimationFrame
          || window.oRequestAnimationFrame
          || window.msRequestAnimationFrame
          || function (callback) {
            setTimeout(callback, 17);
          };

          return rq;
        }
      });
    }
  };
}
)(jQuery, Drupal);
