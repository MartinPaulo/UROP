<?php
/**
 * @file
 * Contains a form that allows the user to describe their past jobs.
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

function urop_jobs_form($form_state, $user_uid) {
  $form['uid'] = array(
      "#type" => 'value',
      "#value" => $user_uid,
  );
  $form['jobs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Jobs'),
    '#description' => t('Any jobs that you have held since you left UROP.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['jobs']['articles'] = array(
      '#type' => 'multicrud',
      '#multicrud_elements' => array(
      'commenced' => array(
        '#title' => t('Start date'),
          '#type' => 'textfield',
          '#size' => 4,
        ),
        'terminated' => array(
        '#title' => t('End date'),
          '#type' => 'textfield',
          '#size' => 4,
        ),
        'employer' => array(
        '#title' => t('Employer name'),
          '#type' => 'textfield',
          '#size' => 50,
          '#required' => TRUE,
        ),
        'employer_desc' => array(
        '#title' => t('Employer description'),
          '#type' => 'textfield',
          '#size' => 50,
        ),
        'job' => array(
          '#title' => t('Job title'),
          '#type' => 'textfield',
          '#size' => 50,
        ),
        'job_desc' => array(
          '#title' => t('Job description'),
          '#type' => 'textfield',
          '#size' => 50,
        ),
        'pid' => array(
        '#type' => 'value',
        ),
      ),
    //'#default_value' => urop_jobss_get_values($user_uid),
    );
  $form['jobs']['submit'] = _urop_submit();
  return $form;
}
