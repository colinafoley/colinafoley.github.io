<?php

function migrate_pfaffs_migrate_api() {
  $common_arguments = array(
    'source_connection' => 'legacy',
  );

  $api = array(
    'api' => 2,
    'groups' => array(
      'content' => array(
        'title' => t('Pfaffs Content Migrations'),
      ),
        'relationship' => array(
        'title' => t('Pfaffs Relationship Migrations'),
      ),
    ),
    'migrations' => array(
      'PfaffsPerson' => array(
        'class_name' => 'PersonNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate Pfaffs Persons.'),
        'destination_type' => 'person',
      ),
      'PfaffsE_Source' => array(
        'class_name' => 'E_SourceNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate Pfaffs Electronic Sources.'),
        'destination_type' => 'e_source',
      ),
      'PfaffsWork' => array(
        'class_name' => 'WorkNodeMigration',
        'group_name' => 'content',
        'description' => t('Migrate Pfaffs Works.'),
        'destination_type' => 'work',
      ),
      'PfaffsWorkAuthor' => array(
        'class_name' => 'WorkAuthorMigration',
        'group_name' => 'relationship',
        'description' => t('Migrate Pfaffs Work Authors.'),
        'destination_type' => 'association',
      ),
      'PfaffsRelationship' => array(
        'class_name' => 'RelationshipMigration',
        'group_name' => 'relationship',
        'description' => t('Migrate Pfaffs Relationships.'),
        'destination_type' => 'association',
      ),
      'PfaffsWorksRelationship' => array(
        'class_name' => 'WorkRelationshipMigration',
        'group_name' => 'relationship',
        'description' => t('Migrate Pfaffs Works Relationships.'),
      ),
        'PfaffsWorksWorks' => array(
        'class_name' => 'WorksWorksMigration',
        'group_name' => 'relationship',
        'description' => t('Migrate Pfaffs Works Works Table.'),
        'destination_type' => 'association',
      ),
    ),
  );

  foreach ($api['migrations'] as &$migration) {
    $migration = $migration + $common_arguments;
  }
  return $api;
}
