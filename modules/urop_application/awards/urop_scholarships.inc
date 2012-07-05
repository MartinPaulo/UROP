<?php
/**
 * @file
 * This contains a form that allows applicants to enter their awards, etc...
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


function urop_phd_scholarship_details_form($form_state, $user_uid) {
  $form['uid'] = _urop_user_value($user_uid);
  $form['phd'] = array(
    '#type' => 'fieldset',
    '#title' => t('PhD scholarships'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => '<p>' . t('Details of the PhD scholarships that you have received.'),
  );
  $form['phd']['collection'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
        'name' => array(
          '#title' => t('Scholarship name'),
          '#type' => 'textfield',
          '#size' => 30,
          '#maxlength' => 30,
        ),
         'university' => array(
          '#title' => t('Conferring institution'),
          '#type' => 'textfield',
          '#size' => 30,
          '#maxlength' => 30,
          //'#autocomplete_path' => 'urop/autocomplete/uni',
        ),
        'date' => array(
          '#type' => 'select',
          '#title' => 'Year',
          '#options' => _urop_year_list(),
  '#default_value' => '2012',
  //   '#default_value' => $university ? $university->course_start_year : '2012',
        ),
        'duration' => array(
          '#type' => 'textfield',
          '#title' => t('Duration'),
          '#size' => 3,
          '#maxlength' => 3,
        ),
        'value' => array(
          '#type' => 'textfield',
          '#title' => t('Value'),
          '#size' => 3,
          '#maxlength' => 3,
        ),
      ),
    );
  $form['phd']['submit'] = _urop_submit();
  return $form;
}
