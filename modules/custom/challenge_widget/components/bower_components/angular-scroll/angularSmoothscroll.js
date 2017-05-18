'use strict';

/*
A simple AngularJS directive to render a smooth scroll effect
Usage: <element smooth-scroll target='id' [offset='value']></element>
@author: Arnaud BRETON (arnaud@videonot.es)
Inspired by http://www.itnewb.com/tutorial/Creating-the-Smooth-Scroll-Effect-with-JavaScript
*/

angular.module('angularSmoothscroll', []).directive('smoothScroll', [
  '$log', '$timeout', '$window', function($log, $timeout, $window) {
    /*
        Retrieve the current vertical position
        @returns Current vertical position
    */

    var currentYPosition, elmYPosition, smoothScroll;
    currentYPosition = function() {
      if ($window.pageYOffset) {
        return $window.pageYOffset;
      }
      if ($window.document.documentElement && $window.document.documentElement.scrollTop) {
        return $window.document.documentElement.scrollTop;
      }
      if ($window.document.body.scrollTop) {
        return $window.document.body.scrollTop;
      }
      return 0;
    };
    /*
        Get the vertical position of a DOM element
        @param eID The DOM element id
        @returns The vertical position of element with id eID
    */

    elmYPosition = function(eID) {
      var elm, node, y;
      elm = document.getElementById(eID);
      if (elm) {
        y = elm.offsetTop;
        node = elm;
        while (node.offsetParent && node.offsetParent !== document.body) {
          node = node.offsetParent;
          y += node.offsetTop;
        }
        return y;
      }
      return 0;
    };
    /*
        Smooth scroll to element with a specific ID without offset
        @param eID The element id to scroll to
        @param offSet Scrolling offset
    */

    smoothScroll = function(eID, offSet) {
      var distance, i, leapY, speed, startY, step, stopY, timer, _results;
      startY = currentYPosition();
      stopY = elmYPosition(eID) - offSet;
      distance = (stopY > startY ? stopY - startY : startY - stopY);
      if (distance < 100) {
        scrollTo(0, stopY);
        return;
      }
      speed = Math.round(distance / 100);
      if (speed >= 20) {
        speed = 20;
      }
      step = Math.round(distance / 25);
      leapY = (stopY > startY ? startY + step : startY - step);
      timer = 0;
      if (stopY > startY) {
        i = startY;
        while (i < stopY) {
          setTimeout('window.scrollTo(0, ' + leapY + ')', timer * speed);
          leapY += step;
          if (leapY > stopY) {
            leapY = stopY;
          }
          timer++;
          i += step;
        }
        return;
      }
      i = startY;
      _results = [];
      while (i > stopY) {
        setTimeout('window.scrollTo(0, ' + leapY + ')', timer * speed);
        leapY -= step;
        if (leapY < stopY) {
          leapY = stopY;
        }
        timer++;
        _results.push(i -= step);
      }
      return _results;
    };
    return {
      restrict: 'A',
      link: function(scope, element, attr) {
        return element.bind('click', function() {
          var offset;
          if (attr.target) {
            offset = attr.offset || 100;
            $log.log('Smooth scroll: scrolling to', attr.target, 'with offset', offset);
            return smoothScroll(attr.target, offset);
          } else {
            return $log.warn('Smooth scroll: no target specified');
          }
        });
      }
    };
  }
]).directive('smoothScrollJquery', [
  // '$log', function($log) {
  //   return {
  //     restrict: 'A',
  //     link: function(scope, element, attr) {
  //       return element.bind('click', function() {
  //         var offset, speed, target;
  //         if (attr.target) {
  //           // offset = attr.offset || 100;
  //           // target = $('#' + attr.target);
  //           // speed = attr.speed || 500;
  //           // $log.log('Smooth scroll jQuery: scrolling to', attr.target, 'with offset', offset, 'and speed', speed);
  //           // return $('html,body').stop().animate({
  //           //   scrollTop: target.offset().top - offset
  //           // }, speed);
  //         } else {
  //           // $log.log('Smooth scroll jQuery: no target specified, scrolling to top');
  //           // return $('html,body').stop().animate({
  //           //   scrollTop: 0
  //           // }, speed);
  //         }
  //       });
  //     }
  //   };
  // }
]);


