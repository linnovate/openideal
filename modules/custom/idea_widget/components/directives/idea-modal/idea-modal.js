AppModule.directive('ideaModal', ideaModalFunc);
ideaModalFunc.$inject = ['$mdDialog'];

function ideaModalFunc($mdDialog) {
    return {
        restrict: 'E',
        template:  '<md-button class="md-primary md-raised challenge-add-idea-button" ng-click="open($event)">{{label}}</md-button>',
        link: function($scope, element, attrs){
            $scope.label = 'Open Modal';
            $scope.action = 'Open Modal';
            $scope.fromChallenge = 'Open Modal';
            if(attrs.label){
              $scope.label = attrs.label;
              $scope.action = attrs.action;
              $scope.fromChallenge = attrs.fromchallenge;
              Drupal.settings.action = attrs.action;
              Drupal.settings.fromchallenge = attrs.fromchallenge;
            }
           $scope.open= function(ev) {
            $mdDialog.show({
              templateUrl: '/sites/all/modules/idea_widget/components/idea-widget-form.html',
              parent: angular.element(document.body),
              targetEvent: ev,
              clickOutsideToClose:true,
              fullscreen: $scope.customFullscreen // Only for -xs, -sm breakpoints.
             })
            .then(function(answer) {
              $scope.status = 'You said the information was "' + answer + '".';
            }, function() {
              $scope.status = 'You cancelled the dialog.';
            });
          };
          $scope.cancel = function() {
            $mdDialog.cancel();
          };
          $scope.hide = function() {
            $mdDialog.hide();
          };
          $scope.answer = function(answer) {
            $mdDialog.hide(answer);
          };
            // var id = Drupal.settings.idea_tabs.nid;
        }
    }
};