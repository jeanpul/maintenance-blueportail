<?php

include_once("BluePHP/Utils/Input.inc");
include_once("BluePHP/BTopLocalServer/ZoneCounting.inc"); 
include_once("BluePHP/BlueCountGUI/Dashboards/PanelHTML.inc");

/**
 * \class LOSRatingHTML
 */
class LOSRatingHTML extends PanelHTML
{
  public function __construct($id, $params)
  {
    parent::__construct($id, $params);
  }

  protected function getTableInput($params)
  {
    $str = parent::getTableInput($params);

    $params = array_merge( array( "label" => "DEFAULT",
				  "numId" => null ),
			   $params );

    $str .= "<table>";

    $str .= createTableRow( array( _("Label"),
				   createInputText($params["label"],
						   array( "name" => "label",
							  "maxlength" => 64 ))));
    $module = new ZoneCounting();
    
    $str .= createTableRow(array( _("Location/Area"),
				  createSelect( $module->getEntries(),
						array( "key" => "id",
						       "name" => "numId",
						       "value" => "nameZ",
						       "default" => $params["numId"],
						       "extra" => false ))
				  )
			   );
    $str .= "</table>";

    return $str;
  }

}

?>