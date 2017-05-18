AppModule.directive('ideaTabs', ideaTabsFunc);
ideaTabsFunc.$inject = ['IWSettingService', 'IWObjService' ,'$log' , '$timeout', '$window'];

function ideaTabsFunc(IWSettingService, IWObjService, $log , $timeout, $window) {
    return {
        restrict: 'E',
        templateUrl:  '/sites/all/modules/idea_widget/components/directives/idea-tabs/idea-tabs.html',
        replace: false,
        scope: false,
        link: function($scope, element, attrs ,attr){
            $scope.selected = 0;
            $scope.select= function(index) {
               $scope.selected = index; 
            };
            IWSettingService.get(function (data,error) {
                $scope.tabs = data.tabs;
                console.log(data);
            });
            var id = Drupal.settings.idea_tabs.nid;
            if(id) {
                IWObjService.get(id, function (data, error) {
                    if (error)
                        console.log('error : get idea', error);
                    else {
                        $scope.idea = data;
                    }
                });
            }
        }
    }
}
