<?php

function Forms_inputText($var, $value = false)
{
  $str = '<input type="text" name="'.$var.'"';
  if($value){
    $str .= ' value="'.$value.'"';
  }
  $str .= "/>\n";
  return $str;
}

function Forms_inputHidden($var, $value)
{
  return '<input type="hidden" name="' . $var . '" value="' . $value . '"/>' . "\n";
}

function Forms_multipleInputHidden($varvals)
{
  $str = '';
  foreach($varvals as $k => $v)
    {
      $str .= Forms_inputHidden($k,$v);
    }
  return $str;
}

function Forms_inputSelect($var, $elts)
{
  $str = '<select name="' . $var . '">';
  foreach($elts as $k => $v)
    {
      $str .= '<option value="'.$k.'">'.$v.'</option>';
    }
  $str .= '</select>' . "\n";
  return $str;
}

function Forms_inputCheckBox($name, $checked)
{  
  $str = '<input type="checkbox" name="' . $name . '"';
  if($checked)
    {
      $str .= ' checked="checked"';
    }
  $str .= '/>';
  return $str;
}

function Forms_submitButton($button_name)
{
  return '<button type="submit" value="' . $button_name . '" >' . $button_name . '</button>' . "\n";
}

function Forms_formGet($url, $form_elts)
{
  $str .= '<form action="' . $url . '" method="get">' . "\n";
  $str .= $form_elts;             
  $str .= '</form>' . "\n";
  return $str;
}

?>