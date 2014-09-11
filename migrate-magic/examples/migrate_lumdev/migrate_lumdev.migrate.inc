<?php
//LumDev Vocabulary Constants
if (!defined('LUMDEV_CHANNELS_VID'))        define('LUMDEV_CHANNELS_VID',         1);
if (!defined('LUMDEV_HOWTO_VID'))           define('LUMDEV_HOWTO_VID',            2);
if (!defined('LUMDEV_TRUEHACK_VID'))        define('LUMDEV_TRUEHACK_VID',         3);
if (!defined('LUMDEV_MODIFICATION_VID'))    define('LUMDEV_MODIFICATION_VID',     4);
if (!defined('LUMDEV_FORUMS_VID'))          define('LUMDEV_FORUMS_VID',           5);
if (!defined('LUMDEV_GENERAL_VID'))         define('LUMDEV_GENERAL_VID',          7);
if (!defined('LUMDEV_LUMINIS_VERSION_VID')) define('LUMDEV_LUMINIS_VERSION_VID',  8);
if (!defined('LUMDEV_CODESTORM_VID'))       define('LUMDEV_CODESTORM_VID',        9);

function migrate_lumdev_migrate_api() {
  $common_arguments = array(
    'source_version' => 6,
    'source_connection' => 'legacy',
  );

  $api = array(
    'api' => 2,
    'groups' => array(
      'user' => array(
        'title' => 'User Migrations',
      ),
      'taxonomy' => array(
        'title' => 'Taxonomy Migrations',
      ),
      'file' => array(
        'title' => 'File Migrations',
      ),
      'content' => array(
        'title' => 'Content Migrations',
      ),
      'comment' => array(
        'title' => 'Comment Migrations',
      ),
    ),
    'migrations' => array(
      'LumdevRole' => array(
        'class_name' => 'LumdevRoleMigration',
        'group_name' => 'user',
        'description' => t('Migrate lumdev roles.'),
        'role_mappings' => array(
          'anonymous user' => 'anonymous user',
          'authenticated user' => 'authenticated user',
          'Full Admin' => 'administrator',
        ),
      ),
      'LumdevUser' => array(
        'class_name' => 'LumdevUserMigration',
        'group_name' => 'user',
        'description' => t('Migrate lumdev users.'),
        'role_migration' => 'LumdevRole',
        'dependencies' => array('LumdevRole'),
      ),
      'LumdevChannelsVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate Channels taxonomy.'),
        'source_vocabulary' => LUMDEV_CHANNELS_VID,
        'destination_vocabulary' => 'channels',
      ),
      'LumdevHowToVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate How-To taxonomy.'),
        'source_vocabulary' => LUMDEV_HOWTO_VID,
        'destination_vocabulary' => 'how_to',
      ),
      'LumdevTrueHackVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate True Hack taxonomy Hack Type.'),
        'source_vocabulary' => LUMDEV_TRUEHACK_VID,
        'destination_vocabulary' => 'hack_type',
      ),
      'LumdevModificationVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate Modification taxonomy.'),
        'source_vocabulary' => LUMDEV_MODIFICATION_VID,
        'destination_vocabulary' => 'modification',
      ),
      'LumdevForumsVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate forums taxonomy.'),
        'source_vocabulary' => LUMDEV_FORUMS_VID,
        'destination_vocabulary' => 'forums',
      ),
      'LumdevGeneralVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate General taxonomy.'),
        'source_vocabulary' => LUMDEV_GENERAL_VID,
        'destination_vocabulary' => 'general',
      ),
      'LumdevLuminisVersionVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate Luminis Version taxonomy.'),
        'source_vocabulary' => LUMDEV_LUMINIS_VERSION_VID,
        'destination_vocabulary' => 'luminis_version',
      ),
      'LumdevCodeStormVocabulary' => array(
        'class_name' => 'DrupalTerm6Migration',
        'group_name' => 'taxonomy',
        'description' => t('Migrate CodeStorm taxonomy.'),
        'source_vocabulary' => LUMDEV_CODESTORM_VID,
        'destination_vocabulary' => 'codestorm',
      ),
      'LumdevFile' => array(
        'class_name' => 'DrupalFile6Migration',
        'group_name' => 'file',
        'description' => t('Migrate files'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_dir' => 'http://www.lumdev.net/files/',
      ),
      'LumdevNodePage' => array(
        'class_name' => 'LumdevNodePageMigration',
        'group_name' => 'content',
        'description' => t('Migrate Pages.'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_type' => 'page',
        'destination_type' => 'page',
        'dependencies' => array(
          'LumdevLuminisVersionVocabulary',
          'LumdevChannelsVocabulary',
          'LumdevCodeStormVocabulary',
          'LumdevGeneralVocabulary',
          'LumdevHowToVocabulary',
          'LumdevModificationVocabulary',
          'LumdevTrueHackVocabulary',
          'LumdevFile',
        ),
      ),
      'LumdevNodeBlog' => array(
        'class_name' => 'LumdevNodeBlogMigration',
        'group_name' => 'content',
        'description' => t('Migrate Blogs.'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_type' => 'blog',
        'destination_type' => 'blog',
        'dependencies' => array(
          'LumdevLuminisVersionVocabulary',
          'LumdevChannelsVocabulary',
          'LumdevCodeStormVocabulary',
          'LumdevGeneralVocabulary',
          'LumdevHowToVocabulary',
          'LumdevModificationVocabulary',
          'LumdevTrueHackVocabulary',
          'LumdevFile',
        ),
      ),
      'LumdevNodeStory' => array(
        'class_name' => 'LumdevNodeStoryMigration',
        'group_name' => 'content',
        'description' => t('Migrate Stories to Articles.'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_type' => 'story',
        'destination_type' => 'article',
        'dependencies' => array(
          'LumdevLuminisVersionVocabulary',
          'LumdevChannelsVocabulary',
          'LumdevCodeStormVocabulary',
          'LumdevGeneralVocabulary',
          'LumdevHowToVocabulary',
          'LumdevModificationVocabulary',
          'LumdevTrueHackVocabulary',
          'LumdevFile',
        ),
      ),
      'LumdevNodePoll' => array(
        'class_name' => 'LumdevNodePollMigration',
        'group_name' => 'content',
        'description' => t('Migrate Polls to Polls.'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_type' => 'poll',
        'destination_type' => 'poll',
        'dependencies' => array(
          'LumdevLuminisVersionVocabulary',
          'LumdevChannelsVocabulary',
          'LumdevHowToVocabulary',
          'LumdevModificationVocabulary',
          'LumdevTrueHackVocabulary',
          'LumdevFile',
        ),
      ),
      'LumdevNodeForum' => array(
        'class_name' => 'LumdevNodeForumMigration',
        'group_name' => 'content',
        'description' => t('Migrate Polls to Polls.'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_type' => 'forum',
        'destination_type' => 'forum',
        'dependencies' => array(
          'LumdevForumsVocabulary',
          'LumdevLuminisVersionVocabulary',
          'LumdevGeneralVocabulary',
          'LumdevCodeStormVocabulary',
          'LumdevFile',
        ),
      ),
      'LumdevNodeBook' => array(
        'class_name' => 'LumdevNodeBookMigration',
        'group_name' => 'content',
        'description' => t('Migrate Book pages.'),
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
        'source_type' => 'book',
        'destination_type' => 'book',
        'dependencies' => array(
          'LumdevLuminisVersionVocabulary',
          'LumdevChannelsVocabulary',
          'LumdevHowToVocabulary',
          'LumdevModificationVocabulary',
          'LumdevTrueHackVocabulary',
          'LumdevFile',
        ),
      ),
      'LumdevCommentPageMigration' => array(
        'class_name' => 'DrupalComment6Migration',
        'group_name' => 'comment',
        'description' => t('Migrate page comments'),
        'source_type' => 'page',
        'destination_type' => 'page',
        'node_migration' => 'LumdevNodePage',
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
      ),
      'LumdevCommentBlogMigration' => array(
        'class_name' => 'DrupalComment6Migration',
        'group_name' => 'comment',
        'description' => t('Migrate blog comments'),
        'source_type' => 'blog',
        'destination_type' => 'blog',
        'node_migration' => 'LumdevNodeBlog',
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
      ),
      'LumdevCommentStoryMigration' => array(
        'class_name' => 'DrupalComment6Migration',
        'group_name' => 'comment',
        'description' => t('Migrate story comments to article comments'),
        'source_type' => 'story',
        'destination_type' => 'article',
        'node_migration' => 'LumdevNodeStory',
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
      ),
      'LumdevCommentPollMigration' => array(
        'class_name' => 'DrupalComment6Migration',
        'group_name' => 'comment',
        'description' => t('Migrate poll comments'),
        'source_type' => 'poll',
        'destination_type' => 'poll',
        'node_migration' => 'LumdevNodePoll',
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
      ),
      'LumdevCommentForumMigration' => array(
        'class_name' => 'DrupalComment6Migration',
        'group_name' => 'comment',
        'description' => t('Migrate forum comments'),
        'source_type' => 'forum',
        'destination_type' => 'forum',
        'node_migration' => 'LumdevNodeForum',
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
      ),
      'LumdevCommentBookMigration' => array(
        'class_name' => 'DrupalComment6Migration',
        'group_name' => 'comment',
        'description' => t('Migrate book comments'),
        'source_type' => 'book',
        'destination_type' => 'book',
        'node_migration' => 'LumdevNodeBook',
        'user_migration' => 'LumdevUser',
        'default_uid' => 1,
      ),
    ),
  );
  foreach ($api['migrations'] as &$migration) {
    $migration = $migration + $common_arguments;
  }

  return $api;
}
