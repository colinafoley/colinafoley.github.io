<?php

class LumdevRoleMigration extends DrupalRole6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}

class LumdevUserMigration extends DrupalUser6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_user_name', 'profile_name');
    $this->addFieldMapping('field_user_organization', 'profile_college');
    $this->addFieldMapping('field_user_city', 'profile_city');
    $this->addFieldMapping('field_user_state', 'profile_state');
    $this->addFieldMapping('field_user_bio', 'profile_bio');
    $this->addFieldMapping('field_user_source', 'profile_source');
    $this->addFieldMapping('field_user_focus', 'profile_expertise');
    $this->addFieldMapping('field_user_stopspam', 'profile_stopspam');

    $this->addUnmigratedDestinations(array(
      'field_user_name:language',
      'field_user_organization:language',
      'field_user_city:language',
      'field_user_state:language',
      'field_user_bio:language',
      'field_user_source:language',
      'field_user_stopspam:language',
    ));
  }

}
