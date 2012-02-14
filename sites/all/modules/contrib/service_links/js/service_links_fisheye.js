// $Id: service_links_fisheye.js,v 1.1.2.2 2010/10/04 16:11:15 thecrow Exp $
if (Drupal.jsEnabled) {
  $(document).ready(function(){
    $('#fisheye').Fisheye({
      maxWidth: 32,
      items: 'a',
      itemsText: 'span',
      container: '.fisheyeContainer',
      itemWidth: 16,
      proximity: 60,
      halign : 'center'
    })
  });
}
