
jQuery = jQuery || {};
!function ($) {

    $(document).ready(function () {
        var copyTextareaBtn = document.querySelector('.js-textareacopybtn');
        if(copyTextareaBtn) {
            copyTextareaBtn.addEventListener('click', function (event) {
                event.preventDefault();
                var copyTextarea = document.querySelector('.js-copytextarea');
                copyTextarea.select();
                try {
                    var successful = document.execCommand('copy');
                    var msg = successful ? 'successful' : 'unsuccessful';
                    console.log('Copying text command was ' + msg);
                } catch (err) {
                    console.log('Oops, unable to copy');
                }
            });
        }
    });

}(jQuery);
