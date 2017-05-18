
AppModule1.directive('fileInputM', fileInputMFunc);
fileInputMFunc.$inject['Upload'];

function fileInputMFunc(Upload) { 
    return {
        restrict: 'E',
        templateUrl:  '/sites/all/modules/challenge_widget/components/directives/file-input-m/file-input-m.html',
        replace: false,
        scope: {
            element: '=',
            valuesObj : '=',
            titleButton:'@',
            allowMultiple:'@',
            viewOnly: '@',
            titleFile: '@',
            disable: '=',
            errorUpload: '@'
        },
        link: function($scope, element, attrs){
            var data = {};
            $scope.uploadedFiles = [];
            $scope.files = [];
            var filesNames = [];
            // removeFlag = true if user click on remove file,false = after $watch ;
            var removeFlag = false;
            if(!$scope.titleButton)
                $scope.titleButton = 'Search';
            if(!$scope.viewOnly)
                $scope.viewOnly = false;
            if($scope.element){
                $scope.fieldID = $scope.element.name;
                $scope.label = $scope.element.label;
            }
            if($scope.valuesObj && $scope.valuesObj[$scope.fieldID] == undefined){
                $scope.valuesObj[$scope.fieldID] = [];
            }
            else {
                $scope.uploadedFiles = JSON.parse(JSON.stringify($scope.valuesObj[$scope.fieldID]));
            }
            $scope.$watch('files.length', function() {
                // if upload file and watch not from remove file
                if ($scope.files.length  && removeFlag == false) {
                   $scope.upload($scope.files); 
                }
                else {
                    removeFlag = false;
                }
            });
            $scope.upload = function(files) {
                if($scope.allowMultiple == 'false') {
                    $scope.valuesObj[$scope.fieldID] = [];    
                }
                var length = files.length;
                //file = lfFile:name,size,type,lfFileName,lfFileType
                var file = files[length-1];
                console.log(file);
                //Check for file extension contains $scope.element.file_extensions
                if($scope.element.file_extensions.indexOf(file.lfFileName.split('.').pop()) !=-1) {
                    $scope.errorUpload ='';
                    Upload.upload({
                        url: '/api/challenge_widget/add_file',
                        file: file.lfFile,
                        headers: {'Content-Type': file.lfTagType},
                        data: {field_name: $scope.fieldID},
                        progress: function (e) {
                        }
                    }).then(function (data, status, headers, config) {
                        var data = data.data;
                        if (data.status == "Error") {
                            $scope.errorUpload  = "Error uploading file:" + data.msg;
                        }
                        else {
                            if($scope.allowMultiple == 'false') {
                                //disable choose photo button
                                $scope.disable = true;    
                            }
                            if ($scope.valuesObj[$scope.fieldID]) {
                                $scope.valuesObj[$scope.fieldID].push({fid: data.fileInfo.fid,url: data.fileInfo.url,uri: data.fileInfo.uri,filename:data.fileInfo.filename});
                            }
                        }
                    });
                }
                else { 
                    $scope.disable = true; 
                    $scope.errorUpload = file.lfFileName +' could not be uploaded.Only files with the following extensions are allowed:'+$scope.element.file_extensions;
                }
            };
            $scope.onFileRemove = function(obj,idx){
                $scope.disable = false;
                $scope.errorUpload = '';
                removeFlag = true;
                //remove upload file from $scope.files
                $scope.files.pop();
            };
        }
    }
}
