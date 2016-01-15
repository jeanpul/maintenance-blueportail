<?php

include_once("IndicatorInterface.inc");
include_once("IndicatorTitle.inc");
include_once("PeriodTitle.inc");
include_once("PeriodTitleFlowXMLChart.inc");
include_once("TabularMonoValue.inc");
include_once("TabularFlowXMLChart.inc");

class IndicatorVisitors implements IndicatorInterface
{
  protected $params;  
  protected $glang;
  protected $colKeys;
  protected $values;
  protected $counters;
  protected $indicatorTitle;
  protected $displayPeriods = array( 'hour' => array('minute' => "Hour / 1 minute",
						     '5minute' => "Hour / 5 minutes"),
				     'day' => array('hour' => "Day"),
				     'month' => array('day' => "Month"),
				     'year' => array('month' => "Year")
				     );
  protected $entities = array( "location" => "Location", "area" => "Area");
  protected $formats = array ( "graphic" => "Graphic", "tabular" => "Tabular" );
  protected $rfunc = "getCountersIds";
  protected $name = "Visitors";
  protected $key = "unique";

  public function IndicatorVisitors($glang, $params)
    {    
      $this->glang = $glang;
      $this->params = $params;
      $this->colKeys = array( "period", "value1" );

      if(isset($params['Step']) and preg_match('/_/', $params['Step']))
	{
	  $tmp = split('_', $params['Step']);
	  $this->params['CompPeriod'] = $tmp[0];
	  $this->params['Step'] = $tmp[1];
	}

      if(isset($params["displayPeriods"]))
	{
	  $this->displayPeriods = $params["displayPeriods"];
	}
    }

  public function getId()
    {
      return $this->params["Indicator"];
    }

  public function getName()
    {
      return $this->name;
    }

  // FAB: must match the function
  // in Analysis.js
  public function getParamsId()
    {
      return "params_" . $this->getId();
    }

  public function getDataContentsId()
    {
      return "DataContents_" . $this->getId();
    }

  public function getType()
    {
      return $this->params["Analysis"];
    }

  public function setUserConfigParameters($configData)
    {
      $this->params["Step"] = $configData["Step"]["value"];
      $this->params["Entity"] = $configData["Entity"]["value"];
      $this->params["id"] = $configData["id"]["value"];
    }

 


