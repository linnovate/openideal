/**
 * ideaL chart javascript
 *
 */

// Load the Visualization API and the chart package.
google.load("visualization", "1", {packages:["corechart", "gauge"]});

(function($) {
  Drupal.behaviors.idealChart = {
    attach: function(context, settings) {
      // Set a callback to run when the Google 
      // Visualization API is loaded.
      google.setOnLoadCallback(drawChart);

      // Callback that creates and populates a data table, 
      // instantiates the chart, passes in the data and
      // draws it.
      function drawChart() {
        var row = new Array();
        // Create the data table.
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Label');
        
        // Adding the colomns. 
        // These are graphs titles.
        for (var col in settings.columns) {
          data.addColumn('number', settings.columns[col]);
        }

        // Adding the heders.
        // The rows titles.
        for (var i in settings.header) {
          var row = new Array();
          // Adding the rows.
          // The points of the column for each row.
          for (var j in settings.rows) {
             row[j] = parseInt(settings.rows[j][i]);
          } 
          row.unshift(settings.header[i]);
          data.addRows([row])
        };

        // Set chart options
        var options = settings.options;

        // Instantiate and draw our chart, passing in some options.
        var chart = new google.visualization[settings.type](document.getElementById(settings.html_id));
        chart.draw(data, options);
      }
    }
  };
})(jQuery);

