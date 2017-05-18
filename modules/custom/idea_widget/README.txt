-- INSTALLATION --

1.put this directory under sites/all/modules
2.enable this module.


-- CONFIGURATION --

configure the fields that you want that appear in the widget in this url:
(your site)/admin/config/idea_widget.


-- EMBED THE WIDGET--

in the site that you want put the widget need write the script in two options:

1.<script type="text/javascript" src="{your domain}/sites/all/modules/idea_widget/idea_widget.js"></script>
this script append to parent wrapper html element the widget form.


2.<script type="text/javascript" src="{your domain}/sites/all/modules/idea_widget/idea_widget.js?popup=true"></script>

this script generate button that when click  it open popup that contain the widget .



should warp this script in html tag 
for example:
<div><script type="text/javascript" src="{your domain}/sites/all/modules/idea_widget/idea_widget.js"></script></div>






