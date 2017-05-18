// declare a module
var AppModule = angular.module('moiApp', ['ngFileUpload','ngMaterial','ngMessages', 'lfNgMdFileInput']);

AppModule.factory('IWSettingService', IWSettingService);
IWSettingService.$inject = ['$http', '$q'];

AppModule.factory('IWObjService',IWObjService);
IWObjService.$inject = ['$http','$q'];

AppModule.controller('ideaWidgetCtr', ideaWidgetCtrFunc);
ideaWidgetCtrFunc.$inject = ['IWSettingService','IWObjService','$scope','$q' ,'$interval','$http','$log','$mdDialog'];
AppModule.directive('fileInput',fileInputFunc);
fileInputFunc.$inject = ['Upload'];


function ideaWidgetCtrFunc(IWSettingService,IWObjService,$scope,$q , $interval, $http, $log, $mdDialog) {

    var vm = this;
        vm.transformChip = transformChip;
        vm.autocompleteDemoRequireMatch = true;
        vm.isDisabled = false;
        vm.challenges = [];

    var self = this;
      self.loader = false;
      self.determinateValue = 30;
      chllenge_ref = null;

      self.simulateQuery = false;
      self.selectedItemChange = selectedItemChange;
      self.searchTextChange   = searchTextChange;

      // Iterate every 100ms, non-stop and increment
      // the Determinate loader.
      
    /**
     * Return the proper object when the append is called.
     */
    function transformChip(chip) {
      // If it is an object, it's already a known chip
      if (angular.isObject(chip)) {
        return chip;
      }
      // Otherwise, create a new one
      return { name: chip, type: 'new' }
    }


    function searchTextChange(text) {
      $log.info('Text changed to ' + text);
    }

    function selectedItemChange(item) {
      $log.info('Item changed to ' + JSON.stringify(item));
    }

    vm.init = function () {
        // var chllenge_ref =  window.location.pathname.split('/');
        // chllenge_ref = chllenge_ref[3];
        // vm.idea = {};
        // vm.idea.field_adding_partners = [];
        // vm.idea.field_challenge_ref = chllenge_ref ;
        var args =  window.location.pathname.split('/');
        if(args[2] && Drupal.settings.fromchallenge == "yes"){
            chllenge_ref = args[2];
        }
        vm.idea = {};
        //vm.idea.field_adding_partners = [];
        if (chllenge_ref) {
           vm.idea.field_challenge_ref = chllenge_ref ;
           vm.isDisabled  = true;

        }
        vm.title = Drupal.t('solution suggestion');
        vm.subTitle = Drupal.t('Share your ideas with us');
        vm.language = 'en';
        vm.contentHeight = jQuery(window).height()-270;
        if(!localStorage.getItem("openIdealWidget")) {
             var data = {'external': false};
             localStorage.setItem("openIdealWidget", JSON.stringify(data));
        }
        var extData = JSON.parse(localStorage.getItem("openIdealWidget"));

        var params = window.location.search.replace("?", "");
        var args = location.pathname.split('/');
        var ideaFromLs = JSON.parse(localStorage.getItem("openIdealWidget_idea"));
        if(ideaFromLs){
            vm.idea = ideaFromLs;
            if(chllenge_ref != 'edit')
               vm.idea.field_challenge_ref = chllenge_ref;
            if(ideaFromLs.field_adding_partners == undefined) {
                vm.idea.field_adding_partners = [];
            }
        }
        if(extData.external == true || params.indexOf('external=true') !==-1){
             vm.external = true;
        }
       else if(parseInt(args[args.length-2]) && args[args.length-1]=='edit' || (Drupal.settings.action == 'edit') ){
            var id = parseInt(args[args.length-2]) && args[args.length-1]=='edit' ? parseInt(args[args.length-2]):parseInt(args[args.length-1]);
            vm.edit = true;
            var data = {id:  id, action: 'edit'};            // var data = {id:  parseInt(args[args.length-1]), action: 'edit'};
            IWObjService.get(data,function(data,error){
                if(error)
                    console.log('error : get idea',error);
                else {
                    vm.idea = data;
                    //console.log('dataget',vm.idea);
                    vm.title = vm.language == 'en' ?  'Edit solution ' + vm.idea.title: 'עריכת רעיון ' + vm.idea.title;
                    var chllenge_ref_value1 = vm.idea.field_challenge_ref[0].value;
                    // get all exits challenges
                    IWSettingService.getChallenges().then(function(response) {
                    if(response) {
                        vm.challenges = response;
                        if (chllenge_ref_value1) {
                            // display chllenge_ref display by key(node nid)
                             var chllenge_ref_value  = vm.challenges.filter(function( obj ) {
                              return obj.value == chllenge_ref_value1;
                            });
                            vm.idea.field_challenge_ref = chllenge_ref_value[0];
                        }
                        
                    }

                });
                if(vm.idea.field_adding_partners == undefined) {
                    vm.idea.field_adding_partners = [];
                }
                }
            });
        }
        IWSettingService.get(function (data,error) {
            vm.actionLabels = {'draft':{en:'Save as Draft',he:'שמור כטיוטה'},'save':{en:'Save',he:'שמור ושלח'},'back':{en:'Back',he:'חזור'},'reset':{en:'reset',he:'נקה'} ,'next':{en:'Next',he:'המשך'},continue:'Continue'} ;
            vm.tabs = data.tabs;
            vm.language = 'en';
            vm.activeTab = 0;
            vm.oiSite = data.oi_site;
            if(vm.oiSite.indexOf(window.location.host) != -1){
              vm.isInner = true;
            }
        });

        // get all challenges
        IWSettingService.getChallenges().then(function(response) {
            if(response) {
                vm.challenges = response;
                if (chllenge_ref) {
                    // display chllenge_ref value by key,args[2]
                     var chllenge_ref_value  = vm.challenges.filter(function( obj ) {
                      return obj.value == chllenge_ref;
                    });
                     console.log(chllenge_ref_value);
                    vm.idea.field_challenge_ref = chllenge_ref_value[0].display;
                }
                
            }

        });

        $interval(function() { 
            self.determinateValue += 1;
            if (self.determinateValue > 100) {
              self.determinateValue = 30;
            }
        }, 100);

    };

    vm.getMatches = function(searchText){
        var results = searchText ? vm.challenges.filter(createFilterFor(searchText)) : vm.challenges,
          deferred;
        if (self.simulateQuery) {
            deferred = $q.defer();
            $timeout(function () { deferred.resolve( results ); }, Math.random() * 1000, false);
            return deferred.promise;
          } else {
            return results;
        }
    }

    /**
     * Create filter function for a query string
     */
    function createFilterFor(query) {
      var lowercaseQuery = angular.lowercase(query);

      return function filterFn(item) {
        return (angular.lowercase(item.display).indexOf(lowercaseQuery) == 0);
      };

    }

    vm.selectTab = function(tab){
    }
    vm.leaveTab = function(tab){
        saveClient(vm.idea);
    }
    vm.save = function () {
        var data = validate_and_prepare_post();
        if(!data){
            return;
        }
        self.loader = true;
        IWObjService.save(data,function (data,error) {
            if(error){
                console.log(error);
            }
            else {
                if (vm.idea.nid == undefined)
                    vm.idea.nid = data.nid;
                 saveClient({},true);
                if(data.path){
                    window.location.href = data.path;
                    self.loader = true;
                }

                else{alert('success save Idea:' + vm.idea.title);  }
            }
        });
    }
    vm.saveDraft = function(){
        self.loader = true;
        var data = validate_and_prepare_post();
        if(!data){
            return;
        }
        IWObjService.saveDraft(data,function (data,error) {
            vm.inProgress = false;
            if(error){
                console.log(error);
            }
            else {
                if (vm.idea.nid == undefined)
                    vm.idea.nid = data.nid;
                // alert('success save a draft Idea:' + vm.idea.title);
                saveClient({},true);
                $scope.ideaForm.$setPristine();
                if(data.path){
                  window.location.href = data.path;
                  self.loader = true;
                } 
            }
        });
    }
    vm.reset = function(){
        localStorage.removeItem("openIdealWidget_idea");
        vm.idea = {};

    }
    // Close Dialog
    vm.close  = function() {
        $mdDialog.hide(); 
    }

    function validate_and_prepare_post(){
        if($scope.ideaForm.$invalid || $scope.ideaForm.field_challenge_ref){
            vm.errMessages = [];
            for(var i=0;i<vm.tabs.length;i++){
                var tab = vm.tabs[i];
                for(var j=0;j<tab.fields.length;j++) {
                    var name1 = tab.fields[j].name;
                    //console.log('name1',name1);
                     if(tab.fields[j].name && $scope.ideaForm[name1].$invalid){
                         if($scope.ideaForm[tab.fields[j].name]['$error'].required){
                            if(vm.language == 'en')
                                vm.errMessages[vm.errMessages.length] = tab.fields[j].label +' field in Tab:'+tab.name+' is ' + 'required';
                            else{
                                vm.errMessages[vm.errMessages.length] = tab.fields[j].label +' בטאב '+tab.name+' הינו שדה חובה ';
                            }
                         }
                     }
                     // Check if field_challenge_ref field value is exsist
                     if (name1 == 'field_challenge_ref' ) {
                        var challenges_ref_value1 = $scope.ideaForm.field_challenge_ref.$viewValue;
                        //console.log('rivka',challenges_ref_value1);

                        chllenge_exits  = vm.challenges.filter(function( obj ) {
                            //console.log(obj.display);
                            return obj.display == challenges_ref_value1;
                        }); 
                        //console.log('chllenge_exits', chllenge_exits);
                        if (chllenge_exits.length == 0) {    
                            //console.log('not exits',vm.errMessages);
                            //console.log(vm.errMessages[vm.errMessages.length]);
                           if(vm.language == 'en')
                                vm.errMessages[vm.errMessages.length] = 'Chose an existing business name in Tab: '+tab.name;
                            else{
                                vm.errMessages[vm.errMessages.length] = 'בחר אתגר קיים בטאב '+tab.name;
                            } 
                        }

                     }
                }
            }
            // if the form is invalid or challenge is not exist
            if($scope.ideaForm.$invalid || chllenge_exits.length == 0)
                return;
        }
        else vm.errMessages = [];
        vm.inProgress = true;
        saveClient(vm.idea);
        var data = {};
        data = vm.idea;
        if (chllenge_ref == null ) {
            if(data.field_challenge_ref && data.field_challenge_ref.value)
                data.field_challenge_ref = data.field_challenge_ref.value;
        }
        else {
            data.field_challenge_ref = chllenge_ref ;
        }
        data.info = JSON.stringify({external:data.external});
        data.data = JSON.stringify(data);
        return data ;
    }
    function saveClient(data,clear){
         if(($scope.ideaForm.$dirty && vm.idea.nid == undefined)  || clear ){
           localStorage.setItem("openIdealWidget_idea",JSON.stringify(data));
         }
    }
}
function getURLParameter(name) {
    return decodeURIComponent((new RegExp('[?|&]' + name + '=' + '([^&;]+?)(&|#|;|$)').exec(location.search) || [null, ''])[1].replace(/\+/g, '%20')) || null;
}
function IWSettingService($http, $q){
    function get(cb) {
        return $http({method:'GET', url: '/api/idea_widget/get-fields',params:{}})
            .success(function(data, status, headers, config) {
                return cb(data,null);
            })
            .error(function(data, status, headers, config) {
                return cb(data,null);
            });
    }
    function getChallenges() {
      var deferred = $q.defer();
      $http({method:'GET', url: '/all_challenges',params:{}})
        .then(function(data, status, headers, config) {
            if(data.error)
                deferred.reject(data.error);
            else {
                var innerData = [];
                if(!data){
                   deferred.resolve(innerData);
                }
                angular.forEach(data.data, function(value, key) {
                    innerData.push({'value':value.value,'display':value.fields.title});
                });
                deferred.resolve(innerData); 
                }
        });
        return deferred.promise;
    }
    return {
        get:get,
        getChallenges:getChallenges, 
    }
}

