<?php
  /**
   * @file
   * This file contains information about the UROP supervisors.
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
  function urop_build_supervisor_form($user) {
    $sql = "SELECT * from {urop_supervisors} WHERE uid = '%d'";
    $supervisor = db_fetch_object(db_query(db_rewrite_sql($sql), $user->uid));
    $result = drupal_get_form('urop_supervisor_form', $supervisor, $user->uid);
    $result .= _urop_form_footer();
    return $result;
  }

  /**
   * First and Surname
   * Position
   * Institution
   * Laboratory/Department
   * Address
   * Phone
   * @param $form_state
   * @param $supervisor
   * @param $user_uid
   */
  function urop_supervisor_form($form_state, $supervisor, $user_uid) {
    $form['uid'] = _urop_user_value($user_uid);
    $form['supervisor'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your details'),
    );
    $form['supervisor']['family'] = array(
      '#type' => 'textfield',
      '#title' => t('Family name'),
      '#required' => TRUE,
      '#default_value' => $supervisor ? $supervisor->family_name : '',
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['supervisor']['given'] = array(
      '#type' => 'textfield',
      '#title' => t('Given names'),
      '#required' => TRUE,
      '#default_value' => $supervisor ? $supervisor->given_name : '',
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['supervisor']['position'] = array(
      '#type' => 'textfield',
      '#title' => t('Position'),
      '#description' => "Your position.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->position : '',
    );
    $form['supervisor']['institution'] = array(
      '#type' => 'textfield',
      '#title' => t('Institution'),
      '#description' => "Your institution.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->institution : '',
    );
    $form['supervisor']['laboratory'] = array(
      '#type' => 'textfield',
      '#title' => t('Laboratory/Department'),
      '#description' => "Your laboratory/department.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->laboratory : '',
    );
    $form['supervisor']['address'] = array(
      '#type' => 'fieldset',
      '#title' => t('Institution address.'),
    );
    $form['supervisor']['address']['address_line_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Street address 1'),
      '#description' => "The first line of your institution's street address.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->address_line_1 : '',
    );
    $form['supervisor']['address']['address_line_2'] = array(
      '#type' => 'textfield',
      '#title' => t('Street address 2'),
      '#description' => "The second line of your institution's street address.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->address_line_2 : '',
    );
    $form['supervisor']['address']['address_city'] = array(
      '#type' => 'textfield',
      '#title' => t('City/Suburb'),
      '#description' => "The city or suburb that matches your institution's postcode/zip.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->city : '',
    );
    $form['supervisor']['address']['address_postcode'] = array(
      '#type' => 'textfield',
      '#title' => t('Postcode/Zip'),
      '#description' => "The postcode/zip of your institution's street address.",
      '#required' => TRUE,
      '#size' => 5,
      '#maxlength' => 5,
      '#default_value' => $supervisor ? $supervisor->postcode : '',
    );
    $form['supervisor']['address']['address_state'] = array(
      '#type' => 'select',
      '#title' => t('State/Region'),
      '#description' => "The state/region in which your institution's street address is located.",
      '#required' => TRUE,
      '#options' => urop_application_get_list_of_states(),
      '#default_value' => $supervisor ? $supervisor->state : 'VIC',
    );
    $form['supervisor']['phone'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact phone'),
      '#description' => "Your work (or mobile) phone number.",
      '#required' => TRUE,
      '#size' => 50,
      '#maxlength' => 50,
      '#default_value' => $supervisor ? $supervisor->phone : '',
    );
    $form['submit'] = _urop_submit();
    return $form;
  }

  function urop_application_personal_details_form_validate($form, &$form_state) {
    if (!urop_is_valid_phone($form_state['values']['phone'])) {
      form_set_error('phone', 'Please enter a valid phone number.');
    }
  }

  function urop_supervisor_form_submit($form, &$form_state) {
    // if this is the admin user, then we need to use the correct user id:
    // not the global user, but rather the one that was passed in via the url!
    $user_uid = $form_state['values']['uid'];
    $table = 'urop_supervisors';
    $supervisor = new stdClass();
    $key = array();
    // if this is an existing record, then we should set the key field to use
    $sql = "SELECT COUNT(*) from {urop_supervisors} WHERE uid = '%d'";
    $count = db_result(db_query($sql, $user_uid));
    if ($count > 0) {
      $key[] = 'uid';
    }
    $supervisor->uid = $user_uid;
    $supervisor->family_name = $form_state['values']['family'];
    $supervisor->given_name = $form_state['values']['given'];
    $supervisor->position = $form_state['values']['position'];
    // This should be in a separate table, but being bound by data format of old system and time, dirty it is :(
    $supervisor->institution = $form_state['values']['institution'];
    $supervisor->laboratory = $form_state['values']['laboratory'];
    $supervisor->address_line_1 = $form_state['values']['address_line_1'];
    $supervisor->address_line_2 = $form_state['values']['address_line_2'];
    $supervisor->city = $form_state['values']['address_city'];
    $supervisor->postcode = $form_state['values']['address_postcode'];
    $supervisor->state = $form_state['values']['address_state'];
    $supervisor->phone = $form_state['values']['phone'];

    drupal_write_record($table, $supervisor, $key);
    drupal_set_message(t('Your details have been updated.'));
  }
