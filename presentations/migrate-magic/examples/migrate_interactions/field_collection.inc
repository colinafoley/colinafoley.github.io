<?php

abstract class NoteMigration extends Migration {
  protected $sourcePath;
  protected $fieldCollectionName;
  protected $additionalFields;

  function __construct(array $arguments) {
    if (!empty($arguments['source_path'])) {
      $this->sourcePath = $arguments['source_path'];
    }
    if (!empty($arguments['field_collection_name'])) {
      $this->fieldCollectionName = $arguments['field_collection_name'];
    }
    $this->additionalFields = is_array($this->additionalFields) ? $this->additionalFields : array();

    $this->destination = new MigrateDestinationFieldCollection($this->fieldCollectionName, array('host_entity_type' => 'node'));

    $this->source = new MigrateSourceCSV($this->sourcePath, $this->csvColumns(), $options = array(), $this->additionalFields);

    parent::__construct($arguments);
  }

  abstract protected function csvColumns();

  protected function convertDateIn(&$current_row_date) {
    if (!(empty($current_row_date))) {
      $current_row_date = date('Y-m-d H:i:s', strtotime($current_row_date));
    }
  }
}

class PersonNoteMigration extends NoteMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'per_id' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Person ID',
        ),
        'per_notedt' => array(
          'type' => 'varchar',
          'length' => 25,
          'not null' => true,
          'description' => 'Note date',
        ),
        'per_note' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => true,
          'description' => 'Note',
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

    $this->addFieldMapping('field_note_date', 'per_notedt')
      ->arguments(DateMigrateFieldHandler::arguments('UTC'));
    $this->addFieldMapping('field_note_detail', 'per_note');
    $this->addFieldMapping('host_entity_id', 'per_id')
      ->sourceMigration('Person');
  }

  public function csvColumns() {
    $columns[0] = array('per_id', 'Person ID');
    $columns[1] = array('per_notedt', 'Person Note Date');
    $columns[2] = array('per_note', 'Person Note');

    return $columns;
  }

  public function prepareRow($current_row) {
    $this->convertDateIn($current_row->per_notedt);
  }
}

class InstitutionNoteMigration extends NoteMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'ent_id' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Person ID',
        ),
        'ent_notedt' => array(
          'type' => 'varchar',
          'length' => 25,
          'not null' => true,
          'description' => 'Note date',
        ),
        'ent_note' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => true,
          'description' => 'Note',
        ),
      ),
      MigrateDestinationFieldCollection::getKeySchema()
    );

    $this->addFieldMapping('field_note_date', 'ent_notedt')
      ->arguments(DateMigrateFieldHandler::arguments('UTC'));
    $this->addFieldMapping('field_note_detail', 'ent_note');
    $this->addFieldMapping('host_entity_id', 'ent_id')
      ->sourceMigration('Institution');
  }

  public function csvColumns() {
    $columns[0] = array('ent_id', 'Institution ID');
    $columns[1] = array('ent_notedt', 'Institution Note Date');
    $columns[2] = array('ent_note', 'Institution Note');

    return $columns;
  }

  public function prepareRow($current_row) {
    $this->convertDateIn($current_row->ent_notedt);
  }
}
