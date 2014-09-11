<?php

class WorkRelationshipMigration extends Migration {
  /**
   * The source and destination content types (bundles) we're dealing with.
   */
  //protected $destinationType;

  /**
   * The source database key defined in settings.php.
   */
  protected $sourceConnection;
  
  public function __construct($arguments) {
    //$this->destinationType = $arguments['destination_type'];
    $this->sourceConnection = $arguments['source_connection'];
    $this->dependencies = array('PfaffsRelationship');
    parent::__construct($arguments);

    $this->sourceFields = array(
      'source' => t('Default uid set in prepareRow()'),
      'pages' => t('Default pages set in prepareRow()'),
      'illustration' => t('Default illustration set in prepareRow()'),
      'note' => t('Default note set in prepareRow()'),
      
    );
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));
    //$this->relationship_map = $this->getRelationshipMap();
    $this->work_map = $this->getWorkMap();

     $this->destination = new MigrateDestinationFieldCollection(
        'field_association_mention',
        array('host_entity_type' => 'node')
      );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wrID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source work relationship record',
          'alias' => 'wksrel',
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );
      $this->addFieldMapping('field_mention_source', 'source');
      $this->addFieldMapping('field_mention_pages', 'pages');
     // $this->addFieldMapping('field_mention_illustration', 'illustration');
      $this->addFieldMapping('field_mention_note', 'note');
      $this->addFieldMapping('field_mention_note:format', 'full_html');
      $this->addFieldMapping('host_entity_id', 'rID')->sourceMigration('PfaffsRelationship');
  }

  function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('works_relationship', 'wksrel')
      ->fields('wksrel');
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
    $current_row->source = $this->work_map[$current_row->wID];
    $current_row->pages = $current_row->wrPages;
    //$current_row->illustration = $this->$current_row->wID;
    $current_row->note = $current_row->wrNote;
    $current_row->full_html = "full_html";
  }

}//end class
