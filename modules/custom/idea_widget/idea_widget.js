var scripts = document.getElementsByTagName('script');
var myScript = scripts[ scripts.length - 1 ];
var queryString = myScript.src.replace(/^[^\?]+\??/,'');
var params = parseQuery( queryString );
var parent = myScript.parentElement;
var scriptSrc = myScript.getAttribute('src');
var ids = [];
if(scriptSrc.indexOf('/sites/all/modules/idea_widget/idea_widget.js') != -1 ) {
    var id = generateId('oi-idea-widget', 0);
    parent.setAttribute('id', id);
}

function parseQuery ( query ) {
    var Params = new Object ();
    if ( ! query ) return Params; // return empty object
    var Pairs = query.split(/[;&]/);
    for ( var i = 0; i < Pairs.length; i++ ) {
        var KeyVal = Pairs[i].split('=');
        if ( ! KeyVal || KeyVal.length != 2 ) continue;
        var key = unescape( KeyVal[0] );
        var val = unescape( KeyVal[1] );
        val = val.replace(/\+/g, ' ');
        Params[key] = val;
    }
    return Params;
}
function generateId(id,i){
    if(document.getElementById(id+ '-' + i)){
        i++;

        generateId(id ,i);
    }
    return id + '-' + i;
}
String.prototype.replaceAll = function(search, replacement) {
    var target = this;
    return target.replace(new RegExp(search, 'g'), replacement);
};


jQuery = jQuery || {};
!function ($) {
    if(scriptSrc.indexOf('/sites/all/modules/idea_widget/idea_widget.js') !=-1) {
        var remote = '';
        var url = "/sites/all/modules/idea_widget/components/index.html";
        var data = {};
        var srcScript = myScript.getAttribute('src');
        var  remote_s = srcScript.substring(0, srcScript.indexOf('//') + 2 + srcScript.substring(srcScript.indexOf('//') + 2).indexOf('/'));
        if (remote_s.indexOf(window.location.hostname) === false) {
            remote = remote_s;
            url = remote + url+ '?external=true';
            var idiframe = generateId('oi-iframe',0);
            var iframe = '<iframe id="'+idiframe+'" height="600px" src="' + url + '" width="400px"></iframe>';
            data.external = true;
            appendData(iframe);
        }
        else {
             url +='?'+queryString;
             data.external = false;
             localStorage.setItem("openIdealWidget", JSON.stringify(data));
            var idE = id;
            var paramsE = params;
            $.get(url, function (data) {
                appendData(data,idE,paramsE);

            });
            
        }
    }
        function appendData(data,idE,params) {
            var buttonText = params.popupButton ? params.popupButton : 'Add Idea(openIdeal)';
            if (params.popup == 'true') {
                var cssUrl = remote + "/sites/all/modules/idea_widget/idea_widget.css";
                var data = '<link rel="stylesheet" href="' + cssUrl + '"><button id="oiBtn" class="">' + buttonText + '</button><div id="oiModal" class="modal"> <div class="modal-content"> <div class="modal-header"> <span class="oi-modal-close">×</span> </div> <div class="modal-body">' + data + '</div> <div class="modal-footer"></div></div></div>';
                $('#' + idE).append(data);
                var modal = document.getElementById('oiModal');
                var btn = document.getElementById("oiBtn");
                var span = document.getElementsByClassName("oi-modal-close")[0];

                btn.onclick = function () {
                    modal.style.display = "block";
                }
                span.onclick = function () {
                    modal.style.display = "none";
                }
                window.onclick = function (event) {
                    if (event.target == modal) {
                        modal.style.display = "none";
                    }
                }
            }
            else {
                $('#' + idE).append(data);
            }
        }
    $(document).ready(function () {
    });
    $('iframe').load(function() {
        this.style.height =
            this.contentWindow.document.body.offsetHeight + 'px';
    });
}(jQuery);