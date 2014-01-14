/*
* Create a suggestion autocomplete box.
* Inspired by https://drupal.org/sandbox/cedric/2053111
*/

(function($) {
    Drupal.behaviors.suggestions = {
        attach: function(context) {
            var dest = $(context).find('.form-textarea:first');
            if (!dest || !dest.length) {
                return;
            }
            
            dest.keypress(function(suggestions_key) {
                
                if (suggestions_key.which != 35 && suggestions_key.which != 64) {
                    
                        // Get the last 30 characters before the cursor so we can look for a tag.
                        // 30 was chosen because it is longer than the longest word in the
                        // English language (antidisestablishmentarianism - 28 characters);
                        // much longer than the average Twitter #hashtag (probably around 8
                        // characters); and about 6 times the average English word length
                        // (about 5.1). The maximum length of a Drupal username is 60
                        // characters, and the maximum length of a taxonomy term is 255
                        // characters -- but while it is *possible* to have a tag that is
                        // longer than what we're checking for, if you've already typed 30
                        // characters, you've pretty much defeated the point of having a
                        // suggestion already. Meanwhile, we save AJAX calls.
                        var textBeforeCursor = dest.textBeforeCursor(30);
                        // Check if these characters could contain a tag.
                        if (textBeforeCursor.text.indexOf('#') < 0 && textBeforeCursor.text.indexOf('@') < 0) {
                            return;
                        }
                        
                        //do the magic of autosuggesting
                        $(dest).sew({
                            //values: $.parseJSON(response), 
                            elementFactory: elementFactory, 
                            token: ["@", "#"],
                            onFilterChanged: function(sew, expression, token) {
                                  $.ajax({
                                    url: Drupal.settings.basePath + 'index.php?q=suggestions/load&text=' + encodeURIComponent(expression)+'&token='+token,
                                    type: "GET",
                                    success: function(result) {
                                      var newValues = [];
                                      var response = $.parseJSON(result);
                                      if(response !==  null){
                                        for (var i = 0; i < Math.min(10, response.length); i++) {
                                            var repo = response[i];
                                            newValues.push({
                                              val: repo.val,
                                              meta: repo.meta
                                            })
                                          }
                                          sew.setValues(newValues);
                                      }
                                      
                                    }
                                  });
                                
                              }
                        });
                        
                   
                }
            });
        }
    }

    /**
     * Inspired by http://plugins.jquery.com/project/jCaret
     */
    $.fn.textBeforeCursor = function(distanceBefore) {
        var t = this[0];
        if ($.browser.msie) {
            var range = document.selection.createRange();
            var stored_range = range.duplicate();
            stored_range.moveToElementText(t);
            stored_range.setEndPoint('EndToEnd', range);
            var e = stored_range.text.length - range.text.length;
            var s = e - distanceBefore;
        }
        else {
            var e = t.selectionStart, s = e - distanceBefore;
        }
        if (s < 0) {
            s = 0;
        }
        var te = t.value.substring(s, e);
        return {start: s, end: e, text: te, replace: function(st) {
                t.value = t.value.substring(0, s) + st + t.value.substring(e, t.value.length);
                var newloc = s + st.length;
                if ($.browser.msie) {
                    var selRange = t.createTextRange();
                    selRange.collapse(true);
                    selRange.moveStart('character', newloc);
                    selRange.moveEnd('character', 0);
                    selRange.select();
                }
                else {
                    t.selectionStart = newloc;
                    t.selectionEnd = newloc;
                }
            }};
    };

    var customItemTemplate = "<div><span />&nbsp;<small /></div>";

    function elementFactory(element, e, token) {
        if (token == "#") {
            // Tags don't need a template
            element.text(e.val);
            return;
          }
        var template = $(customItemTemplate).find('span').text(e.val).end().find('small').text("(" + e.meta + ")").end();
        element.append(template);
    };

})(jQuery);