  public function setDefaultParameters()
    {
      global $userEntities;

      $this->params["rfunc"] = $this->rfunc;

      // prend la date courante comme defaut
      if(!isset($this->params["HourValue"]))
	{
	  $this->params["HourValue"] = strftime("%H");
	}
      if(!isset($this->params["DateValue"]))
	{
	  $this->params["DateValue"] = strftime("%Y-%m-%d");
	}
      if(!isset($this->params["TimeStr"]))
      	{
      	  $this->params["TimeStr"] = strftime("%Y%m%d%H%M%S");
	}

      $this->params["timestamp"] = mktime((int) $this->params["HourValue"], 
					  0,
					  0,
					  substr($this->params["DateValue"], 5, 2),
					  substr($this->params["DateValue"], -2, 2),
					  substr($this->params["DateValue"], 0, 4));
      
      // si la periode n'est pas definie
      // ou si la periode souhaitee n'est pas utilisable
      // par cet indicateur alors on en prend une par defaut
    
      if(isset($this->params["CompPeriod"]) and array_key_exists($this->params["CompPeriod"], $this->displayPeriods))
	{
	  $gr = $this->displayPeriods[$this->params["CompPeriod"]];
	  if(isset($this->params["Step"]) and isset($gr[$this->params["Step"]]))
	    {
	      // ok exist
	    }
	  else
	    {  
	      $a = array_keys($gr);
	      $this->params["Step"] = $a[0];
	    }
	}
      else if(isset($this->params["Step"]))
	{
	  $found = false;
	  foreach($this->displayPeriods as $gr => $s)
	    {
	      if($s == $this->params["Step"])
		{
		  $this->params["CompPeriod"] = $gr;
		  $found = true;
		  break;
		}
	    }
	  if(!$found)
	    {
	      $a = array_keys($this->displayPeriods);
	      $this->params["CompPeriod"] = $a[0];	     
	      $a = array_keys($this->displayPeriods[$this->params["CompPeriod"]]);
	      $this->params["Step"] = $a[0];
	    }
	}
      else
	{
	  $a = array_keys($this->displayPeriods);
	      $this->params["CompPeriod"] = $a[0];	     
	      $a = array_keys($this->displayPeriods[$this->params["CompPeriod"]]);
	      $this->params["Step"] = $a[0];
	 
	}

      // si l'entite n'est pas definie
      // ou si l'entite souhaitee n'est pas utilisable
      // par cet indicateur alors on en prend une par defaut
      if(!isset($this->params["Entity"]) or
	 !array_key_exists($this->params["Entity"], $this->entities))
	{
	  $this->params["Entity"] = key($this->entities);
	}

      if(!$this->have_counters())
	{
	  $this->counters = $this->glang->getCountersIds($this->params);
	}

      if($this->have_counters())
	{
	  // si l'identifiant de l'entite n'est pas definie
	  // ou si cet identifiant n'est pas utilisable
	  // alors on en prend un par defaut
	  if(!isset($this->params["id"]) or 
	     !array_key_exists($this->params["id"], $this->counters))
	    {
	      $this->params["id"] = key($this->counters);
	    }
	  
	  $this->params["entName"] = $this->counters[$this->params["id"]]["name"];

	  $this->key = md5(sprintf("%s%s%s%s%s", 
				   $this->params["login"], 
				   $this->params["Analysis"], 
				   $this->params["DateValue"], 
				   $this->params["id"],
				   $this->params["Step"]));
	}

      if(!isset($this->params["Format"]) or
	 !array_key_exists($this->params["Format"], $this->formats))
	{
	  $this->params["Format"] = key($this->formats);
	}

      if(!isset($this->params["Indicator"]))
	{
	  $this->params["Indicator"] = 0;
	}

    }

  protected function computeCountersValues()
    {
      // REPORT PARAMETERS
      if(is_array($this->counters) and count($this->counters))
	{
	  // PROCESS
	  $paramsCounters = $this->params;
	  $paramsCounters["TimeStamp"] = $this->params["timestamp"];
	  $paramsCounters["idp"] = $this->counters[$this->params["id"]]["idp"];

	  $this->values = $this->glang->clang->getVisitorsValues($paramsCounters);
	}
    }

  protected function setAnalysisTitles()
    {
      $this->params["entName"] = $this->counters[$this->params["id"]]["name"];

      $this->indicatorTitle = new IndicatorTitle($this->params["Step"]);      
      $this->indicatorTitle->setGeneralTitle(_($this->name),
					     _("for location"),
					     $this->params["entName"],
					     $this->params["timestamp"]);
      $this->indicatorTitle->setValuesTitles( array( _("Periods"), _($this->name) ) );
    }

  public function getParamsContents()
    {
      global $configData;

      $str = "<div class=\"ParametersBox\" id=\"ParamsBoxContents_" . $this->params["Indicator"] . "\" >\n";
      $str .= "<h3>" . myhtmlentities(_("Indicator parameters")) . " :</h3>\n";
      $str .= "<ul>\n";
      $str .= "<li class=\"Title\">" . myhtmlentities(_("Format")) . " : " 
	. getSelectFromArray("Format", 
			     $this->formats, 
			     $this->params["Format"], 
			     "onchange=\"formatChanged('" . $this->params["Indicator"] . "', this.options[this.selectedIndex].value);\"") 
	. "</li>\n";
      if($configData["EntitiesSelection"]["value"] == "enabled")
	{
	  $str .= $this->getEntitiesParameters();
	}
      $str .= $this->getDateParameters();
      $str .= "</ul>\n";
      $str .= "</div>\n";

      return $str;
    }

