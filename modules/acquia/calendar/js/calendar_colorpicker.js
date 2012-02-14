// $Id: calendar_colorpicker.js,v 1.1.4.3 2008/11/21 22:04:56 karens Exp $
/**
 * Implementation of hook_elements.
 * 
 * Much of the colorpicker code was adapted from the Colorpicker module.
 * That module has no stable release yet nor any D6 branch.
 */
/*
 *  Bind the colorpicker event to the form element
 */
Drupal.behaviors.calendarColorpicker = function (context) {
  
  // do we have multiple calendar_colors?
  if ($("div.calendar_colorpicker").size() > 0) {
  
    // loop over each calendar_color type
    $("div.calendar_colorpicker").each(function() {

      // create the farbtastic colorpicker
    var farb = $.farbtastic(this);
    
    // get the id of the current matched colorpicker wrapper div
    var id = $(this).attr("id");

    // get the calendar_color_textfields associated with this calendar_color
    $("input.calendar_colorfield").filter("." + id).each(function () {
      // set the background colors of all of the textfields appropriately
       farb.linkTo(this);
    
      // when clicked, they get linked to the farbtastic colorpicker that they are associated with
      $(this).click(function () {
        farb.linkTo(this);
      });

    });

    });
  }
};

