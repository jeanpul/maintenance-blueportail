<?php

include_once("BluePHP/BlueCountGUI/BCGUI_ModuleProcessing.inc");

class Panels extends BCGUI_ModuleProcessing
{
  function __construct($sharedCon = null)
  {
    parent::__construct(array( "name" => "Panels",
			       "keyname" => "id",
			       "sort" => "idd" ),
			$sharedCon);
  }

  public function setEntry($params)
  {
    $params = array_merge( array( "isActive" => "false" ),
			   $params );
    parent::setEntry($params);
    $this->setParamsEntry($params);
    return $params;
  } 

  public function getEntry($params)
  {
    $params = parent::Select("Panels.*, description as size, Dashboards.class as theme, " . 
			     "(select max(position) from Panels as PP " . 
			     "where PP.idd=Panels.idd) as maxPos from Panels, " . 
			     "PanelClasses, Dashboards where Panels.id=" . $params["id"] .
			     " and Dashboards.id=Panels.idd" . 
			     " and Panels.class=PanelClasses.class");
    $params = $params[0];
    return array_merge( $params, $this->getParamsEntry($params) );
  } 

  public function setPanelAtPos($params)
  {
    $params = array_merge( array( "isActive" => "false" ),
			   $params );
    $p = $this->getEntry($params);
    if($p["position"] < $params["position"])
      { // move the panel to the right so move neighboors to the left until its new position
	parent::QueryNoReturn("update Panels set position=position-1 where position >" . $p["position"] . 
			      " and position <=" . $params["position"] . " and idd=" . $params["idd"]);
      }
    else if($p["position"] > $params["position"])
      { // move the panel to the left so move neighboors to the right until its new position
	parent::QueryNoReturn("update Panels set position=position+1 where position >=" . $params["position"] . 
			      " and position <" . $p["position"] . " and idd=" . $params["idd"]);	    
      }
    $params = array_merge($p, $params);
    return $this->setEntry($params);
  }
  
  public function switchPanelPos($params)
  {
    // retrieve current position
    $p = $this->getEntry($params);
    if($p["position"] != $params["position"])
      { // retrieve the panel that has the position wanted
	$panelDst = $this->getPanelAtPos( array( "idd" => $params["idd"],
						 "position" => $params["position"] ) );

	if(is_array($panelDst) and count($panelDst))
	  {
	    $panelDst = $panelDst[0];
	    // switch position for both panels
	    parent::QueryNoReturn("update Panels set position=" . $params["position"] .  
				  " where idd=" . $params["idd"] . " and id=" . $params["id"]);
	    parent::QueryNoReturn("update Panels set position=" . $p["position"] .  
				  " where idd=" . $params["idd"] . " and id=" . $panelDst["id"]);
	  }
      }
    return array();
  }

  public function getPanelAtPos($params)
  {
    return parent::Select("Panels.*, description as size, (select max(position) from Panels) as maxPos from Panels, PanelClasses where idd=" . $params["idd"] .
			  " and position=" . $params["position"] . " and Panels.class=PanelClasses.class");
  }
  
  public function addPanelAtPos($params)
  {
    // check if a panel already exist at this position
    // if so then move one step after the other panels
    $p = $this->getPanelAtPos($params);
    if(is_array($p) and count($p))
      {
	parent::QueryNoReturn("update Panels set position=position+1 where position >=" . $params["position"] . " and idd=" . $params["idd"]);
      }

    parent::QueryNoReturn("insert into Panels values(null," . $params["idd"] . 
    			  ",'" . $params["class"] . "','" . $params["category"] . "','true'," . $params["position"] . ")");
    // retrieve the panel id to be used for the panel config
    $p = $this->getPanelAtPos($params);
    if(is_array($p) and count($p))
      {
	$p = $p[0];
	$this->newParamsEntry( array_merge($p, $params) );
      }
    return $p;
  }

  /**
   * Remove a Panel at a specific position.
   * then re-order other panels so there is no
   * gap that is each position is increased by 1
   * \param $params panel "position" and dashboard "idd"
   */
  public function removePanelAtPos($params)
  {
    $p = $this->getPanelAtPos($params);
    if(is_array($p) and count($p))
      {
	$p = $p[0];
	parent::QueryNoReturn("delete from Panels where idd=" . $params["idd"] . " and position=" . $params["position"]);
	parent::QueryNoReturn("update Panels set position=position-1 where position > " . $params["position"] . " and idd=" . $params["idd"]);
	$this->removeParamsEntry($p);
      }
    return $params;
  }

  /**
   * Returns an array containing the
   * list of category name. Actually,
   * colors are used for classify different
   * category so you can render then Panel
   * using the category color for example.
   */
  public function getCategories()
  {
    return array( "white", "blue", "red", "green", "yellow", "purple" );
  }

  protected function getParamsEntry($params)
  {
    $panelElt = $this->getConfigClass($params);
    return $panelElt->getEntry($params);
  }

  protected function removeParamsEntry($params)
  {
    $panelElt = $this->getConfigClass($params);
    return $panelElt->delEntry($params);
  }

  protected function setParamsEntry($params) 
  {
    $panelElt = $this->getConfigClass($params);    
    return $panelElt->setEntry($params);    
  }

  protected function newParamsEntry($params) 
  {
    $panelElt = $this->getConfigClass($params);    
    return $panelElt->newEntry($params);    
  }

  protected function getConfigClass($params)
  {
    $className = $params["class"] . "Config";
    include_once("BluePHP/BlueCountGUI/Dashboards/" . $className . ".inc");
    return new $className();
  }
}

?>