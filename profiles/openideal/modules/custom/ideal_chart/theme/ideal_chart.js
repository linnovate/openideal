/**
 * ideaL chart javascript
 *
 */

// Load the Visualization API and the chart package.
google.load("visualization", "1", {packages:["corechart"]});

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
        data.addColumn('string', 'Post Date');
        data.addColumn('number', 'No. Of Ideas');
        data.addColumn('number', 'No. Of Comments');
        data.addColumn('number', 'No. Of No. Of Votes');
        for (var i in settings.header) {
          var row = new Array();
          for (var j in settings.rows) {
             row[j] = parseInt(settings.rows[j][i]);
          } 
          row.unshift(settings.header[i]);
          data.addRows([row])
        };

        // Set chart options
        var options = {
          'title':'Ideas per day',
          'width':800,
          'height':300
        };

        // Instantiate and draw our chart, passing in some options.
        var chart = new google.visualization.LineChart(document.getElementById('content'));
        chart.draw(data, options);
      }
    }
  };
})(jQuery);

