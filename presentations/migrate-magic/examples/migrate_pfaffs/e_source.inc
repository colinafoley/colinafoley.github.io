<?php

class E_SourceNodeMigration extends Migration {
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
      'Title' => t('Default title set in prepareRow()'),
      'Description' => t('description'),
      
    );
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'lID' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of source record',
          'alias' => 'linkid',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->destination = new MigrateDestinationNode($this->destinationType);
      $this->addSimpleMappings(array('changed', 'uid'));
      $this->addFieldMapping('field_e_source_description', 'Description');
      $this->addFieldMapping('title', 'Title');
      $this->addFieldMapping('field_e_source_description:format', 'format');
          
    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'status', 'promote',
      'revision', 'language', 'sticky', 'revision_uid', 'path'));
  }

  function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('e_source', 'linkid')
      ->fields('linkid');
    return $query;
  }

  function prepareRow($current_row) {
    if (parent::prepareRow($current_row) === FALSE) {
      return FALSE;
    }
    //Statndard Drupal Fields
    $current_row->created = $current_row->changed = time();
    $current_row->uid = 1;
    $current_row->format = 'full_html';
    //Migrate Images
    
  }//end prepare row


} //end class

