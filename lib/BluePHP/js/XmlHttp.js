//
// @file XmlHttp.js
// @author fabien.pelisson@blueeyevideo.com
// Used/Included by any AJAX programs
// 

var xmlHttp;

// @see http://www.w3schools.com/ajax/ajax_source.asp
function getXmlHttpObject()
{
    var xmlHttp = null;
    try
    {
	// Firefox, Opera 8.0+, Safari
	xmlHttp = new XMLHttpRequest();
    }
    catch(e)
    {
	// Internet Explorer
	try
	{
	    xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
	}
	catch(e)
	{
	    xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
    }
    return xmlHttp;
}
