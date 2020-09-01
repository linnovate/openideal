(function ($, Drupal) {


  /**
   * Slideshow behaviour.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach Photoswipe and Swiper carousels to Slideshow block.
   */
  Drupal.behaviors.OpenidealSlideshowConfiguration = {
    attach: function (context, settings) {
      $('.pswp').once('openideal_slideshow_remove_class').removeClass('pswp-hidden');
      $('.openideal-slideshow', context).once('openideal_slideshow_configuration').each(function () {
        var isOneSlide = $('.swiper-slide', $(this)).length !== 1;
        // Swiper configuration.
        var mySwiper = new Swiper($(this).get(0), {
          loop: true,
          slidesPerView: 1,
          spaceBetween: 10,
          centeredSlides: true,
          allowSlidePrev: isOneSlide,
          allowSlideNext: isOneSlide,
          slideToClickedSlide: false,
          autoplay: {
            delay: 5000,
            disableOnInteraction: false
          },
          // keyboard control
          keyboard: {
            enabled: true,
          }
        });

        initPhotoSwipeFromDOM('.my-gallery', mySwiper);
      })

    }
  };

  /**
   * Init the photoswipe slider.
   *
   * @param {string} gallerySelector
   *   The index of the current element.
   * @param {object} mySwiper
   *   The slick HTML element.
   *
   * @see https://photoswipe.com/documentation/getting-started.html
   */
  var initPhotoSwipeFromDOM = function (gallerySelector, mySwiper) {

    /**
     * Parse slide data (url, title, size ...) from DOM elements
     *
     * @param {element} el
     *   The index of the current element.
     */
    var parseThumbnailElements = function (el) {
      var thumbElements = el.childNodes,
      numNodes = thumbElements.length,
      items = [],
      figureEl,
      linkEl,
      size,
      item;

      for (var i = 0; i < numNodes; i++) {
        figureEl = thumbElements[i]; // <figure> element

        // include only element nodes
        if (figureEl.nodeType !== 1) {
          continue;
        }

        linkEl = figureEl.children[0]; // <a> element

        size = linkEl.getAttribute("data-size").split("x");

        // create slide object
        item = {
          src: linkEl.getAttribute("href"),
          w: parseInt(size[0], 10),
          h: parseInt(size[1], 10)
        };

        if (figureEl.children.length > 1) {
          // <figcaption> content
          item.title = figureEl.children[1].innerHTML;
        }

        if (linkEl.children.length > 0) {
          // <img> thumbnail element, retrieving thumbnail url
          item.msrc = linkEl.children[0].getAttribute("src");
        }

        item.el = figureEl; // save link to element for getThumbBoundsFn
        items.push(item);
      }

      return items;
    };

    /**
     * Find nearest parent element
     *
     * @param {element} el
     *   The index of the current element.
     * @param {function} fn
     */
    var closest = function closest(el, fn) {
      return el && (fn(el) ? el : closest(el.parentNode, fn));
    };

    // triggers when user clicks on thumbnail
    /**
     * Triggers when user clicks on thumbnail
     *
     * @param {element} e
     */
    var onThumbnailsClick = function (e) {
      e = e || window.event;
      e.preventDefault ? e.preventDefault() : (e.returnValue = false);

      var eTarget = e.target || e.srcElement;

      // find root element of slide
      var clickedListItem = closest(eTarget, function (el) {
        return el.tagName && el.tagName.toUpperCase() === "LI";
      });

      if (!clickedListItem) {
        return;
      }

      // Find index of clicked item by looping through all child nodes
      // alternatively, you may define index via data- attribute
      var clickedGallery = clickedListItem.parentNode,
      childNodes = clickedListItem.parentNode.childNodes,
      numChildNodes = childNodes.length,
      nodeIndex = 0,
      index;

      for (var i = 0; i < numChildNodes; i++) {
        if (childNodes[i].nodeType !== 1) {
          continue;
        }

        if (childNodes[i] === clickedListItem) {
          index = nodeIndex;
          break;
        }
        nodeIndex++;
      }

      if (index >= 0) {
        // open PhotoSwipe if valid index found
        openPhotoSwipe(index, clickedGallery);
      }
      return false;
    };

    /**
     * Parse picture index and gallery index from URL (#&pid=1&gid=2)
     */
    var photoswipeParseHash = function () {
      var hash = window.location.hash.substring(1),
      params = {};

      if (hash.length < 5) {
        return params;
      }

      var vars = hash.split("&");
      for (var i = 0; i < vars.length; i++) {
        if (!vars[i]) {
          continue;
        }
        var pair = vars[i].split("=");
        if (pair.length < 2) {
          continue;
        }
        params[pair[0]] = pair[1];
      }

      if (params.gid) {
        params.gid = parseInt(params.gid, 10);
      }

      return params;
    };

    /**
     * Open photo swipe.
     *
     * @param {int} index
     *   The index of the current element.
     * @param {element} galleryElement
     *   The gallery node element.
     * @param {bool} disableAnimation
     *   Disable animation.
     * @param {bool} fromURL
     *   from Url?.
     */
    var openPhotoSwipe = function (
    index,
    galleryElement,
    disableAnimation,
    fromURL
    ) {
      var pswpElement = document.querySelectorAll(".pswp")[0],
      gallery,
      options,
      items;
      items = parseThumbnailElements(galleryElement);
      options = {
        closeEl: true,
        captionEl: true,
        fullscreenEl: true,
        zoomEl: true,
        shareEl: false,
        counterEl: false,
        arrowEl: true,
        preloaderEl: true,
        // define gallery index (for URL)
        galleryUID: galleryElement.getAttribute("data-pswp-uid"),
        getThumbBoundsFn: function (index) {
          // See Options -> getThumbBoundsFn section of documentation for more info
          var thumbnail = items[index].el.getElementsByTagName("img")[0], // find thumbnail
          pageYScroll = window.pageYOffset || document.documentElement.scrollTop,
          rect = thumbnail.getBoundingClientRect();

          return { x: rect.left, y: rect.top + pageYScroll, w: rect.width };
        }
      };

      // PhotoSwipe opened from URL
      if (fromURL) {
        if (options.galleryPIDs) {
          for (var j = 0; j < items.length; j++) {
            if (items[j].pid === index) {
              options.index = j;
              break;
            }
          }
        } else {
          // in URL indexes start from 1
          options.index = parseInt(index, 10) - 1;
        }
      } else {
        options.index = parseInt(index, 10);
      }

      // Exit if index not found.
      if (isNaN(options.index)) {
        return;
      }

      if (disableAnimation) {
        options.showAnimationDuration = 0;
      }

      // Pass data to PhotoSwipe and initialize it
      gallery = new PhotoSwipe(pswpElement, PhotoSwipeUI_Default, items, options);
      gallery.init();

      // photoswipe event: Gallery unbinds events
      // (triggers before closing animation)
      gallery.listen("unbindEvents", function () {
        // This is index of current photoswipe slide
        var getCurrentIndex = gallery.getCurrentIndex();
        // Update position of the slider
        mySwiper.slideTo(getCurrentIndex, false);
        // Start swiper autoplay (on close - if swiper autoplay is true)
        mySwiper.autoplay.start();
      });
      // Swiper autoplay stop when image zoom.
      gallery.listen('initialZoomIn', function () {
        if(mySwiper.autoplay.running){
          mySwiper.autoplay.stop();
        }
      });
    };

    // Loop through all gallery elements and bind events
    var galleryElements = document.querySelectorAll(gallerySelector);

    for (var i = 0, l = galleryElements.length; i < l; i++) {
      galleryElements[i].setAttribute("data-pswp-uid", i + 1);
      galleryElements[i].onclick = onThumbnailsClick;
    }

    // Parse URL and open gallery if it contains #&pid=3&gid=1
    var hashData = photoswipeParseHash();
    if (hashData.pid && hashData.gid) {
      openPhotoSwipe(hashData.pid, galleryElements[hashData.gid - 1], true, true);
    }
  };
}
)(jQuery, Drupal);
