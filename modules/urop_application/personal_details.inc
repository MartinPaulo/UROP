<?php
  /**
   * @file
   * This contains a form that enables applicants to manage their personal details.
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

  /**
   * @param User $user   The drupal user loaded from the url (not necessarily the logged in user)
   * @return string The form to describe the urop students personal details.
   */
  function urop_application_form($user) {
    $sql = "SELECT * from {urop_students} WHERE uid = '%d'";
    $student = db_fetch_object(db_query(db_rewrite_sql($sql), $user->uid));
    $result = drupal_get_form('urop_application_personal_details_form', $student, $user->uid);
    $result .= _urop_form_footer();
    return $result;
  }

  // See: http://www.w3.org/International/questions/qa-personal-names
  function urop_application_personal_details_form($form_state, $student, $user_uid) {
    $form['uid'] = _urop_user_value($user_uid);
    // This is the first form element. It's a textfield with a label, "Name"
    $form['name'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your personal details'),
    );
    $form['name']['family'] = array(
      '#type' => 'textfield',
      '#title' => t('Family name'),
      '#required' => TRUE,
      '#default_value' => $student ? $student->family_name : '',
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['name']['given'] = array(
      '#type' => 'textfield',
      '#title' => t('Given names'),
      '#required' => TRUE,
      '#default_value' => $student ? $student->given_name : '',
      '#size' => 50,
      '#maxlength' => 50,
    );
    // any search must give priority to preferred name over given name if this field is completed.
    $form['name']['preferred'] = array(
      '#type' => 'textfield',
      '#title' => t('Preferred name'),
      '#description' => "What should we call you (for example, when we send you mail)?",
      '#required' => FALSE,
      '#default_value' => $student ? $student->preferred_name : '',
      '#size' => 50,
      '#maxlength' => 50,
    );

    $form['name']['gender'] = array(
      '#type' => 'radios',
      '#title' => "Gender",
      '#description' => 'Your gender.',
      '#required' => TRUE,
      '#options' => array('F' => t('Female'), 'M' => t('Male')),
      '#default_value' => $student ? $student->gender : '',
    );

    $dob = $student ? strtotime($student->dob) : time();
    $form['name']['date_of_birth'] = array(
      '#type' => 'date',
      '#title' => "Date of birth",
      '#description' => 'The month, day and year in which you were born.',
      '#required' => TRUE,
      '#default_value' => array(
        'day' => format_date($dob, 'custom', 'j'),
        'month' => format_date($dob, 'custom', 'n'),
        'year' => format_date($dob, 'custom', 'Y'),
      ),
    );
    $form['name']['international'] = array(
      '#type' => 'radios',
      '#title' => "Are you an Local or International student",
      '#description' => 'Your Local/International status does not affect your eligibility to be selected for UROP.',
      '#required' => TRUE,
      '#options' => array('L' => t('Local'), 'I' => t('International')),
      '#default_value' => $student ? $student->international : '',
    );
    $form['name']['address'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your address in Australia during semester.'),
    );
    $form['name']['address']['address_line_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Street address 1'),
      '#description' => "The first line of your street address.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $student ? $student->address_line_1 : '',
    );
    $form['name']['address']['address_line_2'] = array(
      '#type' => 'textfield',
      '#title' => t('Street address 2'),
      '#description' => "The second line of your street address.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $student ? $student->address_line_2 : '',
    );
    $form['name']['address']['address_city'] = array(
      '#type' => 'textfield',
      '#title' => t('City/Suburb'),
      '#description' => "The city or suburb that matches your postcode/zip.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $student ? $student->city : '',
    );
    $form['name']['address']['address_postcode'] = array(
      '#type' => 'textfield',
      '#title' => t('Postcode/Zip'),
      '#description' => "The postcode/zip of your street address.",
      '#required' => TRUE,
      '#size' => 5,
      '#maxlength' => 5,
      '#default_value' => $student ? $student->postcode : '',
    );
    $form['name']['address']['address_state'] = array(
      '#type' => 'select',
      '#title' => t('State/Region'),
      '#description' => "The state/region in which your street address is located.",
      '#required' => TRUE,
      '#options' => urop_application_get_list_of_states(),
      '#default_value' => $student ? $student->state : 'VIC',
    );
    $form['name']['phone'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact phone'),
      '#description' => "Your mobile or home phone number.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $student ? $student->phone : '',
    );

    // Adds a simple submit button that refreshes the form and clears its contents
    //-- this is the default behavior for forms.
    $form['submit'] = _urop_submit();
    return $form;
  }

  // This adds a handler/function to validate the data entered into the
  // "year of birth" field to make sure it's between the values of 1900
  // and 2000. If not, it displays an error.
  // The value report is $form_state['values'] (see http&#58;//drupal.org/node/144132#form-state).
  function urop_application_personal_details_form_validate($form, &$form_state) {
    //$date_of_birth = $form_state['values']['date_of_birth'];
    //    if ($year_of_birth && ($year_of_birth < 1900 || $year_of_birth > 2000)) {
    //    form_set_error('year_of_birth', 'Enter a year between 1900 and 2000.');
    //    }
    if (!urop_is_valid_phone($form_state['values']['phone'])) {
      form_set_error('phone', 'Please enter a valid phone number.');
    }
  }

  function urop_application_personal_details_form_submit($form, &$form_state) {
    // if this is the admin user, then we need to use the correct user id:
    // not the global user, but rather the one that was passed in via the url!
    $user_uid = $form_state['values']['uid'];
    $table = 'urop_students';
    $student = new stdClass();
    $key = array();
    // if this is an existing record, then we should set the key field to use
    $sql = "SELECT COUNT(*) from {urop_students} WHERE uid = '%d'";
    $count = db_result(db_query($sql, $user_uid));
    if ($count > 0) {
      $key[] = 'uid';
    }
    $student->uid = $user_uid;
    $student->family_name = $form_state['values']['family'];
    $student->given_name = $form_state['values']['given'];
    $student->preferred_name = $form_state['values']['preferred'];
    $student->address_line_1 = $form_state['values']['address_line_1'];
    $student->address_line_2 = $form_state['values']['address_line_2'];
    $student->city = $form_state['values']['address_city'];
    $student->postcode = $form_state['values']['address_postcode'];
    $student->state = $form_state['values']['address_state'];
    $student->phone = $form_state['values']['phone'];
    $student->gender = $form_state['values']['gender'];
    $student->international = $form_state['values']['international'];
    $student->dob = '' . $form_state['values']['date_of_birth']['year'] . '-' .
      $form_state['values']['date_of_birth']['month'] . '-' .
      $form_state['values']['date_of_birth']['day'];

    drupal_write_record($table, $student, $key);
    drupal_set_message(t('Your personal details have been updated.'));
    if (urop_application_is_ready($user_uid)) {
      $form_state['redirect'] = 'urop/' . $user_uid . '/submit';
    }
    else {
      $form_state['redirect'] = 'urop/' . $user_uid . '/university';
    }
  }


