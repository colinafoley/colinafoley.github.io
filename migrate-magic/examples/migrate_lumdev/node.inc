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

class LumdevNodePageMigration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_luminis_version', LUMDEV_LUMINIS_VERSION_VID)
      ->sourceMigration('LumdevLuminisVersionVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_channels', LUMDEV_CHANNELS_VID)
      ->sourceMigration('LumdevChannelsVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_codestorm', LUMDEV_CODESTORM_VID)
      ->sourceMigration('LumdevCodeStormVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_general', LUMDEV_GENERAL_VID)
      ->sourceMigration('LumdevGeneralVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_how_to', LUMDEV_HOWTO_VID)
      ->sourceMigration('LumdevHowToVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_modification', LUMDEV_MODIFICATION_VID)
      ->sourceMigration('LumdevModificationVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_hack_type', LUMDEV_TRUEHACK_VID)
      ->sourceMigration('LumdevTrueHackVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('field_attachments', 'upload')
      ->sourceMigration('LumdevFile');
    $this->addFieldMapping('field_attachments:file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_attachments:description', 'upload:description')
      ->sourceMigration('LumdevFile');
  }
}

class LumdevNodeBlogMigration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_luminis_version', LUMDEV_LUMINIS_VERSION_VID)
      ->sourceMigration('LumdevLuminisVersionVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_channels', LUMDEV_CHANNELS_VID)
      ->sourceMigration('LumdevChannelsVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_codestorm', LUMDEV_CODESTORM_VID)
      ->sourceMigration('LumdevCodeStormVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_general', LUMDEV_GENERAL_VID)
      ->sourceMigration('LumdevGeneralVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_how_to', LUMDEV_HOWTO_VID)
      ->sourceMigration('LumdevHowToVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_modification', LUMDEV_MODIFICATION_VID)
      ->sourceMigration('LumdevModificationVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_hack_type', LUMDEV_TRUEHACK_VID)
      ->sourceMigration('LumdevTrueHackVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('field_attachments', 'upload')
      ->sourceMigration('LumdevFile');
    $this->addFieldMapping('field_attachments:file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_attachments:description', 'upload:description')
      ->sourceMigration('LumdevFile');
  }
}

class LumdevNodeStoryMigration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_luminis_version', LUMDEV_LUMINIS_VERSION_VID)
      ->sourceMigration('LumdevLuminisVersionVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_channels', LUMDEV_CHANNELS_VID)
      ->sourceMigration('LumdevChannelsVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_codestorm', LUMDEV_CODESTORM_VID)
      ->sourceMigration('LumdevCodeStormVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_general', LUMDEV_GENERAL_VID)
      ->sourceMigration('LumdevGeneralVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_how_to', LUMDEV_HOWTO_VID)
      ->sourceMigration('LumdevHowToVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_modification', LUMDEV_MODIFICATION_VID)
      ->sourceMigration('LumdevModificationVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_hack_type', LUMDEV_TRUEHACK_VID)
      ->sourceMigration('LumdevTrueHackVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('field_attachments', 'upload')
      ->sourceMigration('LumdevFile');
    $this->addFieldMapping('field_attachments:file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_attachments:description', 'upload:description')
      ->sourceMigration('LumdevFile');
  }
}

class LumdevNodePollMigration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_luminis_version', LUMDEV_LUMINIS_VERSION_VID)
      ->sourceMigration('LumdevLuminisVersionVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_channels', LUMDEV_CHANNELS_VID)
      ->sourceMigration('LumdevChannelsVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_how_to', LUMDEV_HOWTO_VID)
      ->sourceMigration('LumdevHowToVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_modification', LUMDEV_MODIFICATION_VID)
      ->sourceMigration('LumdevModificationVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_hack_type', LUMDEV_TRUEHACK_VID)
      ->sourceMigration('LumdevTrueHackVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('active')
      ->defaultValue(1);
    $this->addFieldMapping('runtime', 'seconds_to_run')
      ->defaultValue(0);
    $this->addFieldMapping('choice', 'choice')
      ->description('src_choices populated in prepareRow()');
    $this->addFieldMapping('votes', 'votes')
      ->description('src_votes populated in prepareRow()');

    $this->addFieldMapping('field_attachments', 'upload')
      ->sourceMigration('LumdevFile');
    $this->addFieldMapping('field_attachments:file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_attachments:description', 'upload:description')
      ->sourceMigration('LumdevFile');
  }

  public function prepareRow($row) {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('poll_choices', 'pc')
      ->fields('pc', array('chtext', 'chorder', 'chvotes', 'chid'))
      ->condition('pc.nid', $row->nid);
    $choices = $query->execute()->fetchAll();
    $row->choice = array();

    foreach ($choices as $choice) {
      $row->choice[] = array(
        'chid' => $choice->chid,
        'chtext' => $choice->chtext,
        'chvotes' => $choice->chvotes,
        'weight' => $choice->chorder,
      );
    }
    // Note that we won't know until much later what the chid is for each
    // choice, so it's best to tie the votes to choices by text.
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('poll_votes', 'pv')
      ->fields('pv', array('uid', 'hostname'))
      ->condition('pv.nid', $row->nid);
    $query->innerJoin('poll_choices', 'pc', 'pv.nid = pc.nid AND pv.chorder = pc.chorder');
    $query->fields('pc', array('chtext', 'chid'));
    $votes = $query->execute()->fetchAll();

    $row->votes = array();
    foreach ($row->votes as $vote) {
      $chid = $vote['chid'];
      if (!isset($chid)) {
        $result = db_select('poll_choice', 'pc')
                ->fields('pc', array('chid'))
                ->condition('pc.nid', $entity->nid)
                ->condition('pc.chtext', $vote['chtext'])
                ->execute();
        $chid = $result->fetchField();
      }
    }
    return TRUE;
  }

}

class LumdevNodeForumMigration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('taxonomy_forums', LUMDEV_FORUMS_VID)
      ->sourceMigration('LumdevForumsVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('field_luminis_version', LUMDEV_LUMINIS_VERSION_VID)
      ->sourceMigration('LumdevLuminisVersionVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_codestorm', LUMDEV_CODESTORM_VID)
      ->sourceMigration('LumdevCodeStormVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_general', LUMDEV_GENERAL_VID)
      ->sourceMigration('LumdevGeneralVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('field_attachments', 'upload')
      ->sourceMigration('LumdevFile');
    $this->addFieldMapping('field_attachments:file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_attachments:description', 'upload:description')
      ->sourceMigration('LumdevFile');

  }
}


class LumdevNodeBookMigration extends DrupalNode6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_luminis_version', LUMDEV_LUMINIS_VERSION_VID)
      ->sourceMigration('LumdevLuminisVersionVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_channels', LUMDEV_CHANNELS_VID)
      ->sourceMigration('LumdevChannelsVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_how_to', LUMDEV_HOWTO_VID)
      ->sourceMigration('LumdevHowToVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_modification', LUMDEV_MODIFICATION_VID)
      ->sourceMigration('LumdevModificationVocabulary')
      ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_hack_type', LUMDEV_TRUEHACK_VID)
      ->sourceMigration('LumdevTrueHackVocabulary')
      ->arguments(array('source_type' => 'tid'));

    $this->addFieldMapping('field_attachments', 'upload')
      ->sourceMigration('LumdevFile');
    $this->addFieldMapping('field_attachments:file_class')
      ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_attachments:description', 'upload:description')
      ->sourceMigration('LumdevFile');
  }

}