  protected function buildSelectForPeriod($name, $default_granularity, $default_step, $extra = "")
    {
      $str = '<select name="' . $name . '" ' . $extra . ">\n";
      foreach($this->displayPeriods as $gr => $ss)
	{	  
	  foreach($ss as $s => $n)
	    {
	      $selected = "";
	      if($gr == $default_granularity and
		 $s == $default_step)
		{
		  $selected = 'selected="selected"';
		}
	      $str .= '<option value="' . $gr . '_' . $s . '" $selected>'. myhtmlentities(_($n)) ."</option>\n";
	    }
	}
      $str .= "</select>\n";
      return $str;
    }

  protected function getStepParameters()
    {
      $str = "<li class=\"Title\">" . myhtmlentities(_("Period")) . " : ";      
      $str .= $this->buildSelectForPeriod( $this->params["Indicator"] + "_Step", 
					   $this->params["CompPeriod"], $this->params["Step"], 
					   "onchange=\"stepChanged('" . $this->params["Indicator"] . "', this.options[this.selectedIndex].value);\"");
      $str .= "</li>\n";
      return $str;
    }

  protected function getEntitiesParameters()
    {
      $str = "<li class=\"Title\">" . myhtmlentities(_("Entities box")) . " :</li>\n";
      $str .= "<li><ul>\n";
      $str .= "<li>" . getSelectFromArray($this->params["Indicator"] . "_Entity", 
					  $this->entities, $this->params["Entity"], 
					  "onchange=\"entityChanged('0', this.options[this.selectedIndex].value);\"") . "</li>\n";
      $str .= "<li>" . $this->getQueryCounters() . "</li>\n";
      $str .= "</ul></li>\n";
      return $str;
    }

  protected function getQueryCounters()
    {
      // counters name/id selection
      $str = '<select onchange="idChanged(\'' . $this->getId() . '\', this.options[this.selectedIndex].value);" name="' . $this->getId() . '_id">' . "\n";
      foreach($this->counters as $idx => $v)
	{
	  $str .= '<option value="' . $v["id"] . '"';
	  if($this->params["id"] == $v["id"])
	    {
	      $str .= " selected=\"selected\"";
	    }
	  $str .= '>' . myhtmlentities($v["name"]) . '</option>' . "\n";
	}
      $str .= "</select>\n";
      return $str;
    }

  protected function getDateParameters()
    {
      $str = "<li class=\"Title\">" . myhtmlentities(_("Date")) . " : " . "<span id=\"DateValueDisplayed_" . $this->getId() . "\">" . $this->params["DateValue"] . "</span>";
      $str .= "<button class=\"calbutton\" id=\"trigger_" . $this->getId() . "\">" . "</button></li>\n";

      // check if hour should be displayed
      if(isset($this->displayPeriods["hour"]))
	{
	  $str .= "<li class=\"Title\">" . myhtmlentities(_("Hour")) . " : " .  $this->getQueryDate() . "</li>\n";
	}
      $str .= $this->getStepParameters();      
      $str .= "</li>\n";
      return $str; 
    }

  protected function getQueryDate()
    {
      $queryStr = getInputHour("HourValue_" . $this->getId(), 
			       $this->params["HourValue"], "onchange=\"hourChanged('" . $this->getId() . "', this.options[this.selectedIndex].value);\"");
      return $queryStr;
    }

  protected function changePeriodsTitle($pt)
    {
      for($i = 0; $i < count($this->values); $i++)
	{
	  $this->values[$i]["period"] = $pt->getPeriodTitle($this->values[$i]);
	}      
    }

  protected function getGraphicContents()
    {
      $this->changePeriodsTitle(new PeriodTitleFlowXMLChart($this->params["Step"]));

      $tabular = new TabularFlowXMLChart(array( "", $this->indicatorTitle->getSubTitle() ), 
					 $this->values, 
					 $this->colKeys);      

      return getGraphicalBox($this->getId(), 
			     $this->indicatorTitle->getGeneralTitle(), 
			     getGraphicPairTable($tabular->getData(array("id" => 0, "key" => $this->key)), 
						 ""));
    }

