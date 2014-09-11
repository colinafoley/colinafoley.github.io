<?php

abstract class GlobaldataNodeMigration extends Migration {
  protected $destinationType;
  protected $sourcePath;
  protected $additionalFields;

  function __construct(array $arguments) {
    if (!empty($arguments['destination_type'])) {
      $this->destinationType = $arguments['destination_type'];
    }
    if (!empty($arguments['source_path'])) {
      $this->sourcePath = $arguments['source_path'];
    }
    $this->additionalFields = is_array($this->additionalFields) ? $this->additionalFields : array();

    $this->destination = new MigrateDestinationNode($this->destinationType);
    $this->source = new MigrateSourceCSV($this->sourcePath, $this->csvColumns(), $options = array(), $this->additionalFields);

    parent::__construct($arguments);
  }

  abstract protected function csvColumns();
}

class RegionNodeMigration extends GlobaldataNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'reg_ID' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Region ID',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'region');
  }

  protected function csvColumns() {
    $columns[0] = array('reg_ID', 'Region ID');
    $columns[1] = array('region', 'Region');

    return $columns;
  }
}

class CountryNodeMigration extends GlobaldataNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'Cntry_code' => array(
          'type' => 'varchar',
          'length' => 5,
          'not null' => true,
          'description' => 'Country Code (may or may not be standard)',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'Country');
    $this->addFieldMapping('field_country_code', 'Cntry_code');
    $this->addFieldMapping('field_country_region', 'Region')
      ->sourceMigration('Region');
  }

  protected function csvColumns() {
    $columns[0] = array('Cntry_code', 'Country Code (may or may not be standard)');
    $columns[1] = array('Country', 'Country');
    $columns[2] = array('Region', 'Region ID');

    return $columns;
  }
}

class InstitutionNodeMigration extends GlobaldataNodeMigration {
  public function __construct(array $arguments) {
    $this->additionalFields = array(
      'file_directory' => t('Source file directory for each record\'s files.'),
      'attachment_list' => t('File list exploded by the \';\' delimiter on the Documents column'),
    );
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'Ent_ID' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Entity ID',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'Entity');
    $this->addFieldMapping('field_institution_type', 'Type')
      ->sourceMigration('InstitutionType')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_address_1', 'Address 1');
    $this->addFieldMapping('field_address_2', 'Address 2');
    $this->addFieldMapping('field_city', 'City');
    $this->addFieldMapping('field_postal_code', 'Postal Code');
    $this->addFieldMapping('field_principal_sub', 'Principal Subdivision');
    $this->addFieldMapping('field_country', 'Country')
      ->sourceMigration('Country');
    $this->addFieldMapping('field_phone', 'Phone');
    $this->addFieldMapping('field_institution_link', 'Ent_web');
    $this->addFieldMapping('field_institution_documents', 'attachment_list');
    $this->addFieldMapping('field_institution_documents:source_dir', 'file_directory');
    $this->addFieldMapping('field_institution_documents:destination_dir')
      ->defaultValue('private://');
  }

  protected function csvColumns() {
    $columns[0] = array('Ent_ID', 'Entity ID');
    $columns[1] = array('Entity', 'Institution Name');
    $columns[2] = array('Type', 'Type of Institution');
    $columns[3] = array('Address 1', 'Address 1');
    $columns[4] = array('Address 2', 'Address 2');
    $columns[5] = array('City', 'City');
    $columns[6] = array('Principal Subdivision', 'Prinicipal Subdivision');
    $columns[7] = array('Postal Code', 'Postal Code');
    $columns[8] = array('Country', 'Country');
    $columns[9] = array('Phone', 'Phone');
    $columns[10] = array('Ent_web', 'URL');
    $columns[11] = array('Documents', 'Documents');

    return $columns;
  }

  public function prepareRow($current_row) {
    $current_row->file_directory = substr($this->sourcePath, 0, -4) . '/' . $current_row->Ent_ID;
    $current_row->attachment_list = explode(';', $current_row->Documents);
  }
}

class PersonNodeMigration extends GlobaldataNodeMigration {
  public function __construct(array $arguments) {
    $this->additionalFields = array(
      'name' => t('Constructed name from the Last Name and First Name source fields.'),
      'file_directory' => t('Source file directory for each record\'s files.'),
      'attachment_list' => t('File list exploded by the \';\' delimiter on the Documents column'),
    );
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'P_ID' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Person ID',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('field_person_last_name', 'LastName');
    $this->addFieldMapping('field_person_first_name', 'FirstName');
    $this->addFieldMapping('field_person_title', 'Title');
    $this->addFieldMapping('field_person_department', 'Department');
    $this->addFieldMapping('field_person_affiliation', 'P_coll')
      ->sourceMigration('Affiliation')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_person_email', 'Email');
    $this->addFieldMapping('field_phone', 'Phone');
    $this->addFieldMapping('field_person_type', 'P_type')
      ->sourceMigration('PersonType')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_person_institution', 'P_ent')
      ->sourceMigration('Institution');
    $this->addFieldMapping('field_person_trip_report', 'attachment_list');
    $this->addFieldMapping('field_person_trip_report:source_dir', 'file_directory');
    $this->addFieldMapping('field_person_trip_report:destination_dir')
      ->defaultValue('private://');
    $this->addFieldMapping('field_address_1', 'Address 1');
    $this->addFieldMapping('field_address_2', 'Address 2');
    $this->addFieldMapping('field_city', 'City');
    $this->addFieldMapping('field_postal_code', 'Postal Code');
    $this->addFieldMapping('field_principal_subdivision', 'Principal Subdivision');
    $this->addFieldMapping('field_country', 'Country')
      ->sourceMigration('Country');
    $this->addFieldMapping('field_gender', 'Gender');
  }

