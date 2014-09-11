<?php

class WorkAuthorMigration extends Migration {
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
    $this->person_map = $this->getPersonMap();
    $this->work_map = $this->getWorkMap();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'waID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source record',
          'alias' => 'wksaut',
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
      ->select('works_author', 'wksaut')
      ->fields('wksaut');
    return $query;
  }

  function getPersonMap() {
      $result = db_select('migrate_map_pfaffsperson', 'personmap')
      ->fields('personmap', array('sourceid1', 'destid1'))
      ->execute()->fetchAll();
    $person_map = array();
    foreach ($result as $record) {
      $person_map[$record->sourceid1] = $record->destid1;
    }
    return $person_map;
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
    $current_row->title = $current_row->waID;
    $current_row->referenced = array($this->person_map[$current_row->pID], $this->work_map[$current_row->wID]);
    $current_row->tid = 'tid';
    $association_descriptor = '';
    if ($current_row->waType == 2)
    {
      $current_row->type = array(20);
      $association_descriptor = ' subjectOf ';
    }
    else
    {
      switch ($current_row->waRole){
        case 1:
          $current_row->type = array(21);
          $association_descriptor = ' authorOf ';
          break;
        case 2:
          $current_row->type = array(22);
          $association_descriptor = ' editorOf ';
          break;
        case 3:
          $current_row->type = array(23);
          $association_descriptor = ' illustratorOf ';
          break;
        case 4:
          $current_row->type = array(24);
          $association_descriptor = ' translatorOf ';
          break;
        case 5:
          $current_row->type = array(25);
          $association_descriptor = ' publisherOf ';
          break;
        default:
          $current_row->type = array();
      }
    }
  $person_result = db_select('node', 'person')
      ->fields('person', array('title'))
      ->condition('nid', $this->person_map[$current_row->pID])
      ->execute()->fetchCol();

  $work_result = db_select('node', 'work')
      ->fields('work', array('title'))
      ->condition('nid', $this->work_map[$current_row->wID])
      ->execute()->fetchCol();
  //$current_row->title = $person_result[0] . $association_descriptor . $work_result[0];
  if  (strlen($work_result[0]) > 200){
  $partial_work = substr($work_result[0], 0, 197);  
  $partial_work .= "...";
  }
  else{
  $partial_work = $work_result[0];
  }   
  
  $current_row->title = $person_result[0] . $association_descriptor . $partial_work;
  $work_date_result = db_select('field_data_field_work_date', 'workdate')
      ->fields('workdate', array('field_work_date_value'))
      ->condition('entity_id', $this->work_map[$current_row->wID])
      ->execute()->fetchCol();
  $current_row->date = $work_date_result[0];

}

function complete($entity, stdClass $row) {
  $node = $entity;
  $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_association_mention'));
   // Load items into field collection
  $field_collection_item->field_mention_source[LANGUAGE_NONE][0]['nid'] =  $this->work_map[$row->wID];
  if ($row->waPages && $row->waPages <>''){
  $field_collection_item->field_mention_pages[LANGUAGE_NONE][0]['value'] = $row->waPages;
  } 
  if ($row->waIll && $row->waIll <>''){
  $field_collection_item->field_mention_illustration[LANGUAGE_NONE][0]['value'] = $row->waIll;
  }
  if ($row->waNote && $row->waNote <>''){
    $field_collection_item->field_mention_note[LANGUAGE_NONE][0]['value'] = $row->waNote;  
     $field_collection_item->field_mention_note[LANGUAGE_NONE][0]['format'] = 'full_html';
  }

  $field_collection_item->setHostEntity('node', $node );
  $field_collection_item->save(TRUE);
  node_save($node);
  }


}//end class
