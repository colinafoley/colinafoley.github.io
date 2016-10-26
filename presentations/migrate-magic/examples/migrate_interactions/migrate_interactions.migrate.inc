<?php

function migrate_interactions_migrate_api() {
  $common_arguments = array(
  );

  $file_path = dirname(__FILE__) . '/data';

  $api = array(
    'api' => 2,
    'groups' => array(
      'term' => array(
        'title' => t('Term Migrations'),
      ),
      'content' => array(
        'title' => t('Content Migrations'),
      ),
      'notes' => array(
        'title' => t('Notes Migrations'),
      ),
    ),
    'migrations' => array(
      'PersonType' => array(
        'class_name' => 'PersonTypeTermMigration',
        'group_name' => 'term',
        'description' => t('Migrate tbv_pertype into new Person Type vocabulary.'),
        'destination_vocabulary' => 'person_type',
        'source_path' => $file_path . '/tbv_pertype.txt',
      ),
      'AgreementType' => array(
        'class_name' => 'AgreementTypeTermMigration',
        'group_name' => 'term',
        'description' => t('Migrate tbv_agreement into new Agreement Type vocabulary.'),
        'destination_vocabulary' => 'agreement_type',
        'source_path' => $file_path . '/tbv_agreement.txt',
      ),
      'InstitutionType' => array(
        'class_name' => 'InstitutionTypeTermMigration',
        'group_name' => 'term',
        'description' => t('Migrate tbv_enttype into new Institution Type vocabulary.'),
        'destination_vocabulary' => 'institution_type',
        'source_path' => $file_path . '/tbv_enttype.txt',
      ),
      'Affiliation' => array(
        'class_name' => 'AffiliationTermMigration',
        'group_name' => 'term',
        'description' => t('Migrate tbv_affiliation into new Affiliation vocabulary.'),
        'destination_vocabulary' => 'affiliation',
        'source_path' => $file_path . '/tbv_affiliation.txt',
      ),
      'Region' => array(
        'class_name' => 'RegionNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate tbv_regions into region nodes.'),
        'destination_type' => 'region',
        'source_path' => $file_path . '/tbv_regions.txt',
      ),
      'Country' => array(
        'class_name' => 'CountryNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate tbv_country into country nodes.'),
        'destination_type' => 'country',
        'source_path' => $file_path . '/tbv_country.txt',
        'dependencies' => array(
          'Region',
        ),
      ),
      'Institution' => array(
        'class_name' => 'InstitutionNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate tbl_entity into institution nodes.'),
        'destination_type' => 'institution',
        'source_path' => $file_path . '/tbl_entity.txt',
        'dependencies' => array(
          'InstitutionType',
          'Country',
        ),
      ),
      'Person' => array(
        'class_name' => 'PersonNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate tbl_person into person nodes.'),
        'destination_type' => 'person',
        'source_path' => $file_path . '/tbl_person.txt',
        'dependencies' => array(
          'Affiliation',
          'PersonType',
          'Institution',
          'Country',
        ),
      ),
      'Agreement' => array(
        'class_name' => 'AgreementNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate tbl_agreement into agreement nodes.'),
        'destination_type' => 'agreement',
        'source_path' => $file_path . '/tbl_agreement.txt',
        'dependencies' => array(
          'AgreementType',
          'Institution',
        ),
      ),
      'PersonNote' => array(
        'class_name' => 'PersonNoteMigration',
        'group_name' => 'notes',
        'description' => t('Migrate tbl_per_notes into field collections attached to person nodes.'),
        'field_collection_name' => 'field_note',
        'source_path' => $file_path . '/tbl_per_notes.txt',
        'dependencies' => array(
          'Person',
        ),
      ),
      'InstitutionNote' => array(
        'class_name' => 'InstitutionNoteMigration',
        'group_name' => 'notes',
        'description' => t('Migrate tbl_ent_notes into field collections attached to institution nodes.'),
        'field_collection_name' => 'field_note',
        'source_path' => $file_path . '/tbl_ent_notes.txt',
        'dependencies' => array(
          'Institution',
        ),
      ),
    ),
  );

  foreach ($api['migrations'] as &$migration) {
    $migration = $migration + $common_arguments;
  }

  return $api;
}
