<?php

class RelationshipMigration extends Migration {
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
   
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'reID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source record',
          'alias' => 'rel',
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
      ->select('relationships', 'rel')
      ->fields('rel');
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

  function prepareRow($current_row) {
    if (parent::prepareRow($current_row) === FALSE) {
      return FALSE;
    }
    $current_row->created = $current_row->changed = time();
    $current_row->uid = 1;
    //$current_row->title = $current_row->waID;
    $current_row->referenced = array($this->person_map[$current_row->p1ID], $this->person_map[$current_row->p2ID]);
    $current_row->tid = 'tid';
    $association_descriptor = ' generalRelationship ';
     switch ($current_row->reType){
        case 1:
          $current_row->type = array(7);
          $association_descriptor = ' acquaintanceOf ';
          break;
        case 2:
          $current_row->type = array(8);
          $association_descriptor = ' antagonistOf ';
          break;
        case 3:
          $current_row->type = array(9);
          $association_descriptor = ' collaboratesWith ';
          break;
        case 4:
          $current_row->type = array(9);
          $association_descriptor = ' collaboratesWith ';
          break;
        case 5:
          $current_row->type = array(11);
          $association_descriptor = ' familyOf ';
          break;
        case 6:
          $current_row->type = array(12);
          $association_descriptor = ' friendOf ';
          break;
       //no 7 in the database
        case 8:
          $current_row->type = array(13);
          $association_descriptor = ' lifePartnerOf ';
          break;
        case 9:
          $current_row->type = array(14);
          $association_descriptor = ' livesWith ';
          break;
        case 10:
        //asked to delete neighborOf one record changing to acquaintanceOf
          $current_row->type = array(7);
          $association_descriptor = ' acquaintanceOf ';
          break;
        case 11:
          $current_row->type = array(17);
          $association_descriptor = ' romanticWith ';
          break;
        case 12:
          $current_row->type = array(11);
          $association_descriptor = ' familyOf ';
          break;
        case 13:
          $current_row->type = array(11);
          $association_descriptor = ' familyOf ';
          break;
        case 14:
          $current_row->type = array(9);
          $association_descriptor = ' collaboratesWith ';
          break;
        case 15:
          $current_row->type = array(40);
          $association_descriptor = ' associatedWith ';
          break;
          default:
          $current_row->type = array(1);
        
      }
  $person_result1 = db_select('node', 'person')
      ->fields('person', array('title'))
      ->condition('nid', $this->person_map[$current_row->p1ID])
      ->execute()->fetchCol();

  $person_result2 = db_select('node', 'person')
      ->fields('person', array('title'))
      ->condition('nid', $this->person_map[$current_row->p2ID])
      ->execute()->fetchCol();
  
  $current_row->title = $person_result1[0] . $association_descriptor . $person_result2[0];
  
  //$work_date_result = db_select('field_data_field_work_date', 'workdate')
  //     ->fields('workdate', array('field_work_date_value'))
  //     ->condition('entity_id', $this->work_map[$current_row->wID])
  //     ->execute()->fetchCol();
  //$current_row->date = $work_date_result[0];


}
/*
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
  }

  $field_collection_item->setHostEntity('node', $node );
  $field_collection_item->save(TRUE);
  node_save($node);
  }
*/

}//end class
