<?php
/**
 * @file
 * Contains a form that allows the user to describe their honours
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

function urop_honours_form($form_state, $user_uid) {
  $form['honours'] = array(
    '#type' => 'fieldset',
    '#title' => t('Honours (and co-supervisors)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['honours']['university'] = array(
      '#type' => 'textfield',
      '#title' => t('Institution'),
      '#required' => FALSE,
      '#description' => "The name of the institution you received your honours from.",
      '#autocomplete_path' => 'urop/autocomplete/uni',
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['honours']['year'] = array(
      '#type' => 'select',
      '#title' => "Year",
      '#description' => 'The year in which you recieved your honours.',
      '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
      '#required' => FALSE,
  );
  $form['honours']['supervisor'] = array(
      '#type' => 'textfield',
      '#title' => t('Supervisor'),
      '#description' => "The name of your honours supervisor.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['honours']['supervisor_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Supervisor email'),
      '#description' => "Your supervisors current email address.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['honours']['department'] = array(
      '#type' => 'textfield',
      '#title' => t('University enrolment department'),
      '#description' => "Your university enrolment department.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['honours']['laboratory'] = array(
      '#type' => 'textfield',
      '#title' => t('Laboratory name'),
      '#description' => "The name of the laboratory.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['honours']['submit'] = _urop_submit();
  return $form;
}
