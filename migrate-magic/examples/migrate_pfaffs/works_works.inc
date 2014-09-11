<?php

class WorksWorksMigration extends Migration {
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
      'title' => t('Title'),
      'type' => t('Default type set in prepareRow()'),
      'date' => t('Default date set in prepareRow()'),
      'referenced' => t('Default referenced set in prepareRow()'),
      'mention' => t('Default mention set in prepareRow()'),
    );
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));
   // $this->person_map = $this->getPersonMap();
    $this->work_map = $this->getWorkMap();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wwID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source record',
          'alias' => 'wkswks',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode($this->destinationType);
      $this->addSimpleMappings(array('created', 'changed', 'uid', 'title'));
      $this->addFieldMapping('field_association_date', 'date');
      $this->addFieldMapping('field_association_type', 'type');
      $this->addFieldMapping('field_association_type:source_type', 'tid');
      //->arguments(array('source_type' => 'tid'));
      $this->addFieldMapping('field_association_reference', 'referenced');
      $this->addFieldMapping('field_association_mention', 'mention');

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language', 'sticky', 'revision_uid', 'path'));
  }

  function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('works_works', 'wkswks')
      ->fields('wkswks');
    return $query;
  }

  function getWorkMap() {
    $result = db_select('migrate_map_pfaffswork', 'workmap')
      ->fields('workmap', array('sourceid1', 'destid1'))
      ->execute()->fetchAll();
    $work_map = array();
    foreach ($result as $record) {
      $work_map[$record->sourceid1] = $record->destid1;
    }
    return $work_map;
  }

  function prepareRow($current_row) {
    if (parent::prepareRow($current_row) === FALSE) {
      return FALSE;
    }
    $current_row->created = $current_row->changed = time();
    $current_row->uid = 1;
    $current_row->referenced = array($this->work_map[$current_row->w1ID], $this->work_map[$current_row->w2ID]);
    $current_row->tid = 'tid';
    $association_descriptor = '';
      switch ($current_row->wwType){
        case 30:
          $current_row->type = array(30);
          $association_descriptor = ' critiqueOf ';
          break;
        case 29:
          $current_row->type = array(29);
          $association_descriptor = ' parodyOf ';
          break;
        case 28:
          $current_row->type = array(28);
          $association_descriptor = ' partOf ';
          break;
        case 27:
          $current_row->type = array(27);
          $association_descriptor = ' revisionOf ';
          break;
        case 26:
          $current_row->type = array(26);
          $association_descriptor = ' reprintOf ';
          break;
        case 20:
          $current_row->type = array(20);
          $association_descriptor = ' subjectOf ';
          break;
        default:
          $current_row->type = array(6);
           $association_descriptor = ' relatedTo ';
      }
    
  $work_result1 = db_select('node', 'work')
      ->fields('work', array('title'))
      ->condition('nid', $this->work_map[$current_row->w1ID])
      ->execute()->fetchCol();

  $work_result2 = db_select('node', 'work')
      ->fields('work', array('title'))
      ->condition('nid', $this->work_map[$current_row->w2ID])
      ->execute()->fetchCol();
  //$current_row->title = $person_result[0] . $association_descriptor . $work_result[0];
  if  (strlen($work_result1[0]) > 100){
  $partial_work1 = substr($work_result1[0], 0, 97);  
  $partial_work1 .= "...";
  }
  else{
  $partial_work1 = $work_result1[0];
  }   
  if  (strlen($work_result2[0]) > 100){
  $partial_work2 = substr($work_result2[0], 0, 97);  
  $partial_work2 .= "...";
  }
  else{
  $partial_work2 = $work_result2[0];
  }  
  
  $current_row->title = $partial_work1 . $association_descriptor . $partial_work2;

  $work_date_result = db_select('field_data_field_work_date', 'workdate')
      ->fields('workdate', array('field_work_date_value'))
      ->condition('entity_id', $this->work_map[$current_row->w1ID])
      ->execute()->fetchCol();
  $current_row->date = $work_date_result[0];
}

function complete($entity, stdClass $row) {
  $node = $entity;
  $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_association_mention'));
   // Load items into field collection
  $field_collection_item->field_mention_source[LANGUAGE_NONE][0]['nid'] =  $this->work_map[$row->w1ID];
  if ($row->wwNote && $row->wwNote <>''){
    $field_collection_item->field_mention_note[LANGUAGE_NONE][0]['value'] = $row->wwNote;  
    $field_collection_item->field_mention_note[LANGUAGE_NONE][0]['format'] = 'full_html';
  }
  else{
      // $field_collection_item->field_mention_note[LANGUAGE_NONE][0]['value'] = 'wwNote is reading as empty empty';  
  }
  $field_collection_item->setHostEntity('node', $node );
  $field_collection_item->save(TRUE);
  node_save($node);
  }


}//end class
