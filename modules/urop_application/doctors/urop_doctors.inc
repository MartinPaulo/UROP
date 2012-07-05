<?php
/**
 * @file
 * This contains a form that allows applicants to pass feedback on their internship back to UROP.
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

function urop_form_medical($user) {
  $result = drupal_get_form('urop_doctors_form', $user->uid);
  $result .= _urop_form_footer();
  return $result;
}

function urop_doctors_form($form_state, $user_uid) {
  $form['uid'] = _urop_user_value($user_uid);
  $form['medical'] = _urop_fieldset('Medical doctors<sup>*</sup>');
  $form['medical']['#description'] = '<sup>*</sup>' . t("For applicable students only!");

  $form['medical']['intern'] = _urop_fieldset('Internship');
  $widget_eb_hospital = _urop_doctors_get_hospital_edit_box();
  $widget_eb_hospital['#required'] = TRUE;
  $widget_eb_hospital['#description'] = t('The hospital at which you did your internship.');
  $form['medical']['intern']['i_hospital'] = $widget_eb_hospital;
  $form['medical']['intern']['year'] = _urop_doctors_year('The year in which you did your internship.');

  $form['medical']['residency'] = _urop_fieldset('Residency');
  $widget_eb_hospital['#description'] = t('The hospital at which you did your residency.');
  $widget_eb_hospital['#required'] = FALSE;
  $form['medical']['residency']['r_hospital'] = $widget_eb_hospital;
  $form['medical']['residency']['year'] =_urop_doctors_year('The year in which you did your residency.');
  $form['medical']['residency']['year']['#required'] = FALSE;

  $form['medical']['speciality'] =_urop_fieldset('Speciality');
  $form['medical']['speciality']['#collapsible'] = TRUE;
  $form['medical']['speciality']['#collapsed'] = TRUE;
  $form['medical']['speciality']['name'] = array(
          '#type' => 'textfield',
          '#title' => t('Name'),
          //'#default_value' => $student ? $student->family_name : '',
          '#description' => t('Your speciality (if any).'),
          '#size' => 50,
          '#maxlength' => 50,
  );
  $widget_eb_hospital['#description'] = t('The hospital at which you did your speciality.');
  $form['medical']['speciality']['s_hospital'] = $widget_eb_hospital;
  $widget_tb = _urop_text_area('Description');
  $widget_tb['#description'] = t('Please describe your speciality.');
  $form['medical']['speciality']['description'] = $widget_tb;

  $form['medical']['research'] =_urop_fieldset('Research');
  $form['medical']['research']['#collapsible'] = TRUE;
  $form['medical']['research']['#collapsed'] = TRUE;
  $form['medical']['research']['#description'] = 'Have you been involved in any research?';
  $widget_tb = _urop_text_area('Description');
  $form['medical']['research']['involved'] = array(
      '#type' => 'radios',
      '#title' => "Research involvement",
      '#required' => TRUE,
      '#options' => array('Y' => t('Yes'), 'N' => t('No')),
      //'#default_value' =>  $student ? $student->gender : '',
  );
  $widget_tb['#description'] = t('If yes, please describe your research involvment.');
  $form['medical']['research']['description'] = $widget_tb;

  $form['submit'] = _urop_submit();
  return $form;
}

function _urop_doctors_get_hospital_edit_box() {
  return _urop_edit_box('Hospital');
}

function _urop_doctors_year($description) {
  return array(
        '#type' => 'select',
        '#title' => t('Year'),
        '#description' => t($description),
        '#options' => _urop_year_list(),
        '#default_value' => '2012',
        // 	'#default_value' => $university ? $university->course_start_year : '2012',
        '#required' => TRUE,
  );
}

function urop_form_medical_validate($form, &$form_state) {
  // if this is the admin user, then we need to use the correct user id:
  // not the global user, but rather the one that was passed in via the url!
  $user_uid = $form_state['values']['uid'];
}

function urop_form_medical_submit($form, &$form_state) {
  // if this is the admin user, then we need to use the correct user id:
  // not the global user, but rather the one that was passed in via the url!
  $user_uid = $form_state['values']['uid'];
}
