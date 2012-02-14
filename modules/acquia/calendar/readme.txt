Creates calendar displays of Views results.
 
Create a new calendar by enabling or cloning the default calendar,
changing the date argument to use the correct date field(s), and setting
up the year, month, day, week, and block views with the desired styles 
and fields.
 
Unlike previous versions of the Calendar module, there is just a single
Date argument instead of year, month, and day arguments. The argument
value will be YYYY-MM-DD for a day, YYYY-MM for a month, YYYY for a
year, and YYYY-W99 for a week. There is a default option to set the 
argument to the current date when the argument is empty.

A calendar display creates calendar navigation and links to 
multiple displays for the year, month, day, or week views. The actual
displays are created by attaching calendar views that use whatever
styles are desired for those pages. 
 
Calendar views are attachments to create the year, month, day,
and week displays. They can be set to use any style, either a
calendar style or any other Views style, like teasers or lists.
If you don't want to use one of them, don't attach it to
anything. Only the attached views will show up in the calendar.

A calendar block will create a calendar block for the
view results. Attach a block view to the block and set up the
desired style in the block view. 

If the Calendar iCal module is enabled, an iCal feed can be
attached to the view.



