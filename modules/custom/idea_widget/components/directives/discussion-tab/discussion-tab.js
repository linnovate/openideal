AppModule.directive('discussionTab', discussionTabFunc);
// IWFeedbackService.$inject = ['$http','$q'];
discussionTabFunc.$inject = ['IWSettingService', 'IWObjService' ,'$mdDialog'];
ideaWidgetCtrFunc.$inject = ['IWSettingService','IWObjService','$scope','$q'];
AppModule.directive('ngElementReady',ngElementReady);
ngElementReady.$inject = [];
function discussionTabFunc(IWSettingService, IWObjService ,$mdDialog) {
    return {
        restrict: 'E',
        templateUrl:  '/sites/all/modules/idea_widget/components/directives/discussion-tab/discussion-tab.html',
        replace: false,
        scope: false,
        link: function($scope, element, attrs){
            $scope.selectedRow = -1;
            $scope.selectedIndex = 0;
            IWSettingService.get(function (data,error) {
                $scope.tabs = data.tabs;

            });
            var id = Drupal.settings.idea_tabs.nid;
            if(id) {
                IWObjService.get(id, function (data, error) {
                    if (error)
                        console.log('error : get idea', error);
                    else {
                        $scope.idea = data;
                        if(!$scope.idea.discussion){
                           $scope.messageNoDiscussion = "אין כרגע  דיונים";
                        }
                        else{
                              setCtoolsModalLinks();
                        }
                    }
                });
            }  
            $scope.init=function(){
                $scope.selectedRow = -1;
                $scope.class = "un-active";
                $scope.message = "";
                $scope.messageNoDiscussion = "";
                var id = Drupal.settings.idea_tabs.nid;
                if(id) {
                    IWObjService.get(id, function (data, error) {
                        if (error)
                            console.log('error : get idea', error);
                        else {
                            $scope.idea = data;
                            if(!$scope.idea.discussion){
                               $scope.messageNoDiscussion = "אין כרגע דיונים";
                            }
                        }
                    });
                }
            }
            
            $scope.selDiscussion=function(discussion ,$index){
                $scope.selectedIndex = $index;
                if(discussion == $scope.selected_discussion) {
                    $scope.selected_discussion = '';
                    return;
                }
                $scope.selected_discussion=discussion;  
                if ($scope.class === "un-active")
                  $scope.class = "active";
                else
                  $scope.class = "un-active";
            }
            $scope.initD = function(){
                console.log(initD);
            }
            

            $scope.isSelected=function(discussion){
                return $scope.selected_discussion===discussion;
            }
            function setCtoolsModalLinks(){
                setTimeout(function() {
                    $('.inside a.ctools-use-modal').each(function() {
                        // console.log(this);
                        var $this = $(this);
                        console.log(Drupal.CTools.Modal.clickAjaxLink)
                        // $this.click(Drupal.CTools.Modal.clickAjaxLink);
                        // Create a drupal ajax object
                        var element_settings = {};
                        if ($this.attr('href')) {
                          element_settings.url = $this.attr('href');
                          element_settings.event = 'click';
                          element_settings.progress = { type: 'throbber' };
                        }
                        var base = $this.attr('href');
                        Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
                    });
                },100);

            }
            
        }
    }
   

}

function ngElementReady() {
    return {
        priority: -1000, // a low number so this directive loads after all other directives have loaded. 
        restrict: "A", // attribute only
        link: function($scope, $element, $attributes) {
            console.log(" -- Element ready!");
            // do what you want here.
            angular.element("a.uimodal").once("uimodal").click(function(e) {
            e.preventDefault();
            var outputHolder = $('<div class="uimodal-output loading"></div>');
            $("body").append(outputHolder);
            var href = $(this).attr("href") + "#block-system-main .content";
            outputHolder.dialog({
              modal: true,
              show: "fade",
              hide: "fade",
              position: ["center", 130],
              width: 660,
              draggable: false,
              resizable: false,
              close: function(event, ui) {
                // Remove it from the DOM.
                outputHolder.remove();
              }
            });
            outputHolder.load(href, null, function() {
              outputHolder.removeClass("loading");
//             Drupal.ajax={}
//             Drupal.ajax['ajaxing'] = true;

//             Drupal.ajax['callback'] = "ideal_feedback_node_form_ajax_submit";

//             Drupal.ajax['effect'] = "none";

//             Drupal.ajax['ajaxing'] = true;
//             Drupal.ajax['ajaxing'] = true;
//             Drupal.ajax['ajaxing'] = true;
//             Drupal.ajax['ajaxing'] = true;
//             Drupal.ajax['ajaxing'] = true;

// \\


            Drupal.settings.ajax['edit-submit']={};
            Drupal.settings.ajax['edit-submit']['callback'] = "ideal_feedback_node_form_ajax_submit";
            Drupal.settings.ajax['edit-submit']['event'] = "mousedown";
            Drupal.settings.ajax['edit-submit']['keypress'] = true;
            Drupal.settings.ajax['edit-submit']['prevent'] = "click";
            Drupal.settings.ajax['edit-submit']['submit'] = {_triggering_element_name: "op" , _triggering_element_value : "צור דיןו"} ;
            Drupal.settings.ajax['edit-submit']['url'] = "/he/system/ajax";
            Drupal.settings.ajax['edit-submit']['wrapper'] = "discussion-node-form";
              console.log(Drupal.settings.ajax);
              Drupal.attachBehaviors();
            });
            // $.ajax({
            //   url: href,
            //   //context: $("#block-system-main .content")
            // }).done(function(html) {
            //     // var data = $.parseHTML(html);
            //     // var a = data.find($("#block-system-main .content"));
            //   outputHolder.html(html);

            //   Drupal.attachBehaviors(outputHolder);
            //});
          });

  

  // Client side drupal_set_message
  $.fn.extend({
    setMessage: function(message, type) {
      type = type || 'status';
      var messages = $('div.messages.' + type);
      if (messages.length === 0) {
        messages = $('<div class="messages ' + type + '"><ul></ul></div>');
        messages.prependTo('.region-content');
      }
      messages.find('ul').append('<li>' + message + '</li>');
    },
    setModalMessage: function(message, type) {
      type = type || 'status';
      var messages = $('.ui-dialog-content .uimodal div.messages.' + type);
      if (messages.length === 0) {
        messages = $('<div class="messages ' + type + '"><ul></ul></div>');
        messages.prependTo('.ui-dialog-content .uimodal');
      }
      messages.find('ul').append('<li>' + message + '</li>');
    },
    // for usage as ajax_command_invoke('document', 'reloadPage');
    reloadPage: function() {
      location.reload();
    }
  });
        }
    };
}