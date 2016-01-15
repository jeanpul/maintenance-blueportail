<?php

include_once("BluePHP/BlueProjectPHP/Config.inc");
include_once("BluePHP/Utils/ModuleProcessing.inc");
include_once("BluePHP/BAM/BAMHttpClient.inc");
include_once("CommonOps.inc");
include_once("FamillyProjects.inc");

class Projects extends ModuleProcessing
{
  public function __construct($sharedCon = false)
  {
    parent::__construct(array( "name" => "projects",
			       "keyname" => "project_id",
			       "sort" => "project_name" ),
			$sharedCon);
  }

  function getCategories($params)
  {
    return parent::Select("project_cat_id as id, project_cat_id as name from projects_cat " .
			  "where project_id=" . $params["action_project"] . " order by project_cat_id");
  }

//  function getSuivis($params = array())
//  {
//    return parent::Select("projects.project_id as id, concat('(',projects.project_id,') ', project_name) as project_name from projects, livraison, products " .
//			  "where livraison.project_id = projects.project_id and " .
//			  "livraison.product_id = products.product_id order by projects.project_id desc");
//  }

  function getSuivis($params = array())
  {
    return parent::Select("projects.project_id as id, concat('(',projects.project_id,') ', project_name) as project_name from projects, familly_projects " .
			  "where familly_projects.project_id = projects.project_id and " .
			  "familly_projects.familly_id=2 order by projects.project_id desc");
  }

  function getProducts($params)
  {
    return parent::Select("project_product.*, products.* from project_product, products where project_product.project_id=" . $params["project_id"] . 
			  " and project_product.product_id=products.product_id");
  }

  function newEntry($params)
  {
    $ret = parent::newEntry($params);
    $last = parent::Select("max(project_id) as project_id from projects");
    $params = array_merge( $params, 
			   $last[0] );
    $fam = new FamillyProjects();
    $fam->newEntry($params);

    $projectBase = "/net/pong/data/Projets/";
    $projectDir = $projectBase . $params["project_id"];
    // gives all rights to the group (blueeyevideo)
    $rights = 0775;
    mkdir($projectDir, $rights); 

    $this->create_planning($projectBase, $projectDir, $rights);
    
    mkdir($projectDir . "/Updates", $rights);
//    mkdir($projectDir . "/Devis", $rights);
//    mkdir($projectDir . "/Propales", $rights);
//    mkdir($projectDir . "/Commandes", $rights);
    mkdir($projectDir . "/Configs", $rights);
    mkdir($projectDir . "/Plans", $rights);
    mkdir($projectDir . "/Vues", $rights);
    mkdir($projectDir . "/Rapports", $rights);
//    mkdir($projectDir . "/Achats", $rights);
//    mkdir($projectDir . "/Livraison", $rights);
//    mkdir($projectDir . "/Facture", $rights);
    mkdir($projectDir . "/SAV", $rights);
    mkdir ($projectDir . "/Gestion Commerciale", $rights);
    mkdir ($projectDir . "/Gestion Commerciale/1-Devis", $rights);
    mkdir ($projectDir . "/Gestion Commerciale/2-Commandes", $rights);
    mkdir ($projectDir . "/Gestion Commerciale/3-Achats", $rights);
    mkdir ($projectDir . "/Gestion Commerciale/4-Facture", $rights);
    mkdir ($projectDir . "/Gestion Commerciale/5-Livraison", $rights);
    mkdir ($projectDir . "/Gestion Commerciale/6-Paiement", $rights);

    // send BAM msg
    $bam = new BAMHttpClient(array("host" => "localhost"));
    $bam->Msg_info("bev.wiki.projects", "Project " . $params["project_name"] . " created with id " .
    $params["project_id"]);

    //    sendEmail("Project created (" . $params["project_name"] . ")",
    //	      "Creation of a new project with project id = " . $params["project_id"] .
    //	      " and familly = " . $params["familly_id"]);
    return $ret;
  }

  private function create_planning($projectBase, $projectDir, $rights)
  {
    mkdir($projectDir . "/Planning", $rights);
    
    // copy main.tji, tasks.tji and taskreports.tji
    copy($projectBase . "196/template/main.tjp", $projectDir . "/Planning/main.tjp");
    copy($projectBase . "196/template/tasks.tji", $projectDir . "/Planning/tasks.tji");
    copy($projectBase . "196/template/taskreports.tji", $projectDir . "/Planning/taskreports.tji");
  }

  function setEntry($params)
  {
    $fam = new FamillyProjects();
    $fam->setEntry($params);
    return parent::setEntry($params);
//
//    sendEmail("Project updated (" . $params["project_name"] . ")",
//	      "Modification of an existing project with project id = " . $params["project_id"] .
//	      " and familly = " . $params["familly_id"]);
  }

  function delEntry($params)
  {
    $fam = new FamillyProjects();
    $fam->delEntry($params);
    return parent::delEntry($params);
  }

}



?>
