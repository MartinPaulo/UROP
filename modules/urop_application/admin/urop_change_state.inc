<?php
  const UROP_ACCEPT = 'accept';
  const UROP_REJECT = 'reject';
  const UROP_SKIP = 'skip';

  /**
   * @file
   * This file is a state engine to track the allowed changes in the UROP student field.
   *
   * An applicant is either a new applicant or a prior unsuccessful applicant.
   * They are then either rejected (moved to the prior unsuccessful state) or accepted for an interview.
   * After the interview they are then either rejected (moved to the prior unsuccessful state) or accepted for placement with a supervisor.
   * After the interview there is a catagory "reserve" which are students that are interviewed and suitable for placement but there is
   * not a perfect match and a match only gets found for them if a suitable project is sought out or another match between a
   * student and supervisor doesn't work out. If neither of these options happen then teh are rejected.
   * After matching with a supervisor, they are then either rejected (moved to the prior unsuccessful state), or placed with the supervisor (become active/current)
   * When they are scheduled to graduate, a follow up is done. If all has gone according to plan at this point they then become “urop graduates”
   * They are out as "urop graudates" either when they are scheduled to graduate or when the placement is up (minimum 6 months) whichever comes first.
   *
   * So student table has a "status" field to show the current status.
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

  function _urop_get_applicant_status_as_string($status) {
    switch ($status) {
      case UROP_APPLICANT_STATUS_UNSET:
      case UROP_APPLICANT_NEW:
        return "New Applicant";
      case UROP_APPLICANT_PRIOR:
        return "Prior Applicant";
      case UROP_APPLICANT_REJECTED:
        return "Rejected Applicant";
      case UROP_APPLICANT_SUBMITTER:
        return "Submitted Application";
      case UROP_APPLICANT_INTERVIEWEE:
        return "Selected For Interview";
      case UROP_APPLICANT_CURRENT:
        return "Current Student";
      case UROP_APPLICANT_GRADUATE:
        return "Program Graduate";
    }
    return "Applicant Status Unknown";
  }


  /**
   * new || prior -> applicant
   * applicant -> interview
   * rejected -> interview
   * interview -> current
   * current -> graduate
   * @param string $current_state what they currently are:
   */
  function _urop_get_next_accept_state($current_state) {
    if (UROP_APPLICANT_INTERVIEWEE == $current_state) {
      return UROP_APPLICANT_CURRENT;
    }
    elseif (UROP_APPLICANT_CURRENT == $current_state) {
      return UROP_APPLICANT_GRADUATE;
    }
    elseif (UROP_APPLICANT_SUBMITTER == $current_state) {
      return UROP_APPLICANT_INTERVIEWEE;
    }
    elseif (UROP_APPLICANT_REJECTED == $current_state) {
      return UROP_APPLICANT_INTERVIEWEE;
    }
    elseif (UROP_APPLICANT_PRIOR == $current_state
      || UROP_APPLICANT_NEW == $current_state
      || UROP_APPLICANT_STATUS_UNSET == $current_state
    ) {
      return UROP_APPLICANT_SUBMITTER;
    }
    return $current_state;
  }

  function urop_accept(&$student) {
    $new_status = _urop_get_next_accept_state($student->status);
    if ($new_status == $student->status) {
      drupal_set_message(check_plain("Why are we trying to promote a student of status $student->status ?"));
      return;
    }
    $student->status = $new_status;
    if (UROP_APPLICANT_CURRENT == $student->status) {
      urop_record_state_change($student);
      _urop_email_successful_applicant($student);
    }
    elseif (UROP_APPLICANT_GRADUATE == $student->status) {
      urop_record_state_change($student, '_urop_email_successful_graduate');
    }
    elseif (UROP_APPLICANT_INTERVIEWEE == $student->status) {
      urop_record_state_change($student, '_urop_email_successful_interviewee');
    }
    elseif (UROP_APPLICANT_INTERVIEWEE == $student->status) {
      urop_record_state_change($student);
    }
    elseif (UROP_APPLICANT_SUBMITTER == $student->status) {
      urop_record_state_change($student, '_urop_email_on_submission');
    }
    else {
      // should never be here
      drupal_set_message(check_plain("Could not save status change: $student->status ?"));
    }
  }

  /**
   * (new -> prior, interview -> prior, applicant -> prior, rejected-> prior are done in bulk on round opening).
   * Ordinarily:
   * interview -> rejected
   * applicant -> rejected
   * current -> interview
   * Once beyond the interview, no other rejections can take place?
   * surely some current can crash and burn -> prior.
   * But no graduates can reset to new/prior.
   * @param string $current_state What the student is at the moment...
   */
  function _urop_get_next_reject_state($current_state) {
    if (UROP_APPLICANT_CURRENT == $current_state) {
      return UROP_APPLICANT_INTERVIEWEE;
    }
    elseif (UROP_APPLICANT_INTERVIEWEE == $current_state) {
      return UROP_APPLICANT_REJECTED;
    }
    elseif (UROP_APPLICANT_SUBMITTER == $current_state) {
      return UROP_APPLICANT_REJECTED;
    }
    return ($current_state);
  }

  function urop_reject($student) {
    $new_status = _urop_get_next_reject_state($student->status);
    if ($new_status == $student->status) {
      drupal_set_message(check_plain("Why are we trying to reject a student of status $student->status ?"));
      return;
    }
    $student->status = $new_status;
    if (UROP_APPLICANT_INTERVIEWEE == $student->status) {
      urop_record_state_change($student, '_urop_email_rejected_interview');
    }
    elseif (UROP_APPLICANT_REJECTED == $student->status) {
      urop_record_state_change($student, '_urop_email_rejected_interview');
    }
    else {
      drupal_set_message(check_plain("Why are we trying to change a student with $student->status ?"));
    }
  }

  function urop_record_state_change($student, $email_function = "") {
    drupal_write_record(UROP_STUDENT_TABLE, $student, UROP_STUDENT_TABLE_KEY);
    if (strlen($email_function) > 0) {
      call_user_func($email_function, $student);
    }
  }

  function _urop_email_on_submission($student) {
    $mail_to = variable_get('urop_feedback_email', 'urop-bio21@unimelb.edu.au');
    drupal_mail('submission', 'submission', $mail_to, language_default(), $student);
  }

  function submission_mail($key, &$message, $student) {
    global $base_url;
    $subject = t('Submission received from: ');
    $subject .= $GLOBALS['user']->name;
    $message['subject'] = $subject;
    $message['body'][] = "Their submission can be found at:\r\n" .
      " * Personal info: $base_url/urop/$student->uid/personal\r\n" .
      " * University record: $base_url/urop/$student->uid/university\r\n" .
      " * Documents: $base_url/urop/$student->uid/documents\r\n" .
      " * All in one: $base_url/urop/$student->uid/review\r\n" .
      " * All in one .doc: $base_url/urop/$student->uid/review/doc\r\n";
  }

  function _urop_email_successful_applicant($student) {
    drupal_set_message(check_plain("The candidate's status now is: $student->status"));
  }

  function _urop_email_successful_graduate($student) {
    drupal_set_message(check_plain("The candidate's status now is: $student->status"));
  }

  function _urop_email_successful_interviewee($student) {
    drupal_set_message(check_plain("The candidate's status now is: $student->status"));
  }

  function _urop_email_rejected_interview($student) {
    drupal_set_message(check_plain("The candidate's status now is: $student->status"));
  }

  function urop_form_accept_reject($user) {
    $student = urop_get_student_status($user->uid);
    $result = drupal_get_form('urop_accept_reject_form', $student, $user->uid);
    $result .= drupal_get_form('urop_add_supervisors_form', $user->uid);
    $result .= _urop_form_footer();
    return $result;
  }

  function urop_accept_reject_form($form_state, $student, $user_uid) {
    $student_status = _urop_get_applicant_status_as_string($student->status);
    $form[UROP_STUDENT_TABLE_KEY] = _urop_user_value($user_uid);
    $form['accept_reject'] = array(
      '#type' => 'fieldset',
      '#title' => t('Accept/Reject'),
      '#collapsible' => FALSE,
    );
    $form['accept_reject']['name'] = array(
      '#value' => "Applicant's firstname/surname: <b>" . check_plain($student->given_name . " " . $student->family_name) . "</b>",
    );
    $form['accept_reject']['notes'] = array(
      '#type' => 'textarea',
      '#title' => t('Notes'),
      '#description' => "Any notes that you might want to make about this action.",
      '#required' => FALSE,
    );
    $form['accept_reject']['status'] = array(
      '#value' => "Applicant's current status: <b>" . check_plain($student_status) . "</b>",
    );
    $form['accept_reject']['action'] = array(
      '#type' => 'radios',
      '#title' => t('Action'),
      '#disabled' => _urop_is_status_disabled($student),
      '#default_value' => UROP_SKIP,
      '#options' => array(
        UROP_ACCEPT => t('Accept') . ' (promote to: ' . _urop_get_applicant_status_as_string(_urop_get_next_accept_state($student->status)) . ')',
        UROP_REJECT => t('Reject') . ' (demote to: ' . _urop_get_applicant_status_as_string(_urop_get_next_reject_state($student->status)) . ')',
        UROP_SKIP => t('Just record note')
      ),
      '#description' => "Whether to accept, reject or do nothing other than record the note.",
      '#required' => TRUE,
    );
    $form['accept_reject']['submit'] = _urop_submit();
    if (urop_application_is_personal_information_needed($user_uid)) {
      $form['accept_reject']['submit']['#disabled'] = TRUE;
      $form['accept_reject']['logic'] = array(
        '#value' => "<p><i>(Disabled because the student has no personal details added)</i></p>",
      );
    }
    // for reset have a look at:
    // http://devengineering.com/best-practices/drupal/cancellation-required-validation-drupal-6
    return $form;
  }

  function _urop_is_status_disabled($student) {
    return $student->status == UROP_APPLICANT_GRADUATE
      || $student->status == UROP_APPLICANT_NEW
      || $student->status == UROP_APPLICANT_PRIOR
      || $student->status == UROP_APPLICANT_STATUS_UNSET;
  }

  function urop_accept_reject_form_submit($form, &$form_state) {
    $user_uid = $form_state['values'][UROP_STUDENT_TABLE_KEY];
    $action = $form_state['values']['action'];
    $student = urop_get_student_status($user_uid);
    $old_status = $student->status;
    switch ($action) {
      case UROP_ACCEPT:
        urop_accept($student);
        break;
      case UROP_REJECT:
        urop_reject($student);
        break;
      case UROP_SKIP:
      default:
        ;
    }
    _urop_save_note($user_uid, $form_state['values']['notes'], $action, $old_status, $student->status);
    drupal_set_message(check_plain("The note/action ($action) has been saved."));
  }

  function _urop_save_note($user_uid, $note, $action, $old_status, $new_status) {
    if (strlen($note) > 0 || $action != UROP_SKIP) {
      $state_change = new stdClass();
      $state_change->uid = $user_uid;
      $state_change->note = $note;
      $state_change->action = $action;
      $state_change->status = $old_status;
      $state_change->new_status = $new_status;
      $state_change->date = format_date(time(), 'custom', 'Y.m.d H:i:s');
      drupal_write_record('urop_state_changes', $state_change);
    }
  }

  // http://drupal.org/node/325766
  function _urop_supervisors_list() {
    $items = array();
    $sql = "SELECT * FROM {urop_supervisors}";
    $result = db_query(db_rewrite_sql($sql));
    while ($data = db_fetch_object($result)) {
      $items[$data->uid] = $data->given_name . " " . $data->family_name . ", " . $data->institution;
    }
    return $items;
  }

  function _urop_students_supervisors_list($user_uid) {
    $items = array();
    $sql = "SELECT * FROM {urop_supervisors_students_join} WHERE student_uid = '%d'";
    $result = db_query(db_rewrite_sql($sql), $user_uid);
    while ($supervisors = db_fetch_object($result)) {
      $items[] = $supervisors->supervisor_uid;
    }
    return $items;
  }

  function urop_add_supervisors_form($form_state, $user_uid) {
    $form['uid'] = _urop_user_value($user_uid);
    $form['supervisors'] = array(
      '#type' => 'fieldset',
      '#title' => t('Match supervisors'),
      '#collapsible' => FALSE,
    );
    $form['supervisors']['list'] = array(
      '#type' => 'select',
      '#title' => t('Supervisors'),
      '#required' => TRUE,
      '#description' => t('The supervisors selected for the student.'),
      '#multiple' => TRUE,
      '#size' => 12,
      '#options' => _urop_supervisors_list(),
      '#default_value' => _urop_students_supervisors_list($user_uid),
    );
    $form['supervisors']['submit'] = _urop_submit();
    return $form;
  }

  function urop_add_supervisors_form_submit($form, &$form_state) {
    // brute force: delete all existing records...
    $user_uid = $form_state['values']['uid'];
    $sql = "DELETE FROM {urop_supervisors_students_join} WHERE student_uid = '%d'";
    db_query($sql, $user_uid);
    // then, add chosen as new...
    $sql = "INSERT INTO {urop_supervisors_students_join} (supervisor_uid, student_uid) VALUES (%d, %d)";
    foreach ($form_state['values']['list'] as $supervisor_id) {
      db_query($sql, $supervisor_id, $user_uid);
    }

  }
