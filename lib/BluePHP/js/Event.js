function ChangeValueOnCheck(idCheckBox, idDst, valueOn, valueOff)
{
    var elt = document.getElementById(idCheckBox);
    var dst = document.getElementById(idDst);
    dst.value = elt.checked ? valueOn : valueOff;
}

function HideShowElt(button, id, htmlShow, htmlHide)
{
  var elt = $('#' + id);
  if(elt.is(":hidden")) {
    elt.show();
    button.innerHTML = htmlHide;
  } else {
    elt.hide();
    button.innerHTML = htmlShow;
  }
}
