// $Id: favorite_services.js,v 1.1.2.4 2010/10/04 16:11:15 thecrow Exp $
if (Drupal.jsEnabled) {
  $(document).ready(function(){
    // work only for FF, IE, Opera, Chrome
    if (window.sidebar || window.external || window.opera) {
      $("a.service-links-favorite").show();
      if (window.chrome) {
        $("a.service-links-favorite").click(function(event){
          event.preventDefault();
          alert(Drupal.t('Use CTRL + D for add this in your Bookmark'));
        });
      } else if (window.opera && window.print) {
        $("a.service-links-favorite").each(function(){
          var url = unescape($(this).attr('href').replace(/\+/g, ' '));
          var url = url.replace(/^[^\?]*\?/g, '');
          var title = url.replace(/^[^#]*#/g, '');
          url = url.replace(/#.*$/g, '');
          $(this).attr('rel', 'sidebar').attr('href', url).attr('title', title);
        });
      } else if (window.sidebar || window.external.AddFavourite) {
        $("a.service-links-favorite").click(function(event){
          event.preventDefault();
          var url = unescape($(this).attr('href').replace(/\+/g, ' '));
          var url = url.replace(/^[^\?]*\?/g, '');
          var title = url.replace(/^[^#]*#/g, '');
          url = url.replace(/#.*$/g, '');

          if (window.sidebar) {
            window.sidebar.addPanel(title, url, '');
          } else if (window.external) {
            window.external.AddFavorite(url, title);
          }
        });
      }
    } else {
      $("a.service-links-favorite").hide();
    }
  });
}
