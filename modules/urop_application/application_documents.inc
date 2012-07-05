<?php
  /**
   * @file
   * This contains a form that allows applicants to update and view their
   * documentation.
   *
   * Author: Martin Paulo
   *
   * This file is part of UROP's student application module.
   *
   * UROP's student application module is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * (at your option) any later version.
   *
   * UROP's student application module is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with UROP's student application module.  If not, see <http://www.gnu.org/licenses/>.
   */

  function urop_application_documents_form($user) {
    // following has html elements and no user input, so check_plain not needed...
    $result = urop_application_get_documentation_needed($user->uid);
    $result .= urop_application_show_current_documents($user->uid);
    $result .= drupal_get_form('urop_application_documents_upload_form', $user->uid);
    $result .= _urop_form_footer();
    return $result;
  }

  /**
   * @param User $user_uid The user id for whose base document directory is to be returned.
   * @return string The directory that is to act as the base for the users document collection. Will be
   * created if it doesn't exist.
   */
  function urop_application_get_document_base_dir($user_uid) {
    //$current_round_tag = _urop_get_current_round_tag(); . $current_round_tag . '/'
    $directory = file_directory_path() . '/' . $user_uid;
    file_check_directory($directory, FILE_CREATE_DIRECTORY);
    return $directory;
  }

  function urop_application_get_approved_formats() {
    return t('Accepted formats are plain text, ms-word, ' .
      '<a href="http://www.adobe.com/products/acrobat/adobepdf.html" target=""_blank">pdf</a>' .
      ' and <a href="http://en.wikipedia.org/wiki/OpenDocument" target="_blank">odf</a>.');
  }

  function urop_application_get_current_documents($user_uid) {
    $source_dir = urop_application_get_document_base_dir($user_uid);
    return file_scan_directory($source_dir, '.*', array('.', '..', 'CVS'), 0, TRUE, 'filename');
  }

  function urop_application_show_current_documents($user_uid) {
    $files = urop_application_get_current_documents($user_uid);
    $result = '';
    if (count($files) > 0) {
      $result = '<p>Uploaded Documents</p>';
      $result = $result . '<ul>';
      foreach ($files as $k => $file) {
        $result = $result . '<li><a href="' . file_create_url($k) . '">' . str_replace(urop_application_get_document_base_dir($user_uid) . '/', '', $file->filename) . '</a></li>';
      }
      $result = $result . '</ul>';
    }
    return $result;
  }

  function urop_application_documents_upload_form($form_state, $user_uid) {
    $form['#attributes']['enctype'] = 'multipart/form-data';
    $form['uid'] = _urop_user_value($user_uid);
    $form['documents'] = array(
      '#type' => 'fieldset',
      '#title' => t('Required documentation'),
      '#collapsible' => !urop_application_is_documentation_needed($user_uid),
      '#collapsed' => !urop_application_is_documentation_needed($user_uid),
      '#description' => t('Please ensure that you have read and understood the ' .
        'application instructions before submitting these documents.')
    );
    $form['documents']['application_upload'] = array(
      '#title' => t('Letter of application'),
      '#type' => 'file',
      '#description' => t('Your covering letter. ' . urop_application_get_approved_formats()),
    );
    $form['documents']['cv_upload'] = array(
      '#title' => t('CV/resume'),
      '#type' => 'file',
      '#description' => t('Your CV/resume. ' . urop_application_get_approved_formats()),
    );
    $form['documents']['results_upload'] = array(
      '#title' => t('Electronic results'),
      '#type' => 'file',
      '#description' => t('Your electronic results record. ' . urop_application_get_approved_formats()),
    );
    $form['documents']['photo_upload'] = array(
      '#title' => t('Photograph'),
      '#type' => 'file',
      '#description' => t('A passport style photograph of you. The image will be resized to %dimensions and the maximum size is %size kB. Accepted formats are jpeg, png or gif.', array('%dimensions' => '240x320', '%size' => '300')),
    );
    $form['documents']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#name' => 'submit',
      '#ahah' => array(
        'type' => 'bar',
        'message' => t('Please wait...'),
      )
    );
    if (urop_application_not_editable($user_uid)) {
      $form['documents']['submit']['#disabled'] = TRUE;
      $form['documents']['logic'] = array(
        '#value' => DISABLED_MESSAGE,
      );
    }

    return $form;
  }


  function urop_application_documents_upload_form_validate($form, &$form_state) {
    // if this is the admin user, then we need to use the correct user id:
    // not the global user, but rather the one that was passed in via the url!
    $user_uid = $form_state['values']['uid'];
    $destination_dir = urop_application_get_document_base_dir($user_uid);
    $destination_dir = $destination_dir . '/' . _urop_get_current_round_tag();
    if (!file_check_directory($destination_dir, FILE_CREATE_DIRECTORY)) {
      drupal_set_message(check_plain(t('The destination path ' . $destination_dir . ' could not be created.')), 'error');
      return;
    }

    urop_application_upload_file($destination_dir, 'cv_upload', 'cv_' . $user_uid);
    urop_application_upload_file($destination_dir, 'results_upload', 'results_' . $user_uid);
    urop_application_upload_file($destination_dir, 'application_upload', 'application_' . $user_uid);
    urop_application_upload_image($destination_dir, 'photo_upload', 'photograph_' . $user_uid);
  }

  function urop_application_documents_upload_form_submit($form, &$form_state) {
    $user_uid = $form_state['values']['uid'];
    if (urop_application_is_ready($user_uid)) {
      $form_state['redirect'] = 'urop/' . $user_uid . '/submit';
    }
    elseif (!urop_application_is_documentation_needed($user_uid)) {
      $form_state['redirect'] = UROP_HOME_URL;
    }
  }

  // maybe should rewrite to use pattern at:
  // http://api.drupal.org/api/drupal/modules!user!user.module/function/user_validate_picture/6
  // TODO: combine with method below...
  function urop_application_upload_image($destination_dir, $field_name, $desired_name) {
    $user_file_name = $_FILES['files']['name'][$field_name];
    if (strlen($user_file_name) <= 0) {
      return FALSE;
    }
    // delete any other files that may exist for this desired upload
    file_scan_directory($destination_dir, $desired_name . '.*', array('.', '..', 'CVS'), 'file_delete', FALSE);
    // Remember: not tested for admin user by drupal
    $validators = array(
      'file_validate_is_image' => array(),
      'file_validate_image_resolution' => array('240x320'),
      'file_validate_size' => array(300 * 1024), // want a 300kb image file size
    );
    $file_extension = urop_application_find_extension($user_file_name);
    $uploaded_file_name = $desired_name . '.' . $file_extension;
    $_FILES['files']['name'][$field_name] = $uploaded_file_name;
    $file = file_save_upload($field_name, $validators, $destination_dir, FILE_EXISTS_REPLACE);
    if ($file) {
      file_set_status($file, FILE_STATUS_PERMANENT);
      drupal_set_message(check_plain("Successfully uploaded $user_file_name as '$uploaded_file_name'."));
      return TRUE;
    }
    else {
      drupal_set_message(check_plain("Failed to upload your $desired_name."), 'error');
    }
    return FALSE;
  }

  function urop_application_upload_file($destination_dir, $field_name, $desired_name) {
    $user_file_name = $_FILES['files']['name'][$field_name];
    if (strlen($user_file_name) <= 0) {
      return FALSE;
    }
    // delete any other files that may exist for this desired upload
    file_scan_directory($destination_dir, $desired_name . '.*', array('.', '..', 'CVS'), 'file_delete', FALSE);
    // Remember: not tested for admin user by drupal
    $validators = array(
      'file_validate_extensions' => array('pdf txt doc docx odf odt'),
    );
    $file_extension = urop_application_find_extension($user_file_name);
    $uploaded_file_name = $desired_name . '.' . $file_extension;
    $_FILES['files']['name'][$field_name] = $uploaded_file_name;
    $file = file_save_upload($field_name, $validators, $destination_dir, FILE_EXISTS_REPLACE);
    if ($file) {
      file_set_status($file, FILE_STATUS_PERMANENT);
      drupal_set_message(check_plain("Successfully uploaded $user_file_name as '$uploaded_file_name'."));
      return TRUE;
    }
    else {
      drupal_set_message(check_plain("Failed to upload your $desired_name.", 'error'));
    }
    return FALSE;
  }

  function urop_application_find_extension($filename) {
    $filename = strtolower($filename);
    $exts = split("[/\\.]", $filename);
    $n = count($exts) - 1;
    $exts = $exts[$n];
    return $exts;
  }

  /**
   * @param User $user_uid The user who's documentation is being uploaded.
   * @return string An HTML string (make sure no user input is included!) that lists the files still needed by the user
   * passed in as an argument.
   */
  function urop_application_get_documentation_needed($user_uid) {
    if (!urop_application_is_documentation_needed($user_uid)) {
      $result = '<div class="messages status">';
      $result .= '<p>You have provided all of the required supporting documentation.</p>';
      $result .= '</div>';
      return $result;
    }
    // need to see what files are still to be provided
    $source_dir = file_directory_path() . '/' . $user_uid . '/' . _urop_get_current_round_tag();
    $files = file_scan_directory($source_dir, '.*', array('pdf', 'txt', 'doc', 'docx', 'odf', 'odt'), 0, TRUE, 'filename');
    $required_files = array('cv', 'results', 'application', 'photograph');
    foreach ($files as $file) {
      $file_list = array('cv', 'results', 'application', 'photograph');
      $key = 0;
      foreach ($file_list as $required_file) {
        // if the file name is in the list or required files
        if ($file->name == $required_file) {
          unset($required_files[$key]);
          break;
        }
        $key += 1;
      }
    }
    $result = '<div class="messages status">';
    $result .= '<p>You still need to provide the following:</p>';
    $result .= '<ul>';
    foreach ($required_files as $missing_file) {
      $result .= '<li>';
      $result .= $missing_file;
      $result .= '</li>';
    }
    $result .= '</div>';
    return $result;
  }
