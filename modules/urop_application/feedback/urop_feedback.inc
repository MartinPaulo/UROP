<?php
/**
 * @file
 * This contains a form that allows applicants to pass general feedback back to UROP.
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

function urop_form_feedback($user) {
  $result = drupal_get_form('urop_feedback_form', $user->uid);
  $result .= _urop_form_footer();
  return $result;
}

function urop_feedback_form($form_state, $user_uid) {

  $form['feedback'] = array(
      '#type' => 'textarea',
      '#title' => t('Your feedback'),
      '#description' => "Please provide any feedback about the UROP program and your experience of it.",
      '#required' => TRUE,
  );

  $form['submit'] = _urop_submit();
  // for reset have a look at:
  // http://devengineering.com/best-practices/drupal/cancellation-required-validation-drupal-6
  return $form;
}

function urop_feedback_form_submit($form, &$form_state) {
  $params['feedback'] = $form_state['values']['feedback'];
  $mail_to = variable_get('urop_feedback_email', 'urop-bio21@unimelb.edu.au');
  drupal_mail('feedback', 'feedback', $mail_to, language_default(), $params);
  drupal_set_message(t('Thank you for your feedback...'));
}

function feedback_mail($key, &$message, $params) {
  $subject = t('Feedback recieved from: ');
  $subject .= $GLOBALS['user']->name;
  $message['subject'] = $subject;
  $message['body'][] = $params['feedback'];
}
