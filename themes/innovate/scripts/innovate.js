
(function($) {
  
	$(document).ready(function(){

		if($('.pane-node-field-idea-image .field-items .field-item').length > 3) {
			$('.pane-node-field-idea-image .field-items').bxSlider({
				default: false,
				pager: false,
				slideMargin: 10,
				minSlides: 1,
				maxSlides: 3,
				slideWidth: 190,
			});
		}
		//header behaviour while administrator is logged in 
			//if user can see admin toolbar give normal menu margin on top depending if second row is horizontal
			if($('.navbar-administration #navbar-item--2-tray').hasClass('navbar-active') && $('.navbar-administration #navbar-item--2-tray').hasClass('navbar-tray-horizontal'))
				{
					$('body.navbar-administration header#navbar, .main-container').css('margin-top','78px');
				}
				else
				{
					$('body.navbar-administration header#navbar, .main-container').css('margin-top','39px');
				}
			//or vertical
			$('.navbar-administration a.navbar-icon').click(function(){
				if($('#navbar-item--2-tray').hasClass('navbar-active') && $('.navbar-administration #navbar-item--2-tray').hasClass('navbar-tray-horizontal'))
				{
					$('body header#navbar, .main-container').css('margin-top','39px');
				}
				else
				{
					$('body header#navbar, .main-container').css('margin-top','78px');
				}
				if($('#navbar-item--2-tray').hasClass('navbar-tray-vertical'))
				{
					$('body header#navbar, .main-container').css('margin-top','39px');
				}
			});
			//when user clicks on navbar toggle to toggle position horizontal or vertical
			$('.navbar-administration .navbar-toggle').click(function(){
				if($(this).val()=='horizontal'){
					$('body header#navbar, .main-container').css('margin-top','78px');
				}
				else{
					$('body header#navbar, .main-container').css('margin-top','39px');	
				}
			});
		
	});  
  
    //Open and close main manu
	Drupal.behaviors.open_close_main_manu = {
	    attach: function(context, settings) {
	    	$(".pane-main-memu").css('display','none');
            $(".close-menu").css('display','none');
	    	$('#mini-panel-sidebar_first').once('open_close_main_manu', function () {
	        	$(".open-menu").click(openMenu); 
	        	$(".close-menu").click(closeMenu); 	
	        });  
	        
	        function openMenu(e) {
	            // $(".pane-main-memu").slideToggle("slide");
	            $('.pane-main-memu').slideToggle('slide', function() {
				    if ($(this).is(':visible'))
				        $(this).css('display','flex');
				    else $(this).css('display','none');
				});
	            $(".open-menu").hide(); 
	            $(".close-menu").show();
	        }

	        function closeMenu(e) {
	            $(".pane-main-memu").hide(); 
	            $(".open-menu").show();
	            $(".close-menu").hide(); 
	        };
	        
	    }
  	}  

  	    //icons to input in register pages
	// Drupal.behaviors.placeholder = {
	//     attach: function(context, settings) {  
	  //       $('.page-user-login .inner .form-item-name .form-text').attr("placeholder", 'A');
	  //       $('.page-user-login .inner .form-item-pass .form-text').attr("placeholder", 'C');
	  //       $('.page-toboggan .inner .form-item-name .form-text').attr("placeholder", 'A');
	  //       $('.page-toboggan .inner .form-item-pass .form-text').attr("placeholder", 'C');
			// $('.page-user-register .inner .form-item-name .form-text').attr("placeholder", 'A');
			// $('.page-user-register .inner .form-item-mail .form-text').attr("placeholder", 'B');
			// $('.page-user-register .inner .form-item-pass .form-text').attr("placeholder", 'C');
		// }    
  	// } 
	  function isMobile(){
		if($(window).width() < 767) {
		   return true;
		}
		return false; 
    }
     //placeholder in mobile
	  Drupal.behaviors.editUser_placeholder = {
	    attach: function(context, settings) {  
			if(isMobile()) {
				var fields = ['name', 'current-pass', 'field-bio-und-0-value', 
				'pass-pass1', 'pass-pass2'];
				var selector;
				$.each(fields, function(key, val) {
					selector = '.page-members-edit .form-item-' + val;
					$(selector + ' input').attr("placeholder", $(selector + ' label').text());
				});
				
				// give to social buttons width of ol-item (We cannot use 100% because the absolute position)
				if ($('.row-bottom-openideal .share .social-buttons').length) {
					$share = $('.row-bottom-openideal .share .social-buttons');
					$olItem = $share.parents('.ol-item');
					$share.width($olItem.outerWidth());
				}
			}
		}		    
  	} 

    // create idea . 
	Drupal.behaviors.addIdeaMobile = {
	    attach: function(context, settings) {  
			if(isMobile()) {
				var a =  $('.page-node-add-idea #edit-field-idea-image-und-ajax-wrapper .form-item-field-idea-image-und-0').detach();
				var d =  $('.page-node-add-idea #edit-field-idea-image-und--2-ajax-wrapper .form-item-field-idea-image-und-1').detach();
				var c =  $('.page-node-add-idea #edit-field-attachments-und-ajax-wrapper .form-item-field-attachments-und-0').detach();
				var e =  $('.page-node-add-idea #edit-field-attachments-und--2-ajax-wrapper .form-item-field-attachments-und-1').detach();
				var b = $('.page-node-add-idea .node-column-sidebar');	
				a.prependTo(b);
				c.prependTo(b);
	
			}
		}
			    
  	} 


    //Search
	Drupal.behaviors.Search = {
	    attach: function(context, settings) {  
	        $('.pane-search input').attr("placeholder","search...."); 

	    }
    }


    //Popup Images
	Drupal.behaviors.PopupImages = {
	    attach: function(context, settings) {  
			var modal = document.getElementById('myModal');			
			var modalImg = document.getElementById("img01");
			var captionText = document.getElementById("caption");
			$('img').click(function(event){
				if(! $(event.target).parents('a').length){
				    modal.style.display = "block";
				    modalImg.src = event.target.src;
				    captionText.innerHTML = event.target.alt;
				}	
			});
			var span = document.getElementsByClassName("close")[0];
			modal.onclick = function() { 
			  modal.style.display = "none";
			}
			window.onkeydown = function( event ) {
			    if ( event.keyCode === 27 ) {
			 		 modal.style.display = "none";
			    }
			};
	    }
    }

    //upload Images
	Drupal.behaviors.Upload = {
	    attach: function(context, settings) {
	        $('.form-type-managed-file .image-widget-data').click(function(){
				$('input[id^="edit-field-idea-image-und"]').click();
	        });

	        $('.form-type-managed-file .file-widget').click(function(){
				$('input[id^="edit-field-attachments"]').click();
	        });

			$('.page-members-edit .form-type-file').click(function(){
				$('input[id="edit-picture-upload"]').click();
	        });


			$('input[id^="edit-field-idea-image-und"]').click(function( event ) {
				event.stopPropagation();
			});
			$('input[id="edit-picture-upload"]').click(function( event ) {
				event.stopPropagation();
			});
			$('input[id^="edit-field-attachments"]').click(function( event ) {
				event.stopPropagation();
			});
	    }
    }

})(jQuery);   
