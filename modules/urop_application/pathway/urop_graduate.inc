<?php
/**
 * @file
 * Contains a form that allows the user to describe their graduate career.
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

function urop_graduate_form($form_state, $user_uid) {
  $form['graduate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Graduate study'),
    '#description' => t('Including any professional degrees (e.g.: Dr. of Medicine, and also any graduate diplomas.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['graduate']['articles'] = array(
      '#type' => 'multicrud',
      '#multicrud_elements' => array(
         'degree_name' => array(
            '#title' => t('Degree/Diploma name'),
            '#type' => 'textfield',
            '#size' => 50,
  ),
          'degree_abbreviation' => array(
            '#title' => t('Abbreviation'),
            '#type' => 'textfield',
            '#size' => 5,
  ),
         'start_year' => array(
            '#title' => t('Start year'),
            '#type' => 'select',
            '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
  ),
        'end_year' => array(
            '#title' => t('Completion year'),
            '#type' => 'select',
            '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
  ),
         'pid' => array(
            '#type' => 'value',
  ),
  ),
       //'#default_value' => urop_universitys_get_values($user_uid),
  );
  $form['graduate']['submit'] = _urop_submit();
  return $form;
}
