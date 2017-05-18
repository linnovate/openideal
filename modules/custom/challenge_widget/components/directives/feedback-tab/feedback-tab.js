AppModule1.directive('feedbackTab', feedbackTabFunc);
AppModule1.factory('IWFeedbackService', IWFeedbackService);
IWFeedbackService.$inject = ['$http','$q'];
feedbackTabFunc.$inject = ['IWSettingService', 'IWObjService','IWFeedbackService' ,'$mdDialog'];
challengeWidgetCtrFunc.$inject = ['IWSettingService','IWObjService','$scope','$q'];

function feedbackTabFunc(IWSettingService, IWObjService ,IWFeedbackService,$mdDialog) {
    return {
        restrict: 'E',
        templateUrl:  '/sites/all/modules/challenge_widget/components/directives/feedback-tab/feedback-tab.html',
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
                        if(!$scope.idea.feedback){
                           $scope.messageNoFeedback = "אין כרגע בקשות למשוב";
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
                $scope.messageNoFeedback = "";
                var id = Drupal.settings.idea_tabs.nid;
                if(id) {
                    IWObjService.get(id, function (data, error) {
                        if (error)
                            console.log('error : get idea', error);
                        else {
                            $scope.idea = data;
                            if(!$scope.idea.feedback){
                               $scope.messageNoFeedback = "אין כרגע בקשות למשוב";
                            }
                            else{
                                setCtoolsModalLinks();
                            }
                        }
                    });
                }  
            }
            
            $scope.selFeedback=function(feedback ,$index){
                $scope.selectedIndex = $index;
                if(feedback == $scope.selected_feedback) {
                    $scope.selected_feedback = '';
                    return;
                }
                $scope.selected_feedback=feedback;  
                if ($scope.class === "un-active")
                  $scope.class = "active";
                else
                  $scope.class = "un-active";
            }
            
            $scope.isSelected=function(feedback){
                return $scope.selected_feedback===feedback;
            }
            
            $scope.save_content= function(content , feedbackId) {
                feedbackContent ={};
                feedbackContent.content = content;
                feedbackContent.feedbackId = feedbackId;
                var data = {};
                data.info = JSON.stringify({external:true});
                data.data = JSON.stringify(feedbackContent);
                IWFeedbackService.saveContent(data,function (data,error) {
                    if(error){
                        console.log(error);
                        $scope.message = error;
                    }
                    else {
                        $scope.init();
                        window.scrollTo(0, 0);
                    }
                });
            }
            function setCtoolsModalLinks(){
                 setTimeout(function() {
                $('.inside a.ctools-use-modal').each(function() {
                    var $this = $(this);
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
function IWFeedbackService($http,$q) {
    function saveContent(data,cb) {
        var url ='/api/feedback/entity/save';
        return postFeedback(url,data,cb);
    }
    Object.toparams = function ObjecttoParams(obj) {
        var p = [];
        for (var key in obj) {
            p.push(key + '=' + encodeURIComponent(obj[key]));
        }
        return p.join('&');
    };

    function postFeedback(url,data,cb) {
        $http({method:'POST',
            url: url,
            data: Object.toparams(data),
            headers: {'Content-Type': 'application/x-www-form-urlencoded'}}).success(function (data, status, headers, config) {
            if(data.error) {
                return cb(null, data.error);
            }
            return cb(data.data);
        }).error(function(data, status, headers, config) {
            return cb(null,data);
        });
    }

     return {
        saveContent: saveContent,
    }

}
