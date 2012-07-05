<?php
/**
 * @file
 * Contains a form that allows the user to describe their patents received post UROP.
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

function urop_patent_details_form($form_state, $user_uid) {
  $form['patent'] = array(
    '#type' => 'fieldset',
    '#title' => t('Patents'),
    '#description' => t('Any patents that you might have made since you left UROP.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['patent']['patents'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
      'prize_name' => array(
      '#title' => t('Name'),
      '#type' => 'textfield',
      '#size' => 30,
      ),
    'prize_date' => array(
      '#type' => 'select',
      '#title' => 'Date',
      '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
       ),
     'prize_uni' => array(
        '#title' => t('Collaborators.'),
        '#type' => 'textfield',
        '#size' => 30,
       ),
     ),
    '#default_value' => array(
        '1' => array(
        'prize_name' => 'Quantum entanglement',
        'prize_date' => '2011',
        'prize_uni' => 'A. Einstein, E. Rutherford',
      ),
        '2' => array(
        'prize_name' => 'New Relativity',
        'prize_uni' => 'N. Bohr',
        'prize_date' => '2008',
      ),
    ),
  );
  $form['patent']['submit'] = _urop_submit();
  return $form;
}