function IWObjService($http,$q) {
    function getIdea(data,cb){
        var id = data;
        if(typeof data === 'object')
           id = data.id;
        return $http({method:'GET', url: '/api/idea_widget/entity/get/'+ id ,params:{data: data}})
            .success(function(data, status, headers, config) {
                if(!(data.data && data.data.nid)){
                    cb(null,data);
                }
                return cb(data.data,null);
            })
            .error(function(data, status, headers, config) {
                //return 'error';
                return cb(data.error,null);
            });
    }
    function saveIdea(data,cb) {
        var url ='/api/idea_widget/entity/save';
        return postIdea(url,data,cb);
    }
    function saveDraftIdea(data,cb) {
        var url ='/api/idea_widget/entity/save_draft';
        return postIdea(url,data,cb);
    }
    Object.toparams = function ObjecttoParams(obj) {
        var p = [];
        for (var key in obj) {
            p.push(key + '=' + encodeURIComponent(obj[key]));
        }
        return p.join('&');
    };
    function postIdea(url,data,cb) {
        $http({method:'POST',
            url: url,
            data: Object.toparams(data),
            headers: {'Content-Type': 'application/x-www-form-urlencoded'}}).success(function (data, status, headers, config) {
            if(data.error) {
                return cb(null, data.error);
            }
            else if(data.data.nid == undefined){
                return cb(null, 'cannot Success Save Idea');
            }
            return cb(data.data);
        }).error(function(data, status, headers, config) {
            return cb(null,data);
        });
    }
    function getTerm(term){
      var deferred = $q.defer();
      //$http({method:'GET', url: '/entityreference/autocomplete/single/field_adding_partners/node/idea/NULL/'+term,params:{}})
      $http({method:'GET', url: '/all_challenges/'+term,params:{}})
        .success(function(data, status, headers, config) {
            console.log('data-all-challenges', data);
            var innerData = [];
            if(!data){
               deferred.resolve(innerData);
            }
            angular.forEach(data, function(value, key) {
                //innerData.pu  sh({'key':key,'display':key});
                innerData.push({'value':value.value,'display':value.fields.title});
            });
            deferred.resolve(innerData); 
        })
        .error(function(data, status, headers, config) {
           deferred.reject(data.error);
        });
        return deferred.promise;
    }
    return {
        get: getIdea,
        save: saveIdea,
        saveDraft:saveDraftIdea,
        getTerm:getTerm
    }

}

