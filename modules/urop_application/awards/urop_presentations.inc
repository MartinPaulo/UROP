<?php
/**
 * @file
 * This contains a form that allows applicants to enter the presentations/posters, etc. done post UROP.
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

function urop_presentation_details_form($form_state, $user_uid) {
  $form['presentation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Presentations/Posters'),
    '#description' => t('Any presentations/posters that you might have done since you left UROP.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['presentation']['presentations'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
      'prize_name' => array(
        '#title' => t('Name'),
        '#type' => 'textfield',
        '#size' => 30,
  ),
      'prize_uni' => array(
        '#title' => t('Conference'),
        '#type' => 'textfield',
        '#size' => 30,
  ),
      'prize_date' => array(
        '#type' => 'select',
        '#title' => t('Year'),
        '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
  ),
  'prize_month' => array(
            '#type' => 'select',
            '#title' => t('Month'),
            '#options' => _urop_month_list(),
            '#default_value' => '1',
  ),
  ),
    '#default_value' => array(
      '1' => array(
        'prize_name' => 'Software enlightenment',
        'prize_uni' => 'YOW',
        'prize_date' => '2011',
  ),
      '2' => array(
        'prize_name' => 'The mice are running the experiment',
        'prize_uni' => 'JAOO',
        'prize_date' => '2010',
  ),
  ),
  );
  $form['presentation']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}
