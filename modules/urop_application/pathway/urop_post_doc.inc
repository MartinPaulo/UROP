<?php
/**
 * @file
 * Contains a form that allows the user to describe their postdoctoral research.
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
function urop_post_doc_form($form_state, $user_uid) {
  $form['post_doc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Postdoctoral'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['post_doc']['university'] = array(
      '#type' => 'textfield',
      '#title' => t('Institution'),
      '#required' => FALSE,
      '#description' => "The name of the institution where you performed your Postdoctoral research.",
      '#autocomplete_path' => 'urop/autocomplete/uni',
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['post_doc']['start'] = array(
        '#type' => 'select',
        '#title' => "Start date",
        '#description' => 'The year in which you started your Postdoctoral research.',
        '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
        '#required' => FALSE,
  );
  $form['post_doc']['end'] = array(
      '#type' => 'select',
      '#title' => "End date",
      '#description' => 'The year in which you finished your Postdoctoral research.',
      '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
      '#required' => FALSE,
  );
  $form['post_doc']['supervisor'] = array(
      '#type' => 'textfield',
      '#title' => t('Supervisor'),
      '#description' => "The name of your supervisor.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['post_doc']['supervisor_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Supervisor email'),
      '#description' => "Your supervisors current email address.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['post_doc']['laboratory'] = array(
      '#type' => 'textfield',
      '#title' => t('Laboratory name'),
      '#description' => "The name of the laboratory where you performed your Postdoctoral research.",
      '#required' => FALSE,
      '#size' => 50,
      '#maxlength' => 50,
  );
  $form['post_doc']['submit'] = _urop_submit();
  return $form;
}
