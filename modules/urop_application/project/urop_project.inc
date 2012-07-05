<?php
/**
 * @file
 * Contains a form that allows the user to describe their urop project.
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

/**
 * @param User $user 	The drupal user loaded from the url (not necessarily the logged in user)
 * @return string The form to describe the urop students project description.
 */
function urop_form_project($user) {
  $result = drupal_get_form('urop_project_form', $user->uid);
  $result .= _urop_form_footer();
  return $result;
}

function urop_project_form($form_state, $user_uid) {
  $form['uid'] = _urop_user_value($user_uid);
  $form['project'] = _urop_fieldset('');
  $form['project']['supervisors'] = _urop_fieldset('Supervisors');
  $form['project']['supervisors']['name'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
      'supervisors' => array(
        '#title' => t('Name:'),
        '#type' => 'textfield',
        '#size' => 50,
        '#required' => TRUE,
      ),
      'pid' => array(
        '#type' => 'value',
      ),
    ),
    //'#default_value' => urop_publications_get_values($user_uid),
  );

  $form['project']['lab'] = _urop_fieldset('Research Laboratory');
  $form['project']['lab']['name'] = _urop_edit_box('Name');
  $form['project']['lab']['name']['#required'] = TRUE;
  $form['project']['lab']['institution'] = _urop_edit_box('Institution');
  $form['project']['lab']['institution']['#required'] = TRUE;

  $form['project']['details'] = _urop_fieldset('Project');
  $form['project']['details']['desc'] =_urop_text_area('Description');
  $form['project']['details']['desc'] ['#required'] = TRUE;
  $form['project']['details']['techniques'] =_urop_text_area('Techniques');
  $form['project']['details']['techniques'] ['#required'] = TRUE;
  $form['submit'] = _urop_submit();
  return $form;
}

function urop_project_form_validate($form, &$form_state) {
  // if this is the admin user, then we need to use the correct user id:
  // not the global user, but rather the one that was passed in via the url!
  $user_uid = $form_state['values']['uid'];
}

function urop_project_form_submit($form, &$form_state) {
  // if this is the admin user, then we need to use the correct user id:
  // not the global user, but rather the one that was passed in via the url!
  $user_uid = $form_state['values']['uid'];
}
