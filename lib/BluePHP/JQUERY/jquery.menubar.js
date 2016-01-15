/*
 * jQuery menubar - Menu Bar plugin - v 1.0
 * 
 * Author : fabien dot pelisson at blueeyevideo dot com
 */
;(function($) {
    
    var methods = {
	init: function( options ) {
	    var options = $.extend({}, $.fn.menubar.defaults, options);
	    
	    return $(this).each(function() {
		
		var plugRef = $(this);
		plugRef.clickCb = options.clickCb;
		
		$(this).find("li a").each(function() {
		    var link = $(this).attr("href");
		    if(link.indexOf("?") != -1) {
			var query = link.split("?")[1]; /*.replace( /^?/, '#' );*/
			$(this).attr("href", '#' + query);
		    }
		    $(this).click(function() {
			var selectedElt = plugRef.find("li.active");
			if(selectedElt) {
			    selectedElt.removeClass('active');
			}
			plugRef.clickCb($(this));
			$(this).parent().addClass('active');
		    });
		});
	    });
	},

	setActiveLabel: function(label) {

	    var plugRef = $(this);

	    var selectedElt = $(this).find("li.active");
	    $(this).find("li a").each(function() {
		var elt_label = $(this).attr("href");
		if(elt_label == label) {
		    if(selectedElt) {
			selectedElt.removeClass('active');
		    }
		    $(this).parent().addClass('active');
		    selectedElt = $(this);
		}
	    });
	    return selectedElt;
	}	    
    };
	
    $.fn.menubar = function(method) {
	// Method calling logic
	if ( methods[method] ) {
	    return methods[ method ].apply( this, Array.prototype.slice.call( arguments, 1 ));
	} else if ( typeof method === 'object' || ! method ) {
	    return methods.init.apply( this, arguments );
	} else {
	    $.error( 'Method ' +  method + ' does not exist on jQuery.tooltip' );
	}      
    };

    $.fn.menubar.defaults = {
	"clickCb" : function(params) {}
    };
    
})(jQuery);
