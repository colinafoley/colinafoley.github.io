<?php

class WorkNodeMigration extends Migration {
  /**
   * The source and destination content types (bundles) we're dealing with.
   */
  protected $destinationType;

  /**
   * The source database key defined in settings.php.
   */
  protected $sourceConnection;

  public function __construct(array $arguments) {
    $this->destinationType = $arguments['destination_type'];
    $this->sourceConnection = $arguments['source_connection'];

    parent::__construct($arguments);

    $this->sourceFields = array(
      'uid' => t('Default uid set in prepareRow()'),
      'created' => t('Default created set in prepareRow()'),
      'changed' => t('Default changed set in prepareRow()'),
      'date' => t('Default date set in prepareRow()'),
      'date2' => t('Default date set in prepareRow()'),
      'circa' => t('Default circa set in prepareRow()'),
      'season' => t('Default circa set in prepareRow()'),
      'title' => t('Default title set in prepareRow()'),
      'Author' => t('Author'),
      'Editor' => t('Editor'),
      'Illustrator' => t('Illustrator'),
      'Title' => t('Title'),
      'Work' => t('Work'),
      'Vol' => t('Volume'),
      'volume' => t('Default volume set in prepareRow()'),
      'Iss' => t('Issue'),
      'issue' => t('Default issue set in prepareRow()'),
      'Pub' => t('Publication'),
      'Month' => t('Month'),
      'Day' => t('Day'),
      'Year' => t('Year'),
      'Pages' => t('Page numbers'),
      'Col' => t('Colum'),
      'Type' => t('Type'),
      'Genre' => t('Genre'),
      'Description' => t('Description'),
      'LinkID' => t('Link ID'),
      'Code' => t('Code'),
      'Notes' => t('Desctription'),
    );
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));

    $this->esource_map = $this->getESourceMap();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'workID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source record',
          'alias' => 'wks',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode($this->destinationType);
      $this->addSimpleMappings(array('created', 'changed', 'uid', 'title'));
      $this->addFieldMapping('field_work_circa', 'circa');
      $this->addFieldMapping('field_work_season', 'season');
      $this->addFieldMapping('field_work_date', 'date');
      $this->addFieldMapping('field_work_date:value2', 'date2');
      $this->addFieldMapping('field_work_author', 'Author');
      $this->addFieldMapping('field_work_editor', 'Editor');
      $this->addFieldMapping('field_work_illustrator', 'Illustrator');
      $this->addFieldMapping('field_work_publication_title', 'Work');
      $this->addFieldMapping('field_work_article_title', 'Title');
      $this->addFieldMapping('field_work_volume', 'volume');
      $this->addFieldMapping('field_work_issue', 'issue');
      $this->addFieldMapping('field_work_publisher', 'Pub');
      $this->addFieldMapping('field_work_description', 'Description');
      $this->addFieldMapping('field_work_description:format', 'full_html');
      //->arguments(array('format' => 'full_html'));
      $this->addFieldMapping('field_work_pages', 'Pages');
      $this->addFieldMapping('field_work_column', 'Col');
      $this->addFieldMapping('field_work_type', 'Type');
      $this->addFieldMapping('field_work_genre', 'genre');
      $this->addFieldMapping('field_work_link', 'Link');
      $this->addFieldMapping('field_work_linkid', 'LinkID');
      $this->addFieldMapping('field_work_code', 'Code');
      $this->addFieldMapping('field_work_administrative_note', 'Notes');
      $this->addFieldMapping('field_work_administrative_note:format', 'full_html');
      //->arguments(array('format' => 'full_html'));
      
    
    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language', 'sticky', 'revision_uid', 'path'));
  }

  function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('works', 'wks')
      ->fields('wks');
    return $query;
  }

  function getESourceMap() {
      $result = db_select('migrate_map_pfaffse_source', 'esourcemap')
      ->fields('esourcemap', array('sourceid1', 'destid1'))
      ->execute()->fetchAll();
    $source_map = array();
    foreach ($result as $record) {
      $source_map[$record->sourceid1] = $record->destid1;
    }
    return $source_map;
  }

  function prepareRow($current_row) {
    if (parent::prepareRow($current_row) === FALSE) {
      return FALSE;
    }
    $current_row->created = $current_row->changed = time();
    $current_row->full_html = 'full_html';
    
    $current_row->uid = 1;
    if ($current_row->Title && $current_row->Title <> ""){
      $current_row->title = $current_row->Title;
    }
    else{
      $current_row->title = $current_row->Work;
    }
    $circa = 0;

    $current_row->LinkID = $this->esource_map[$current_row->linkID];
    ///create clean array from the incoming string
    $genre1= str_replace("  ", " ", $current_row->Genre);
    $genre2= str_replace(", ", ",", $genre1);
    $genre3= str_replace("; ", ",", $genre2);
    $genre4= str_replace("/", ",", $genre3);
    $genreOut = explode(",", $genre4);
    $current_row->genre = $genreOut;

    $season = '';
      switch ($current_row->Month) {
        case 'April':
          $month = '04';
          break;
        case 'August':
          $month = '08';
          break;
        case 'Dec':
          $month = '12';
          break;
        case 'Dec.':
          $month = '12';
          break;
        case 'December':
          $month = '12';
          break;
        case 'Feb':
          $month = '02';
          break;
        case 'Feb.':
          $month = '02';
          break;
        case 'February':
          $month = '02';
          break;
        case 'Jan':
          $month = '01';
          break;
        case 'January':
          $month = '01';
          break;
        case 'January 1 - June 25':
          $month = '01';
          //$season = $current_row->Month;
           $season = "Winter/Spring";
          break;
        case 'January-June':
          $month = '01';
          //$season = $current_row->Month;
          $season = "Winter/Spring";
          break;
        case 'July':
          $month = '07';
          break;
        case 'July-August':
          $month = '07';
          //$season = $current_row->Month;
          $season = "Summer";
          break;
        case 'June':
          $month = '06';
          break;
        case 'March':
          $month = '03';
          break;
        case 'March/April':
          $month = '03';
          //$season = $current_row->Month;
          $season = "Spring";
          break;
        case 'May':
          $month = '05';
          break;
        case 'Nov.':
          $month = '11';
          break;
        case 'Novebmer':
          $month = '11';
          break;
        case 'November':
          $month = '11';
          break;
        case 'Ocotober':
          $month = '10';
          break;
        case 'Oct':
          $month = '10';
          break;
        case 'October':
          $month = '10';
          break;
        case 'September':
          $month = '09';
          break;
        case 'Spring':
          $month = '04';
          $season = $current_row->Month;
          break;
        case 'Summer':
          $month = '06';
          $season = $current_row->Month;
          break;
        case 'Summer/Fall':
          $month = '06';
          $season = $current_row->Month;
          break;
        case 'unknown':
          $month = null;
          $season = $current_row->Month;
          break;
        case 'Winter':
          $month = '01';
          $season = $current_row->Month;
          break;
        case 'Winter/Spring':
          $month = '01';
          $season = $current_row->Month;
          break;
        default:
        $month = null;
      }
    $day = null;
    if ($current_row->Day && $current_row->Day <> ""){
      if (strlen($current_row->Day) === 2){
        $day=$current_row->Day;
      }
      else if (strlen($current_row->Day) > 2){
        $day=substr($current_row->Day, 0, 2);
      }
      else{
        $day= '01' . $current_row->Day;
      }
    }

    $year = null;
    $yearstring = $current_row->Year;
    if ($yearstring || $yearstring <> ""){
      ///try to clean it up
      $yearstring=str_replace("?", "", $yearstring);
      $yearstring=str_replace("/", "-", $yearstring);
      $yearstring=str_replace(", ", "-", $yearstring);
      $yearstring=str_replace(",", "-", $yearstring);
      $circa = 0;
      switch ($yearstring) {
        case 'c. 1861-62':
          $yearstring = '1861-1862';
          $circa = 1;
          break;
        case 'circa 1872':
          $yearstring = '1872';
          $circa = 1;
          break;
        case 'udated':
          $yearstring = null;
          $circa = 1;
          break;
        case 'undated':
          $yearstring = null;
          $circa = 1;
          break;
        case 'Various Dates':
          $yearstring = null;
          $circa = 1;
          break;
        default:
        break;
      } 
    }  
    $yearend = "";
    if ($yearstring || !empty($yearstring)|| $yearstring<>""){
      $yeararray = explode("-", $yearstring);
        if (count($yeararray > 1)){
          //get start and end year...
          $year=intval($yeararray[0]);
          $yearend=intval($yeararray[1]);
        }
    }
    else {
      //treat it as length of 1 convert to integer
      $year=intval($yeararray[0]);
    }
    
    if (!$day || empty($day)){
      $circa = 1;
      $day = '01';
    }
    if (!$month || empty($month)){
      $circa = 1;
      $month = '01';
    }
    if (!$yearend || empty($yearend)){
    $todatestring = "";  
    }
    else{
      $todatestring = strval($yearend) .'-'.strval($month) . "-" .strval($day) . " 00:00:00";
    }
    if (!$year || empty($year)){
      $fromdatestring = "";
    }
    else{
     $fromdatestring = strval($year) .'-'.strval($month) . "-" .strval($day) . " 00:00:00";
    }

    $current_row->season = $season;
    $current_row->circa = $circa; 
    $current_row->date = $fromdatestring;
    $current_row->date2 = $todatestring;

    ///working with vol
    switch ($current_row->Vol) {
      case 'I':
        $current_row->volume = 1;
        break;
      case 'II':
        $current_row->volume = 2;
        break;
      case 'III':
        $current_row->volume = 3;
        break;
      case 'IV':
        $current_row->volume = 4;
        break;
      case 'LVIII':
        $current_row->volume = 58;
        break;
      case 'V':
        $current_row->volume = 5;
        break;
      case 'VI':
        $current_row->volume = 6;
        break;
      case 'VII':
        $current_row->volume = 7;
        break;
      case 'VIII':
        $current_row->volume = 8;
        break;
      case 'X':
        $current_row->volume = 10;
        break;
      case 'XCII':
        $current_row->volume = 92;
        break;
      case 'XCIV':
        $current_row->volume = 94;
        break;
      case 'XCV':
        $current_row->volume = 95;
        break;
      case 'XCVI':
        $current_row->volume = 96;
        break;
      case 'XXIX':
        $current_row->volume = 29;
        break;
      case 'XXX':
        $current_row->volume = 30;
        break;
      case 'American Men of Letters Series':
        $current_row->volume = 0;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Vol;
        break;
      case 'Household edition':
        $current_row->volume = 0;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Vol;
        break;
      case '3 Vols.':
        $current_row->volume = 3;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Vol;
        break;
      case '10 Vols.':
        $current_row->volume = 10;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Vol;
        break;
      default:
        try {
          $current_row->volume = $current_row->Vol;
        } catch (Exception $e) {
          $current_row->volume = -99;
        }
        break;
    }
  //working with Iss
    switch ($current_row->Iss) {
      case 'multiple':
        $current_row->issue = 0;
        $current_row->Title = $current_row->Title. ' multiple issues' ;
        break;
      case 'University of Colorado studies. Series B. Studies':
        $current_row->issue = 0;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Iss;
        break;
      case 'New York: Hurd and Houghton':
        $current_row->issue = 0;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Iss;
        break;
      case 'Document No. 9':
        $current_row->issue = 9;
        $current_row->Title = $current_row->Title. ' ' . $current_row->Iss;
        break;
      case 'CXCVIII':
        $current_row->issue = 198;
        break;
      case 'CXCVII':
        $current_row->issue = 197;
        break;
      case 'CXCV':
        $current_row->issue = 195;
        break;
      case 'CXCIV':
        $current_row->issue = 194;
        break;
      case '40-65':
        $current_row->issue = 40;
        $current_row->Title = $current_row->Title. ' Issues ' . $current_row->Iss;
        break;
      case '85-86':
        $current_row->issue = 85;
        $current_row->Title = $current_row->Title. ' Issues ' . $current_row->Iss;
        break;

      default:
        try {
          $current_row->issue = $current_row->Iss;
        } catch (Exception $e) {
          $current_row->issue = -99;
          $current_row->Title = $current_row->Title. ' ' . $current_row->Iss;
        }
        break;
    }
  }//end-prepareRow

}//end-WorkNodeMigration