function fileInputFunc(Upload) {
    return {
        restrict: 'E',
        templateUrl:  '/sites/all/modules/idea_widget/components/directives/file-input/fileInput.html',
        replace: false,
        scope: {
            element: '=',
            valuesObj : '=',
            titleButton:'@',
            allowMultiple:'@',
            viewOnly: '@'
        },
        link: function($scope, element, attrs){
            var data = {};
            $scope.countFilesUplouded = 0;
            $scope.uploadedFiles = [];
            $scope.success_upload = false;
            $scope.files = [];
            if($scope.allowMultiple == undefined)
                $scope.allowMultiple = true;
            if(!$scope.titleButton)
                $scope.titleButton = 'Attach Any File';
            if(!$scope.viewOnly)
                $scope.viewOnly = false;
            if($scope.element){
                $scope.fieldID = $scope.element.name;
                $scope.label = $scope.element.label;
                if($scope.element.type == 'image'){
                    $scope.pattern =  'image/*'
                }
                else if($scope.element.type == 'file'){
                    $scope.pattern = 'application/*,text/*';
                }
            }
            if($scope.valuesObj && $scope.valuesObj[$scope.fieldID] == undefined){
                $scope.valuesObj[$scope.fieldID] = [];
            }
            else {
                $scope.uploadedFiles = JSON.parse(JSON.stringify($scope.valuesObj[$scope.fieldID]));
                $scope.countFilesUplouded = $scope.uploadedFiles.length;
            }

            $scope.$watch('files', function() {
                $scope.upload($scope.files);
            });
            $scope.upload = function(files) {
                //files: an array of files selected, each file has name, size, and type.
                for (var i = 0; i < files.length; i++) {
                    var file = files[i];
                    if($scope.element.file_extensions.indexOf(file.name.split('.').pop()) !=-1) {
                        $scope.error = '';
                        Upload.upload({
                            url: '/api/idea_widget/add_file',
                            file: file,
                            headers: {'Content-Type': file.type},
                            data: {field_name: $scope.fieldID},
                            progress: function (e) {
                            }
                        }).then(function (data, status, headers, config) {
                            var data = data.data;
                            if (data.status == "Error") {
                                $scope.error  = "Error uploading file:" + data.msg;
                            }
                            else {
                                if ($scope.allowMultiple == 'false') {
                                    $scope.valuesObj[$scope.fieldID] = {
                                        fid: data.fileInfo.fid,
                                        path: data.fileInfo.url,
                                        uri: data.fileInfo.uri
                                    };
                                }
                                else {
                                    $scope.uploadedFiles[$scope.countFilesUplouded] = data.fileInfo;
                                    $scope.countFilesUplouded++;
                                    if ($scope.valuesObj[$scope.fieldID]) {
                                        $scope.valuesObj[$scope.fieldID].push({fid: data.fileInfo.fid,url: data.fileInfo.url,uri: data.fileInfo.uri,filename:data.fileInfo.filename});
                                    }
                                }
                            }
                        });
                    }
                    else {
                        $scope.error = file.name +' could not be uploaded.Only files with the following extensions are allowed:'+$scope.element.file_extensions
                    }
                }
            };
        }
    }
}
jQuery = jQuery || {};
!function ($) {
    function generateId(id,i){
        if(document.getElementById(id+ '-' + i)){
            i++;

            generateId(id ,i);
        }
        return id + '-' + i;
    }
    $(document).ready(function () {
        var id  = generateId('idea-widget-wrapper',0);
        $('#idea-widget-wrapper').attr('id', id);
        angular.bootstrap(document.getElementById(id), ['moiApp']);

        var id  = generateId('idea-modal-wrapper',0);
        $('#idea-modal-wrapper').attr('id', id);
        angular.bootstrap(document.getElementById(id), ['moiApp']);
    });
}(jQuery);


