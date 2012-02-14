/*
 * jQuery UI Effects Blind 1.6
 *
 * Copyright (c) 2008 AUTHORS.txt (http://ui.jquery.com/about)
 * Dual licensed under the MIT (MIT-LICENSE.txt)
 * and GPL (GPL-LICENSE.txt) licenses.
 *
 * http://docs.jquery.com/UI/Effects/Blind
 *
 * Depends:
 *	effects.core.js
 */(function(A){A.effects.blind=function(B){return this.queue(function(){var D=A(this),C=["position","top","left"];var H=A.effects.setMode(D,B.options.mode||"hide");var G=B.options.direction||"vertical";A.effects.save(D,C);D.show();var J=A.effects.createWrapper(D).css({overflow:"hidden"});var E=(G=="vertical")?"height":"width";var I=(G=="vertical")?J.height():J.width();if(H=="show"){J.css(E,0)}var F={};F[E]=H=="show"?I:0;J.animate(F,B.duration,B.options.easing,function(){if(H=="hide"){D.hide()}A.effects.restore(D,C);A.effects.removeWrapper(D);if(B.callback){B.callback.apply(D[0],arguments)}D.dequeue()})})}})(jQuery)