<?php

abstract class GlobaldataTermMigration extends Migration {
  protected $destinationVocabulary;
  protected $sourcePath;

  function __construct(array $arguments) {
    if (!empty($arguments['destination_vocabulary'])) {
      $this->destinationVocabulary = $arguments['destination_vocabulary'];
    }
    if (!empty($arguments['source_path'])) {
      $this->sourcePath = $arguments['source_path'];
    }

    $this->addUnmigratedDestinations(array(
      'description',
      'parent',
      'parent_name',
      'format',
      'weight',
      'path',
    ));

    $this->destination = new MigrateDestinationTerm($this->destinationVocabulary);
    $this->source = new MigrateSourceCSV($this->sourcePath, $this->csvColumns(), $options = array());

    parent::__construct($arguments);
  }

  abstract protected function csvColumns();
}

class PersonTypeTermMigration extends GlobaldataTermMigration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'PType_code' => array(
          'type' => 'varchar',
          'length' => 2,
          'not null' => true,
          'description' => 'Person Type code',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'PType_desc');
  }

  protected function csvColumns() {
    $columns[0] = array('PType_code', 'Person Type Code');
    $columns[1] = array('PType_desc', 'Person Type Description');
    return $columns;
  }
}

class AgreementTypeTermMigration extends GlobaldataTermMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'agr_ID' => array(
          'type' => 'varchar',
          'length' => 3,
          'not null' => true,
          'description' => 'Agreement Type ID',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'agr_desc');
  }

  protected function csvColumns() {
    $columns[0] = array('agr_ID', 'Agreement Type ID');
    $columns[1] = array('agr_desc', 'Agreement Type Description');
    return $columns;
  }
}

class InstitutionTypeTermMigration extends GlobaldataTermMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'EType_ID' => array(
          'type' => 'varchar',
          'length' => 2,
          'not null' => true,
          'description' => 'Institution Type ID',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'EType');
  }

  protected function csvColumns() {
    $columns[0] = array('EType_ID', 'Institution Type ID');
    $columns[1] = array('EType', 'Institution Type Description');
    return $columns;
  }
}
class AffiliationTermMigration extends GlobaldataTermMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'Aff_id' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Affiliation ID',
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $this->addFieldMapping('name', 'Affiliation');
    $this->addFieldMapping('field_affiliation_type', 'Aff_type');
  }

  protected function csvColumns() {
    $columns[0] = array('Aff_id', 'Affiliation ID');
    $columns[1] = array('Affiliation', 'Affiliation');
    $columns[2] = array('Aff_type', 'Affiliation Type (C/O)');
    return $columns;
  }
}
