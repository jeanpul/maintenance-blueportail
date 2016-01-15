//
// @file BlueCountLang.js
// @author fabien.pelisson@blueeyevideo.com
//
// AJAX communication with BlueCountLanguage
// YOU MUST INCLUDE XmlHttp.js BEFORE THIS

// create one global element 
// CHECK THAT A XmlHttp request is finished BEFORE
// CALLING ANOTHER.
xmlHttp = getXmlHttpObject();
if(xmlHttp == null)
{  
  alert("Your Web browser does not have full functionalities to support our software product. Please contact info@blueeyevideo.com.");
}

function BlueCountLang(url, id, cb)
{	
  xmlHttp = getXmlHttpObject();
  xmlHttp.onreadystatechange = function()
    {
      if(xmlHttp.readyState == 4)
      {
	cb(id, xmlHttp);
      }
    }
  xmlHttp.open("GET", url, true);
  xmlHttp.send(null);
}

// Parse an XML response from a XmlHttp request,
// create an options list and replace the inner HTML
// of a specified HTML element id (which is of course a select element).
// Set the first option element to selected.
// The XML format must be in the form :
// <array>
// <key>TEXT</key> 
// <array>
// ...
// <id>ID1</id>
// <name>NAME1</name>
// ...
// </array>
// <array>
// <id>ID2</id>
// <name>NAME2</name>
// ...
// </array>
// ...
// </array>
// Then only the id and name tags will be used as following :
// <option value="ID1">NAME1</option>
// <option value="ID2">NAME2</option>
// ...
function BlueCountLang_ChangeSelect(id, xmlHttp)
{
  var root = xmlHttp.responseXML;
  
  var idElts = root.documentElement.getElementsByTagName("id");
  var nameElts = root.documentElement.getElementsByTagName("name");
  var selectInnerHTML = "";
  for(i = 0; i < idElts.length; i++)
    {
      selectInnerHTML += "<option value=\"" + idElts.item(i).firstChild.data + "\">" + nameElts.item(i).firstChild.data + "</option>";
    }
  var selectElt = document.getElementsByName(id)[0];
  selectElt.innerHTML = selectInnerHTML;
}
