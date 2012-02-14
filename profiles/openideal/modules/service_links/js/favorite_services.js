(function ($) {
  $(document).ready(function(){
    // work only for FF, IE, Opera, Chrome
    if (window.sidebar || window.external || window.opera) {
      $("a.service-links-favorite").show();
      if (window.chrome) {
        $("a.service-links-favorite").click(function(event){
          event.preventDefault();
          alert(Drupal.t('Use CTRL + D to add this to your bookmarks'));
        });
      } else if (window.opera && window.print) {
        $("a.service-links-favorite").each(function(){
          var url = $(this).attr('href').split('&favtitle=');
          var title = decodeURI(url[1]);
          url = url[0];

          $(this).attr('rel', 'sidebar').attr('href', url).attr('title', title);
        });
      } else if (window.sidebar || window.external.AddFavourite) {
        $("a.service-links-favorite").click(function(event){
          event.preventDefault();
          var url = $(this).attr('href').split('&favtitle=');
          var title = decodeURI(url[1]);
          url = url[0];

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
})(jQuery);
