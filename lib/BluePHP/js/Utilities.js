//
// @file BlueCountLang.js
// @author fabien.pelisson@blueeyevideo.com
//
// Some functions commonly used in different applications

/**
 * This array store the initial value of the display
 * property so we can restore this value after a switch
 * to none
 */
var EltsInitialSize = new Array();

// @author fabien.pelisson@blueeyevideo.com
// switch element between block <=> none
function switchVisible(id)
{
  var elt = document.getElementById(id);  
  if(!EltsInitialSize[id])
    {
      EltsInitialSize[id] = elt.style.display ? elt.style.display : "block";
    }
  elt.style.display = elt.style.display == "none" ? EltsInitialSize[id] : "none";
};

function setVisible(id, sizeW, sizeH, value)
{
  var elt = document.getElementById(id);
  if(!EltsInitialSize[id])
    { // remember the initial value
      EltsInitialSize[id] = new Array(elt.style.width, elt.style.height);
    }
  elt.style.visibility = value ? "visible" : "hidden";
  elt.style.display = value ? "block" : "none";
  if(sizeW)
    {
      elt.style.width = value ? EltsInitialSize[id][0] : "0px";
    }
  if(sizeH)
    {
      elt.style.height = value ? EltsInitialSize[id][0] : "0px";
    }
}

/**
 * Find the absolute x,y position of the given object.
 * FAB : I found this code on the web, I must retrieve the author name
 */
function findPos(obj) 
{
  var curleft = curtop = 0;
  if (obj.offsetParent) {
    curleft = obj.offsetLeft
      curtop = obj.offsetTop
      while (obj = obj.offsetParent) {
	curleft += obj.offsetLeft
	curtop += obj.offsetTop
      }
  }
  return [curleft,curtop];
}


