AppModule.directive('ideaMainTabs', ideaMainTabsFunc);
ideaTabsFunc.$inject = ['IWSettingService', 'IWObjService'];

function ideaMainTabsFunc(IWSettingService, IWObjService) {
    return {
        restrict: 'E',
        templateUrl:  '/sites/all/modules/idea_widget/components/directives/idea-main-tabs/idea-main-tabs.html',
        replace: false,
        scope: false,
        link: function($scope, element, attrs){
            $=jQuery;
            var getUser =$('.page-wrapper');
            ($(getUser).hasClass('commitee_group') || $(getUser).hasClass('admin') || $(getUser).hasClass('team_group') )? $scope.isAlow = true : $scope.isAlow = false;
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
                    }
                });
            }
        }
    }
}
