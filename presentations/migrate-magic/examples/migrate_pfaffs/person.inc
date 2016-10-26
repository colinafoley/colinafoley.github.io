<?php

class PersonNodeMigration extends Migration {
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
      'title' => t('Default title set in prepareRow()'),
      'peopleID' => t('Source id'),
      'Last' => t('Last Name'),
      'First' => t('First Name'),
      'Middle' => t('Middle Name'),
      'Title' => t('Title'),
      'Last2' => t('Last Name2'),
      'First2' => t('First Name2'),
      'Middle2' => t('Middle Name2'),
      'Title2' => t('Title2'),
      'othername'=> t('Default othername FC set in prepareRow()'),
      'Gender' => t('Gender'),
      'Birth' => t('Birth'),
      'Death' => t('Death'),
      'birthyear' => t('Default birth FC set in prepareRow()'),
      'deathyear' => t('Default death set in prepareRow()'),
      'birthuncertain' => t('Default birthuncertain set in prepareRow()'),
      'deathuncertain' => t('Default deathuncertain set in prepareRow()'),      
      'Birthplace' => t('Place of Birth'),
      'Profession' => t('Profession'),
      'Association' => t('Association'),
      'Press' => t('Press'),
      'Images' => t('Number of Images'),
      'image_list' => t('Default images set in prepareRow()'),
      'Description' => t('Desctription'),
      'Sources' => t('Spources'),
    );
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'peopleID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source record',
          'alias' => 'ppl',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode($this->destinationType);
      $this->addSimpleMappings(array('title', 'created', 'changed', 'uid'));
      $this->addFieldMapping('field_person_last_name', 'Last');
      $this->addFieldMapping('field_person_first_name', 'First');
      $this->addFieldMapping('field_person_middle_name', 'Middle');
      $this->addFieldMapping('field_person_gender', 'Gender');
      $this->addFieldMapping('field_person_birth_place', 'Birthplace');
      $this->addFieldMapping('field_person_profession', 'profession');
      $this->addFieldMapping('field_person_association', 'Association');
      $this->addFieldMapping('field_person_press', 'Press');
      $this->addFieldMapping('body', 'Description');
      $this->addFieldMapping('body:format', 'full_html');
       // ->arguments(array('format' => 'full_html'));
      $this->addFieldMapping('field_person_admin_note', 'Sources');
      $this->addFieldMapping('field_person_admin_note:format', 'full_html');
      //->arguments(array('format' => 'full_html'));
      $this->addFieldMapping('field_person_honorific_name', 'Title');
      $this->addFieldMapping('field_person_other_name', 'othername');
      $this->addFieldMapping('field_person_image', 'image_list');
      $this->addFieldMapping('field_person_image:file_class')
        ->defaultValue('MigrateFileUri');
      $this->addFieldMapping('field_person_image:language')
        ->defaultValue('el');
      $this->addFieldMapping('field_person_image:destination_dir')
        ->defaultValue('public://pfaffs-persons');
      $this->addFieldMapping('field_person_image:file_replace')
        ->defaultValue('FILE_EXISTS_REPLACE');
      $this->addFieldMapping('field_person_image:preserve_files')
        ->defaultValue(FALSE);
      $this->addFieldMapping('field_person_image:source_dir')
        ->defaultValue('http://digital.lib.lehigh.edu/pfaffs/images');
      $this->addFieldMapping('field_person_image:alt')
        ->defaultValue('Historic Photograph');
      $this->addFieldMapping('field_person_image:title')
        ->defaultValue('Historic Photograph');
      $this->addFieldMapping('field_person_birth_year', 'birthyear');
      $this->addFieldMapping('field_person_death_year', 'deathyear');
      $this->addFieldMapping('field_person_birth_uncertain', 'birthuncertain');
      $this->addFieldMapping('field_person_death_uncertain', 'deathuncertain');
     
    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language', 'sticky', 'revision_uid', 'path'));
  }

  function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('people', 'ppl')
      ->fields('ppl');
    return $query;
  }

  function prepareRow($current_row) {
    if (parent::prepareRow($current_row) === FALSE) {
      return FALSE;
    }
    //Statndard Drupal Fields
    $current_row->title = $current_row->Last . ', ' . $current_row->First;
    $current_row->created = $current_row->changed = time();
    $current_row->uid = 1;
    //Migrate Images
    $tmp_image_arr = array();
    if (!empty($current_row->Images) && $current_row->Images > 0){
      for ($i = 1; $i <= $current_row->Images; $i++) {
       $img_name = $current_row->Last . '_' . $current_row->First . "_" .  strval($i) . '.jpg';
       $tmp_image_arr[] = $img_name;
      }
    }
    $current_row->image_list = $tmp_image_arr;
    //profession
    //create clean array from the incoming string
    $profession1= str_replace("  ", " ", $current_row->Profession);
    $profession2= str_replace(", ", ",", $profession1);
    $profession3= str_replace("; ", ",", $profession2);
    $profession4= str_replace("/", ",", $profession3);
    $professionOut = explode(",", $profession4);
    $current_row->profession = $professionOut;

    //Birth and Death 
    if (strlen($current_row->Birth) == 4){
      $current_row->birthyear = $current_row->Birth;
      $current_row->birthuncertain = 0;
      }
    else if (strlen($current_row->Birth) > 4) {
      $current_row->birthyear=substr($current_row->Birth, 0, 4);
      $current_row->birthuncertain = 0;
    }
    else {
     $current_row->birthuncertain = 1;
    }
    
    if (strlen($current_row->Death) == 4){
      $current_row->deathyear = $current_row->Death;
      $current_row->deathuncertain = 0;
      }
    else if (strlen($current_row->Death) > 4){
      $current_row->deathyear=substr($current_row->Death, 0, 4);
      $current_row->deathuncertain = 0;
      }  
    else{
      $current_row->deathuncertain = 1;
    }
   $current_row->full_html = 'full_html';
  }//end prepare row


function complete($entity, stdClass $row) {
   // Create a new field collection 
  $node = $entity;
   $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_person_other_name'));
   // Load items into field collection
  if ($row->First2 && $row->First2 <>''){
    $field_collection_item->field_fc_first_name[LANGUAGE_NONE][0]['value'] = $row->First2;
  }
  if ($row->Middle2 && $row->Middle2 <>''){
    $field_collection_item->field_fc_middle_name[LANGUAGE_NONE][0]['value'] = $row->Middle2;
  }
  if ($row->Last2 && $row->Last2 <>''){
     $field_collection_item->field_fc_last_name[LANGUAGE_NONE][0]['value'] = $row->Last2;
  }
  if ($row->Title2 && $row->Title2 <>''){
    $field_collection_item->field_person_honorific_name[LANGUAGE_NONE][0]['value'] = $row->Title2;  
  }
  if  ($row->Title2 && $row->Title2 <>'' || ($row->Last2 && $row->Last2 <>'') || $row->Middle2 && $row->Middle2 <>'' || $row->First2 && $row->First2 <>'' ){
   $field_collection_item->setHostEntity('node', $node );
   $field_collection_item->save(TRUE);
   node_save($node);
  }
  }

} //end class

