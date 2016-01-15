var BluePHP = {};

(function(context) {

    context.createMethodObject = function (newObject, methodName) {
	return function() 
	{ 
	    var params = null;
	    var options = {};

	    if(arguments.length == 1) {
		params = arguments[0];
	    }

	    options = { "__class" : newObject.__classAddress,
			"__construct" : newObject.__consParams,
			"__call" : { "__function" : methodName,
				     "__params" : params } };

	    if(arguments.length == 2) {
		options = $.extend(options, arguments[1]);
	    }
		
	    var jData = null;
	    $.ajax({
		type: "POST",
		url: "/BluePHP/Utils/ClassFunction.php",
		data: options,
		async: false,
		success: function(data) { jData = data; }
	    });
	    return jData;
	};
    };

    context.newObject = function(classAddress) {
	var newObject = new Object();
	newObject.__classAddress = classAddress;
	newObject.__consParams = null;
	if(arguments.length == 2) {
	    newObject.__consParams = arguments[1];
	}
	    
	$.ajax({
	    url: "/BluePHP/Utils/ClassMethods.php",
	    data: { "__class" : newObject.__classAddress },
	    async: false,
	    success: function(data) {
		newObject.__className = data["class"];
		for(var i = 0; i < data["methods"].length; ++i) {
		    newObject[data["methods"][i]] = context.createMethodObject(newObject, data["methods"][i]);
		}
	    }
	});    
	return newObject;
    };

})(BluePHP);
