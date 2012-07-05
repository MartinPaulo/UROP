<?php

  /**
   * @file
   * This file contains a form that will upload csv files containing data from the old system, then iterate through
   * them, reading the data from them and copying it into the new system.
   * For each entry in the old file a user account has to be created.
   *
   * Author: Martin Paulo
   *
   * This file is part of UROP's data import module.
   *
   * UROP's data import module is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * (at your option) any later version.
   *
   * UROP's data import module is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with UROP's data import module.  If not, see <http://www.gnu.org/licenses/>.
   */

  // we are importing from a csv file. Hence the 'A', 'B', 'C' mapping to column number comments
  // The main tables mappings.
  define('U_ID', 0); // A 1 The "U_PROFILE"
  define('U_DASH_ONE', 1); // B 2
  define('U_FAMILY_NAME', 2); // C 3
  define('U_GIVEN_NAME', 3); // D 4
  define('U_PREFERRED_NAME', 4); // E 5
  define('U_DOB', 5); // F 6
  define('U_EMAIL_PRIVATE', 6); // G 7
  define('U_EMAIL_UNI', 7); // H 8
  define('U_STREET_ONE', 8); // I 9
  define('U_STREET_TWO', 9); // J
  define('U_CITY', 10); // K
  define('U_POSTCODE', 11); // L
  define('U_STATE', 12); // M
  define('U_COUNTRY', 13); // N
  define('U_PHONE', 14); // O
  define('U_MOBILE', 15); // P
  define('U_UNIVERSITY', 16); // Q
  define('U_OTHER_UNIVERSITY', 17); // R
  define('U_COURSE', 18); // S
  define('U_COURSE_OTHER', 19); // T
  define('U_COURSE_DURATION', 20); // U
  define('U_COURSE_COMPLETION_YEAR', 21); // V
  define('U_INTERNATIONAL', 22); // W = 23
  define('U_COUNTRY', 23); // X = 24
  define('U_DASH_TWO', 24); // Y = 25
  define('U_STATUS', 25); // Z = 26
  define('U_DATE_SUSPENDED', 26); // AA = 26 + 1 = 27

  define('U_PREVIOUS_APPLICANT', 28);
  define('U_NO_PREVIOUS', 29);
  define('U_SELECTED_FOR_INTERVIEW', 30);
  define('U_INTERVIEW_SUCCESSFUL', 31); // AF = 26 + 6
  define('U_CRC_LIST', 31);
  define('U_UROP_PLACEMENT', 32);
  define('U_DASH_THREE', 33);
  define('U_HONOURS', 34);
  define('U_HONOURS_YEAR', 35);
  define('U_HONOURS_INSTITUTE', 36);
  define('U_OTHER_INSTITUTE', 37);
  define('U_FILE_APPLICATION', 64); // BM = 26 + 26 + 13
  define('U_FILE_CV', 65); // BN = 26 + 26 + 14
  define('U_FILE_RESULTS', 66); // BO = 26 + 26 + 15
  define('U_GENDER', 67); // BP = 26 + 26 + 16

  // the supervisors table mappings
  define('S_ID', 0); // A = 0
  define('S_INSTITUTE', 0); // B = 1
  define('S_LAB', 0); // C
  define('S_STATE', 0); // D
  define('S_NAME', 0); // E
  define('S_EMAIL', 0); // F
  define('S_START_DATE', 0); // G
  define('S_END_DATE', 0); // H
  define('S_COMMENTS', 0); // I
  define('S_U_PROFILE', 0); // J

  // the notes table mappings
  define('N_ID', 0); // A = 0
  define('N_YEAR', 1); // B = 1
  define('N_DESCRIPTION', 2); // C
  define('N_U_PROFILE', 3); // D



  function urop_form_import_data() {
    $result = drupal_get_form('urop_import_data_form', $student, $user->uid);
    return $result;
  }

  function urop_import_data_form($form_state) {
    $form['import'] = array(
      '#type' => 'fieldset',
      '#title' => t('UROP data import'),
      '#description' => t('<p>Imports CSV data from old system. <p>Select the files to be imported. (Maximum size is: @size MB).', array('@size' => file_upload_max_size()))
    );

    $form['import']['main_data_file'] = array(
      '#type' => 'file',
      '#title' => t('Main CSV File'),
      '#size' => 60,
      '#description' => t('CSV dump of the table formerly known as DataContent_25.', array('@size' => file_upload_max_size())),
    );

    $form['import']['supervisor_data_file'] = array(
      '#type' => 'file',
      '#title' => t('Supervisor CSV File'),
      '#size' => 60,
      '#description' => t('CSV dump of the table formerly known as DataContent_23.', array('@size' => file_upload_max_size())),
    );

    $form['import']['note_data_file'] = array(
      '#type' => 'file',
      '#title' => t('Notes CSV File'),
      '#size' => 60,
      '#description' => t('CSV dump of the table formerly known as DataContent_24.', array('@size' => file_upload_max_size())),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import'),
    );

    // set the form encoding type
    $form['#attributes']['enctype'] = "multipart/form-data";

    return $form;
  }

  function urop_import_data_form_validate($form, &$form_state) {
    _check_file($form_state, 'main_data_file');
    _check_file($form_state, 'supervisor_data_file');
    _check_file($form_state, 'note_data_file');
  }

  function _check_file(&$form_state, $field_name) {
    $file = file_save_upload($field_name);
    // check file uploaded OK
    if (!$file) {
      form_set_error($field_name, t('No file was selected (Field: @field)?', array('@field' => $field_name)));
    } elseif ($file->filemime != 'text/csv') {
      form_set_error($field_name, t('The selected file is not a csv one?'));
    }
    else {
      // set files on form_state, to process when form submit runs
      $form_state['values'][$field_name] = $file;
    }
  }

  function urop_import_data_form_submit($form, &$form_state) {
    ini_set('auto_detect_line_endings', TRUE);
    _extract_main_file($form_state);

  }

  function _extract_main_file($form_state) {
    $file_path = $form_state['values']['main_data_file']->filepath;
    $row_count = 0;
    $create_count = 0;
    $import_count = 0;
    if (($handle = fopen($file_path, "r")) !== FALSE) {
      while ($row = fgetcsv($handle, 0, ',')) {
        $row_count++;
        if (($row_count == 1) && (row_value($row, U_FAMILY_NAME) === 'Family Name')) {
          continue; // the first row is full of captions, so move on to next...
        }
        $e_mail = row_value($row, U_EMAIL_PRIVATE);
        if (strlen($e_mail) <= 0) {
          // we prefer the private email to the uni email, but if the private one doesn't exist, we will go with the uni one...
          $e_mail = row_value($row, U_EMAIL_UNI);
        }
        if (strlen($e_mail) <= 0) {
          drupal_set_message(t('Skipping User ID @id as no mail address!', array('@id' => row_value($row, U_ID))), 'status');
          continue; // can't deal with no e-mail address
        }
        $a_user = urop_find_user($e_mail);
        if (!$a_user) {
          // create user...
          $create_count++;
          $name = 'U_' . row_value($row, U_ID);
          $a_user = urop_create_user($name, $e_mail);
        }
        if ($a_user) {
          $import_count++;
//          urop_save_users_personal_details($a_user->uid, $row);
//          urop_save_users_university_details($a_user->uid, $row);
//          urop_restore_users_files($a_user->uid, $row);
        }
      }
      fclose($handle);
    }
    drupal_set_message(t('Number of rows processed: @row_count.', array('@row_count' => $row_count)), 'status');
    drupal_set_message(t('Number of users created: @create_count.', array('@create_count' => $create_count)), 'status');
    drupal_set_message(t('Number of users imported: @import_count.', array('@import_count' => $import_count)), 'status');
  }

  function row_value($row, $column) {
    return trim($row[$column]);
  }

  function urop_find_user($email) {
    return user_load(array('mail' => check_plain($email)));
  }

  function urop_create_user($name, $email) {
    $new_user = array(
      'name' => $name,
      'pass' => user_password(),
      'mail' => $email,
      'status' => 1,
      'init' => $email
    );
    $account = user_save('', $new_user);
    // Terminate if an error occurred during user_save().
    if (!$account) {
      drupal_set_message(t("Error saving user account for user named @name.", array('@name' => $name)), 'error');
    }
    else {
      watchdog('user', 'New user: %name imported into system.',
        array('%name' => $name), WATCHDOG_NOTICE, l(t('edit'), 'user/' . $account->uid . '/edit'));
    }
    return $account;
  }

  function urop_save_users_personal_details($user_uid, $row) {
    // watchdog('urop_import', "Saving student data: @uid.", array('@uid' => $user_uid), WATCHDOG_INFO);
    $table = 'urop_students';
    $key = array(); // default to new record.
    // the following allows us to rerun imports if we 'improve' the code.
    // if this is an existing record, then we should set the key field to use
    $sql = "SELECT COUNT(*) from {urop_students} WHERE uid = '%d'";
    $count = db_result(db_query($sql, $user_uid));
    if ($count > 0) {
      $key[] = 'uid';
    }
    $student = new stdClass();
    $student->uid = $user_uid;
    $student->family_name = row_value($row, U_FAMILY_NAME);
    $student->given_name = row_value($row, U_GIVEN_NAME);
    $student->preferred_name = row_value($row, U_PREFERRED_NAME);
    $student->address_line_1 = row_value($row, U_STREET_ONE);
    $student->address_line_2 = row_value($row, U_STREET_TWO);
    $student->city = row_value($row, U_CITY);
    $student->postcode = row_value($row, U_POSTCODE);
    $student->state = row_value($row, U_STATE);
    $phone = row_value($row, U_MOBILE);
    if (strlen($phone) <= 0) {
      $phone = row_value($row, U_PHONE);
    }
    $student->phone = $phone;
    $sex = 'F'; // one of 'Male' or 'Female'. Default to 'Female'.
    if (strcasecmp(row_value($row, U_GENDER), 'Male') == 0) {
      $sex = 'M';
    }
    $student->gender = $sex;
    $international = 'L'; // default
    if (_urop_is_yes($row, U_INTERNATIONAL)) {
      $international = 'I';
    }
    $student->international = $international;
    $dob = $row[U_DOB];
    $dob = substr($dob, 0, strrpos($dob, ' 00:00:00.0 ') + 1);
    $student->dob = trim($dob);
    $student->status = 'prior';
    if (_urop_is_yes($row, U_INTERVIEW_SUCCESSFUL)) {
      $student->status = 'graduate';
    }
    //dpm($student);
    if (!drupal_write_record($table, $student, $key)) {
      drupal_set_message(t("Error saving student record for user named @name.", array('@name' => $name)), 'error');
      watchdog('user', 'Could not save student record for %name during import into system.',
        array('%name' => $name), WATCHDOG_NOTICE, l(t('edit'), 'user/' . $account->uid . '/edit'));
    }
  }

  function _urop_get_uni_name($row) {
    // choices are: 0, Deakin University, La Trobe University, Monash University,
    // Other (and then, possibly, in the next column, Swinburne,  Swinburne University of Technology, Swinburne University,  Swinburne Hawthorn, Deakin)
    // RMIT,  Swinburne,  University of Melbourne,  Victoria University
    $uni_name = row_value($row, U_UNIVERSITY);
    if ($uni_name === 'Other') {
      $uni_name = row_value($row, U_OTHER_UNIVERSITY);
    }
    switch ($uni_name) {
      case 'Deakin University':
        return 'DEAKIN';
      case 'La Trobe University':
        return 'LATROBE';
      case  'Monash University':
        return 'MONASH';
      case  'RMIT University':
        return 'RMIT';
      case  'Swinburne University of Technology':
        return 'SWINBURNE';
      case  'University of Melbourne':
        return 'MELBOURNE';
      case  'Victoria University':
        return 'VU';

    }
    return FALSE;
  }

  function _urop_get_uni_course($row) {
    /*
     * The course list is stored in a database. At this moment in time we have the following choices set:
     *
     * 1 	Applied science
     * 2 	Arts/Science
     * 3 	Biomedical Engineering
     * 4 	Biomedical Science
     * 5 	Biomedicine
     * 6 	Biotechnology with Honours (Monash)
     * 7 	Commerce
     * 8 	Commerce/IS
     * 9 	Commerce/Science
     * 10 	Law/Science
     * 11 	Science
     *
     * & Other... (1000) if other then the other field is filed in
     * So if not in above list we will default to "other" and bung it in the other box.
     */
    $course = row_value($row, U_COURSE);
    switch ($course) {
      case 'Applied Science':
        return 1;
      case 'Arts/Science':
        return 2;
      case 'Biomedical Engineering':
        return 3;
      case 'Biomedical Science':
        return 4;
      case 'Biomedicine':
        return 5;
      case 'Biotechnology with Honours (Monash)':
        return 6;
      case 'Commerce':
        return 7;
      case 'Commerce/IS':
        return 8;
      case 'Commerce/Science':
        return 9;
      case 'Law/Science':
        return 10;
      case 'Science':
        return 11;
    }
    return 1000; // "Other"
  }


  function urop_save_users_university_details($user_uid, $row) {
    // watchdog('urop_import', "Saving university data: @uid.", array('@uid' => $user_uid), WATCHDOG_INFO);
    $table = 'urop_universities';
    $university = new stdClass();
    $key = array();
    $sql = "SELECT COUNT(*) from {$table} WHERE uid = '%d'";
    $count = db_result(db_query($sql, $user_uid));
    if ($count > 0) {
      $key[] = 'uid';
    }
    $university->uid = $user_uid;
    $uni_name = _urop_get_uni_name($row);
    if ($uni_name) {
      $university->university = $uni_name;
    }
    $university->course = _urop_get_uni_course($row);
    if ("Other Course" === row_value($row, U_COURSE)) {
      $university->course_other = row_value($row, U_COURSE_OTHER);
    }
    else {
      $university->course_other = row_value($row, U_COURSE);
    }
    $university->course_end_year = row_value($row, U_COURSE_COMPLETION_YEAR);
    if ($university->course_end_year > 0) {
      $university->course_start_year = row_value($row, U_COURSE_COMPLETION_YEAR) - row_value($row, U_COURSE_DURATION);
    }
    drupal_write_record($table, $university, $key);
  }

  function _urop_is_yes($row, $field) {
    return strcasecmp($row[$field], 'Yes') == 0;
  }

  function _urop_find_extension($filename) {
    $filename = strtolower($filename);
    $exts = split("[/\\.]", $filename);
    $n = count($exts) - 1;
    $exts = $exts[$n];
    return $exts;
  }

  function _urop_create_user_docs_dir($user_uid) {
    $directory = file_directory_path() . '/' . $user_uid;
    file_check_directory($directory, FILE_CREATE_DIRECTORY);
    return $directory;
  }

  function _urop_create_user_docs_archive_dir($target_dir) {
    $target_dir = $target_dir . '/archive';
    file_check_directory($target_dir, FILE_CREATE_DIRECTORY);
    return $target_dir;
  }

  function _urop_move_file($user_uid, $source_file, $target_file_name) {
      $target_dir = _urop_create_user_docs_dir($user_uid);
      $target_dir = _urop_create_user_docs_archive_dir($target_dir);
      $target_file = $target_dir . '/' . $target_file_name;
      if (!file_move($source_file, $target_file, FILE_EXISTS_REPLACE)) {
        drupal_set_message(t('Could not move @source_file.', array('@source_file' => utf8_encode($source_file))), 'status');
    }
  }

  function _urop_do_archive($source_file_name, $desired_name, $source_dir, $user_uid) {
      if (strlen($source_file_name) > 0) {
        $file_extension = _urop_find_extension($source_file_name);
        $target_file_name = $desired_name . '.' . $file_extension;
        $source_file =  $source_dir . '/' . $source_file_name;
        $source_file = str_replace(':', ',', $source_file);
        $source_file = str_replace('–', '-', $source_file);
        if (!file_exists($source_file)) {
          drupal_set_message(t('Source file @source_file not found?', array('@source_file' => utf8_encode($source_file))), 'status');
          return;
        }
        if (filesize($source_file) === 0) {
          drupal_set_message(t('Source file @source_file 0 bytes in size?', array('@source_file' => utf8_encode($source_file))), 'status');
          return;
        }
        _urop_move_file($user_uid, $source_file, $target_file_name);
      }
  }

  function urop_restore_users_files($user_uid, $row) {
    // watchdog('urop_import', "Moving files: @uid.", array('@uid' => $user_uid), WATCHDOG_INFO);
    $source_dir = file_directory_path() . '/archives/' . row_value($row, U_ID);
    if (file_check_directory($source_dir)) {
      _urop_do_archive(row_value($row, U_FILE_APPLICATION), 'application', $source_dir, $user_uid);
      _urop_do_archive(row_value($row, U_FILE_CV), 'cv', $source_dir, $user_uid);
      _urop_do_archive(row_value($row, U_FILE_RESULTS), 'results', $source_dir, $user_uid);
    }
    else {
      drupal_set_message(t('Source directory @source_dir not found.', array('@source_dir' => $source_dir)), 'status');
    }
  }