  protected function csvColumns() {
    $columns[0] = array('P_ID', 'Person ID');
    $columns[1] = array('LastName', 'Last Name');
    $columns[2] = array('FirstName', 'First Name');
    $columns[3] = array('Title', 'Title');
    $columns[4] = array('Department', 'Department');
    $columns[5] = array('P_coll', 'Affiliation');
    $columns[6] = array('Email', 'Email');
    $columns[7] = array('Phone', 'Phone');
    $columns[8] = array('P_type', 'Person Type');
    $columns[9] = array('P_ent', 'Person Institution');
    $columns[10] = array('TripReport', 'Trip Report');
    $columns[11] = array('Address 1', 'Address 1');
    $columns[12] = array('Address 2', 'Address 2');
    $columns[13] = array('City', 'City');
    $columns[14] = array('Principal Subdivision', 'Principal Subdivision');
    $columns[15] = array('Postal Code', 'Postal Code');
    $columns[16] = array('Country', 'Country');
    $columns[17] = array('Gender', 'Gender');

    return $columns;
  }

  public function prepareRow($current_row) {
    $current_row->name = $current_row->FirstName . ' ' . $current_row->LastName;
    $current_row->file_directory = substr($this->sourcePath, 0, -4) . '/' . $current_row->P_ID;
    $current_row->attachment_list = explode(';', $current_row->TripReport);
  }
}

class AgreementNodeMigration extends GlobaldataNodeMigration {
  public function __construct(array $arguments) {
    $this->additionalFields = array(
      'title' => t('Title built from Agreement Type and Institution.'),
      'file_directory' => t('Source file directory for each record\'s files.'),
      'attachment_list' => t('File list exploded by the \';\' delimiter on the Documents column'),
    );
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'Agr_ID' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'description' => 'Agreement ID',
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addSimpleMappings(array('title'));
    $this->addFieldMapping('field_agreement_institution', 'Ent_ID')
      ->sourceMigration('Institution');
    $this->addFieldMapping('field_agreement_type', 'Agr_type')
      ->sourceMigration('AgreementType')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_agreement_signed_date', 'Agr_from');
    $this->addFieldMapping('field_agreement_expiration_date', 'Agr_to');
    $this->addFieldMapping('field_agreement_attachments', 'attachment_list');
    $this->addFieldMapping('field_agreement_attachments:source_dir', 'file_directory');
    $this->addFieldMapping('field_agreement_attachments:destination_dir')
      ->defaultValue('private://');
  }

  public function csvColumns() {
    $columns[0] = array('Agr_ID', 'Agreement ID');
    $columns[1] = array('Ent_ID', 'Entity ID');
    $columns[2] = array('Agr_type', 'Agreement Type');
    $columns[3] = array('Agr_from', 'Agreement Signed Date');
    $columns[4] = array('Agr_to', 'Agreement Expiration Date');
    $columns[5] = array('agr', 'Agreement');

    return $columns;
  }

  public function prepareRow($current_row) {
    $title = $this->getInstitutionTitleFrom($current_row);
    $type = $this->getAgreementTypeFrom($current_row);
    $current_row->title = $type . ' - ' . $title;

    $this->convertDateIn($current_row->Agr_from);
    $this->convertDateIn($current_row->Agr_to);

    $current_row->file_directory = substr($this->sourcePath, 0, -4) . '/' . $current_row->Agr_ID;
    $current_row->attachment_list = explode(';', $current_row->agr);
  }

  private function getInstitutionTitleFrom($current_row) {
    $query = db_select('node', 'n')
      ->fields('n', array('title'));
    $query->join('migrate_map_institution', 'mmi', 'mmi.destid1 = n.nid');
    $query->condition('mmi.sourceid1', $current_row->Ent_ID, '=');
    $result = $query->execute()->fetchCol();

    return $result[0];
  }

  private function getAgreementTypeFrom($current_row) {
    $query = db_select('taxonomy_term_data', 'ttd')
      ->fields('ttd', array('name'));
    $query->join('migrate_map_agreementtype', 'mma', 'mma.destid1 = ttd.tid');
    $query->condition('mma.sourceid1', $current_row->Agr_type, '=');
    $result = $query->execute()->fetchCol();

    return $result[0];
  }

  private function convertDateIn(&$current_row_date) {
    if (!(empty($current_row_date))) {
      $current_row_date = date('Y-m-d H:i:s', strtotime($current_row_date));
    }
  }

}
