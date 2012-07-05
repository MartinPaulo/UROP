<?php
/**
 * @file
 * This contains a form that allows applicants to enter their non scholarly articles, etc. done post UROP.
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

function urop_article_details_form($form_state, $user_uid) {
  $form['article'] = array(
    '#type' => 'fieldset',
    '#title' => t('Non scholarly publications'),
    '#description' => t('Any non scholarly articles that relate to your UROP work.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['article']['articles'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
      'name' => array(
        '#title' => t('Name of article'),
        '#type' => 'textfield',
        '#size' => 30,
      ),
      'details' => array(
        '#title' => t('Where published'),
        '#type' => 'textfield',
        '#size' => 30,
      ),
      'year' => array(
        '#type' => 'select',
        '#title' => t('Year'),
        '#options' => _urop_year_list(),
        '#default_value' => '2012',
//   '#default_value' => $university ? $university->course_start_year : '2012',
      ),
      'month' => array(
        '#type' => 'select',
        '#title' => t('Month'),
        '#options' => _urop_month_list(),
        '#default_value' => '1',
      ),
      'type' => array(
        '#title' => t('Publication type'),
        '#type' => 'textfield',
        '#size' => 30,
      ),
    ),
    '#default_value' => array(
      '1' => array(
        'name' => 'PC Magazine',
        'details' => 'How to defrudle a hard drive.',
        'date' => '2011',
        'type' => 'Magazine'
      ),
      '2' => array(
        'name' => 'PC Techniques',
        'details' => 'Modern software design',
        'date' => '2010',
        'type' => 'Website',
      ),
    ),
  );
  $form['article']['submit'] = _urop_submit();
  return $form;
}
