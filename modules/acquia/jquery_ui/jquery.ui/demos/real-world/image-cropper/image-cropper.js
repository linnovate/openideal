   var getSizeImg = function(src) {
        var timg = $('<img>').attr('src', src).css({ position: 'absolute', top: '-1000px', left: '-1000px' }).appendTo('body');
        var size = {width: timg.get(0).offsetWidth, height: timg.get(0).offsetHeight };
        
        try { document.body.removeChild(timg[0]); }
        catch(e) {};

        return size;
    };

$().ready(function(){

	$('#_Container').resizable({
		containment: $('#_Wrapper'),
		handles: 'all',
		knobHandles: true,
		autoHide: true,
		minWidth: 100,
		minHeight: 100,
		resize: function(event, ui){
			var self = $(this).data("resizable"),
				imageSize = $('#_Container').data("image-size"),
				top = self.position.top, 
				height = ((self.position.top + self.size.height) <= imageSize.height ? self.size.height : imageSize.height),
				left = self.position.left,
				width = ((self.position.left + self.size.width) <= imageSize.width ? self.size.width : imageSize.width);

			left = left > 0 ? left : 0;
			top = top > 0 ? top : 0;
			
			var bgPos = '-' + (left + 1) + 'px -' + (top + 1) + 'px';

			//the borders of the resize rect are offsetting the bg pos incorrectly. subtract (add, since its a negative) 1 to fix.
			$(this).css({backgroundPosition: bgPos});

			$("#log-top").html(top + "px");
			$("#log-height").html(height + "px");
			$("#log-left").html(left + "px");
			$("#log-width").html(width + "px");
		},
		stop: function(event, ui){
			var self = $(this).data("resizable"),
				top = self.position.top,
				left = self.position.left;

			left = left > 0 ? left : 0;
			top = top > 0 ? top : 0;

  		$(this).css({backgroundPosition: ((left + 1) * -1) + 'px ' + ((top + 1) * -1) + 'px'});
		}
	})
	.draggable({
		cursor: 'move',
		containment: $('#_Wrapper'),
		drag: function(event, ui){
			var self = $(this).data("draggable");
			$(this).css({backgroundPosition: ((self.position.left + 1) * -1) + 'px ' + ((self.position.top + 1) * -1) + 'px'});

			$("#log-top").html(self.position.top+"px");
			$("#log-left").html(self.position.left+"px");
		}
	});

	$('.thumbs')
		.find("li a")
		.click(function(event){
		  $('#_Container').css({top: '0', left: '0'});
      
      var size = getSizeImg($(this).find("img").attr("src"));

	  	$('#_Container_Image').css({
	  		width: size.width, 
	  		height: size.height, 
	  		background: 'transparent url('+$(this).find("img").attr("src")+') no-repeat scroll 0%' 
	  	});
	  	
      $('#_Wrapper').css({ width: size.width, height: size.height });
	  	$('#_Container')
	  		.css('background', 'transparent url('+$(this).find("img").attr("src")+') no-repeat scroll 0px 0px')
	  		.data("image-size", size);

      return false;
		});

	$('#_Container_Image').css({ opacity: 0.5 });
  $("#log-height").html($('#_Container').height()+"px");
  $("#log-width").html($('#_Container').width()+"px");
  
  $(".thumbs li a:first").click();
});
