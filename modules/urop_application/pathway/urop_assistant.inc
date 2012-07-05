<?php
/**
 * @file
 * This builds a set of forms that allows applicants to describe the research institute they were an assistant at.
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
function urop_assistant_form($form_state, $user_uid) {
  $form['assistant'] = array(
    '#type' => 'fieldset',
    '#title' => t('Research Assistant'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['assistant']['university'] = array(
      '#type' => 'textfield',
      '#title' => t('Institution'),
      '#required' => FALSE,
      '#description' => "The name of the institution you were a research assistant at.",
      '#autocomplete_path' => 'urop/autocomplete/uni',
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['assistant']['year'] = array(
      '#type' => 'select',
      '#title' => "Year",
      '#description' => 'The year in which you were a research assistant.',
      '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
      '#required' => FALSE,
  );
  $form['assistant']['supervisor'] = array(
      '#type' => 'textfield',
      '#title' => t('Supervisor'),
      '#description' => "The name of your supervisor.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['assistant']['supervisor_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Supervisor email'),
      '#description' => "Your supervisors current email address.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['assistant']['laboratory'] = array(
      '#type' => 'textfield',
      '#title' => t('Laboratory name'),
      '#description' => "The name of the laboratory where you were a research assistant.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['assistant']['submit'] = _urop_submit();
  return $form;
}
