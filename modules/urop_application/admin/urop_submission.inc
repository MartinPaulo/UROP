<?php

  /**
   * @file
   * This contains a form that allows applicants to submit their applications.
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

  require_once('urop_change_state.inc');

  function urop_form_submission($user) {
    if (urop_is_round_closed()) {
      $result = "<p>The round is now closed. Submission's are thus not being accepted.</p>";
    }
    else {
      $result = drupal_get_form('urop_submission_form', $user->uid);
    }
    $result .= _urop_form_footer();
    return $result;
  }

  function urop_submission_form($form_state, $user_uid) {
    $form[UROP_STUDENT_TABLE_KEY] = _urop_user_value($user_uid);
    $form['submission'] = array(
      '#type' => 'fieldset',
      '#title' => t('Submit application'),
      '#collapsible' => FALSE,
  );
    $form['submission']['message'] = array(
      '#value' => "<p>You have provided all of the required information for your application.</p>" .
        "<p>When you select the <b>submit</b> button below it will be sent to UROP for processing.</p>" .
        "<p>Once submitted you will <b>not</b> be able to make changes to your University details or your" .
        " supporting documentation.</p>",
    );

    $form['submission']['submit'] = _urop_submit();
    return $form;
  }

  function urop_submission_form_submit($form, &$form_state) {
    $user_uid = $form_state['values'][UROP_STUDENT_TABLE_KEY];
    $student = urop_get_student_status($user_uid);
    $old_status = $student->status;
    urop_accept($student);
    _urop_save_note($user_uid, "Submission made", UROP_SKIP, $old_status, $student->status);
    drupal_set_message(t('UROP have been notified of your submission.'));
    $form_state['redirect'] = UROP_HOME_URL;
  }
