/**
 * @file
 * Global utilities.
 *
 */
(function ($, Drupal) {

  'use strict';

  /**
   * Default bootstrap barrio subtheme behaviour.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Add scroll event to body.
   */
  Drupal.behaviors.bootstrap_barrio_subtheme = {
    attach: function (context, settings) {
      var position = $(window).scrollTop();
      $(window).scroll(function () {
        var $body = $('body');
        if ($(this).scrollTop() > 50) {
          $body.addClass("scrolled");
        } else {
          $body.removeClass("scrolled");
        }
        var scroll = $(window).scrollTop();
        if (scroll > position) {
          $body.addClass("scrolldown");
          $body.removeClass("scrollup");
        } else {
          $body.addClass("scrollup");
          $body.removeClass("scrolldown");
        }
        position = scroll;
      });

    }
  };

  /**
   * Teaser mod behaviors.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach behaviors that react if teaser has no images.
   */
  Drupal.behaviors.openidealThemeTeaser = {
    attach: function (context, settings) {
      $('.teaser-top-section--section__first').once('openideal_theme_teaser').each(function () {
        var $this = $(this);
        if (!$this.has('.block-openidel-slideshow-block').length) {
          $this.addClass('teaser-top-section--without-slideshow')
        }
      });
    }
  };

  /**
   * Custom select behaviors.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attach behaviors to views created select.
   */
  Drupal.behaviors.openidealCustomSelectOption = {
    attach: function (context, settings) {
      $('.custom-sort', context).once('openideal_custom_select_option').each(function () {
        // Hide custom selection on window click.
        $(window).on('click', function (e) {
          var $options = $('.custom-sort--options');
          if (!e.target.matches('.custom-sort--button') && $options.is(':visible')) {
            $options.hide('400');
          }
        });

        // Show custom selection.
        $('.custom-sort--button' ,this).on('click', function () {
          $('.custom-sort--options').toggle('show');
        });

        // Trigger real selection option.
        $('.custom-sort--option' ,this).each(function () {
          $(this).on('click', function () {
            $('.form-item-sort-bef-combine select').val($(this).data('option-id')).change()
          });
        });
      });
    }
  };

  /**
   * Main navigation behaviour.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Toggle the main navigation.
   */
  Drupal.behaviors.openidealThemeNavigationToggle = {
    attach: function (context, settings) {
      $('.site-navigation--dismiss, .navigation-overlay, .site-sidebar--dismiss').once('openideal-theme-navigation-toggle-overlay').on('click', function () {
        // hide sidebar
        $('#site-navigation, .site-sidebar').removeClass('active');
        // hide overlay
        $('.navigation-overlay').removeClass('active');
      });

      $('#sidebar-collapse').once('openideal-theme-navigation-toggle').on('click', function () {
        // open sidebar
        $('#site-navigation').addClass('active');
        // fade in the overlay
        $('.navigation-overlay').addClass('active');
      });

      $('#site-notification-collapse').once('openideal-theme-navigation-toggle').on('click', function () {
        // open sidebar
        $('.site-sidebar').addClass('active');
        // fade in the overlay
        $('.navigation-overlay').addClass('active');
      });
    }
  }

  /**
   * Comments form behaviour.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Hide/show comments add form.
   */
  Drupal.behaviors.openidealThemeCommentsFormAnimation = {
    attach: function (context, settings) {
      $('.comments--header__add-comment-btn, .site-footer-open-comments-btn', context).once('openideal-theme-comments-form-animation').on('click', function () {
        var commentsBottom = $('.comments--bottom');
        // hide sidebar
        commentsBottom.toggle('slow');

        // Scroll to comments body.
        $([document.documentElement, document.body]).animate({
          scrollTop: commentsBottom.offset().top
        }, 1000);
      });

      $('.comment-form--cancel-btn', context).once('openideal-theme-comments-form-animation-reply').on('click', function () {
        // hide sidebar
        var $form = $(this).closest('form');
        if ($form.hasClass('ajax-comments-form-reply')) {
          $form.toggle('slow');
        } else {
          $('.comments--bottom').toggle('slow');
        }
      });

      $('.ajax-comments-form-edit .comment-form--cancel-btn').once('openideal-theme-comments-form-animation-edit').addClass('d-none');
    }
  }

  /**
   * Change votingapi_reaction like label.
   * @Todo: Do the logic in the backend.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Change label.
   */
  Drupal.behaviors.openidealThemeLikeWidgetLabel = {
    attach: function (context, settings) {
      $('.votingapi-reaction-form').once('openideal_theme_like_widget_label').each(function () {
        var $this = $(this);
        var $item = $('.votingapi-reaction-item', $this);
        var $label = $('.votingapi-reaction-label', $this);
        if ($('.radio input', $this).is(':checked')) {
          $item.addClass('active');
          $label.text(Drupal.t('Liked'));
        }
        else {
          if ($label.parents('.challenge-voting').length) {
            $label.text(Drupal.t('Like challenge'));
          } else if ($label.parents('.region-sidebar').length) {
            $label.text(Drupal.t('Like idea'));
          } else if ($label.parents('.site-footer').length) {
            $label.text(Drupal.t('Like'));
          }
        }
      });
    }
  }

  /**
   * Add logic to copy url button.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Implement ability to copy current url into clipboard.
   */
  Drupal.behaviors.openidealThemeCopyUrlButton = {
    attach: function (context, settings) {
      $('.share-buttons--buttons__copy_url', context).once('openideal_theme_copy_url_button').each(function () {
        var $this = $(this);
        // Enable tooltip for element.
        $this.tooltip();

        $this.on('click', function () {
          $(this).tooltip()
          var $temp = $('<input>'),
          url = window.location.href;
          $('body').append($temp);
          $temp.val(url).select();
          document.execCommand('copy');
          $temp.remove();
          $this.tooltip('hide')
          .attr('data-original-title', 'Copied!')
          .tooltip('show');
        });
      })
    }
  }

  /**
   * Attach behaviours on mobile share block.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Implement ability to show/hide mobile share block.
   */
  Drupal.behaviors.openidealThemeMobileShareBlock = {
    attach: function (context, settings) {
      $('.site-footer-open-share-btn', context).once('openideal_theme_mobile_share_block').on('click', function () {
        $('.mobile-share-footer').toggle(400);
      })
    }
  }

  /**
   * Attach behaviours on exposed filter in ideas page.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Add the label before active option in every select.
   */
  Drupal.behaviors.openidealThemeExposedIdeasFilter = {
    attach: function (context, settings) {
      $('.teaser-view-mode .views-exposed-form fieldset, .view--user-admin-people--community-page .views-exposed-form fieldset', context).once('openideal_theme_exposed_ideas_filter').each(function () {
        var $this = $(this);
        var text = $('label', $this).text();

        if ($this.hasClass('form-item-search')) {
          $this.prepend('<div class="search-submit-button"></div>');
          $('.search-submit-button', $this).on('click', function () {
            $('.views-exposed-form input.form-submit', context).trigger('click');
          });
        } else {
          $('select option:selected', $this).prepend('<span class="select-prepend">' + text + ': ' + '</span>')
        }
      });
    }
  }

  /**
   * Attach behaviours on user profile page.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Add colon aftet a label.
   */
  Drupal.behaviors.openidealThemeUserProfile = {
    attach: function (context, settings) {
      $('.user-profile-section-top .user-profile-section-top--section__second .field__label').once('openideal_theme_user_profile').each(function () {
        var $this = $(this);
        var text = $this.text();
        if (text.charAt(text.length - 1) !== ':') {
          $this.text(text + ':');
        }
      });
    }
  }

  /**
   * Add the behaviour to comment reply link.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Add the count of replies in from of reply link,
   *   and logic to hide/show replies.
   */
  Drupal.behaviors.openidealThemeCommentsReply = {
    attach: function (context, settings) {
      $('.comments--thread', context).once('openideal_theme_comments_last_child').each(function () {
        var $this = $(this);

        var $comments = $this.find('.single-comment').toArray();

        for (var $i = $comments.length - 1; $i >= 0; $i--) {
          // If the comment has not children then don't need to show border.
          var $current = $($comments[$i]);
          if ($current.is(':visible')) {
            $($current).addClass('comments--thread__border-none')
            break;
          } else {
            $current.addClass('comments--thread__border-none')
          }
        }
      });

      $('.comment-show .single-comment--open-replies', context).once('openideal_theme_comments_reply').each(function () {
        var $this = $(this);
        var $currentComment = $this.closest('.single-comment');
        var main = $currentComment.siblings('.indented');
        var replies = 0;
        if (main.length > 0) {
          replies = main.find('.single-comment').length
        }
        $this.after('<span>' + replies + Drupal.t(' replies') + '</span>');

        if (replies > 0) {
          $this.closest('.comment-show').on('click', function () {
            main.toggle('slow');
            $currentComment.toggleClass('comments--thread__border-none');
          });
        }
      })
    }
  }

}
)(jQuery, Drupal);