  protected function getCSVContents()
    {
      $this->changePeriodsTitle(new PeriodTitle("RAW"));

      $tabular = new TabularMonoValue($this->indicatorTitle->getValuesTitles(),
				      $this->values, 
				      $this->colKeys);
      $tabular->setRepresentation("csv");
      $strToCSV = $tabular->toString();

      return $strToCSV;
    }

  public function getURLInputParameters()
    {
      $urlParams = "";
      $firstParam = true;
      foreach($this->params as $k => $v)
	{
	  if(!$firstParam)
	    {
	      $urlParams .= "&";
	    }
	  else
	    {
	      $firstParam = false;
	    }
	  $urlParams .= $k . "=" . $v;
	}
      return $urlParams;
    }

  public function getInputParameters()
    {
      $str = "<input type=\"hidden\" name=\"login\" value=\"" . $this->params["login"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"access\" value=\"" . $this->params["access"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"Indicator\" value=\"" . $this->params["Indicator"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"Step\" value=\"" . $this->params["Step"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"Analysis\" value=\"" . $this->params["Analysis"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"Format\" value=\"" . $this->params["Format"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"Entity\" value=\"" . $this->params["Entity"] . "\"></input>\n";
      if(isset($this->params["id"]))
	{
	  $str .= "<input type=\"hidden\" name=\"id\" value=\"" . $this->params["id"] . "\"></input>\n";
	}
      $str .= "<input type=\"hidden\" name=\"DateValue\" value=\"" . $this->params["DateValue"] . "\"></input>\n";
      $str .= "<input type=\"hidden\" name=\"HourValue\" value=\"" . $this->params["HourValue"] . "\"></input>\n";
      return $str;
    }

  protected function getTabularContents()
    {
      $this->changePeriodsTitle(new PeriodTitle($this->params["Step"]));

      $tabular = new TabularMonoValue($this->indicatorTitle->getValuesTitles(),
				      $this->values, 
				      $this->colKeys);
      
      return getTabularBox($this, 
			   $this->indicatorTitle->getGeneralTitle(), 
			   $tabular->getData());
    }

  public function getDataContents()
    {
      if($this->have_counters())
	{
	  $this->computeCountersValues();
	}
      $this->setAnalysisTitles();     

      if(!$this->have_values())
	{	  
	  return $this->getEmptyDataReport($this->params, 
					   $this->indicatorTitle->getGeneralTitle());
	}
      else
	{
	  $str = "";

	  if($this->params["Format"] == "graphic")
	    {
	      $str .= $this->getGraphicContents();
	    }
	  else if($this->params["Format"] == "tabular")
	    {
	      $str .= $this->getTabularContents();
	    }
	  else if($this->params["Format"] == "csv")
	    {	      
	      return $this->getCSVContents();
	    }
	  return $str;
	}
    }

  public function setParameter($key, $value)
    {
      $this->params[$key] = $value;
    }

  private function have_counters()
    {
      return isset($this->counters) and is_array($this->counters) and count($this->counters) > 0;
    }

  private function have_values()
    {
      return isset($this->values) and is_array($this->values) and count($this->values) > 0;
    }

  protected function getEmptyDataReport()
    {
      if($this->params["Format"] != "csv")
	{
	  $str = "<h1>" . myhtmlentities($this->indicatorTitle->getGeneralTitle()) . "</h1>\n";
	  $str .= "<p class=\"nodata\">".myhtmlentities(_("No counting data available for this period."))."</p>\n";
	  return $str;
	}
      else
	{
	  return "";
	}
    }

  public function getAvailableSteps()
    {
      $steps = array();
      foreach($this->displayPeriods as $gr => $ss)
	{
	  foreach($ss as $s => $n)
	    {
	      $steps[$gr . '_' . $s] = $n;
	    }
	}
      return $steps;
    }

  public function getAvailableEntities()
    {
      return $this->entities;
    }
  
  public function getEntitiesList($entity)
    {
      $rfunc = $this->rfunc;
      return $this->glang->clang->$rfunc(array("Entity" => $entity));
    }
}

?>
