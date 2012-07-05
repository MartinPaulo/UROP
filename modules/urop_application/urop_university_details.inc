<?php
  /**
   * @file
   * This contains a form that enables applicants to manage their university details.
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

  // in all forms the user is set as a parameter rather than taken to be the current user,
  // so that the admin can edit them... Hence the "uid" value on the form itself.
  function urop_university_form($user) {
    $sql = "SELECT * from {urop_universities} WHERE uid = '%d'";
    $university = db_fetch_object(db_query(db_rewrite_sql($sql), $user->uid));
    $result = drupal_get_form('urop_application_university_details_form', $university, $user->uid);
    $result .= _urop_form_footer();
    return $result;
  }

  function urop_application_university_details_form($form_state, $university, $user_uid) {
    $form['uid'] = _urop_user_value($user_uid);
    $form['university'] = array(
      '#type' => 'fieldset',
      '#title' => t('The Victorian tertiary institution in which you are enrolled.'),
    );
    $form['university']['university'] = array(
      '#type' => 'select',
      '#title' => t('University'),
      '#required' => TRUE,
      '#description' => t('The name of the university that you are currently studying at.'),
      '#default_value' => $university ? $university->university : '',
      '#options' => _urop_universities(),
    );
    $form['university']['course'] = array(
      '#type' => 'select_or_other',
      '#select_type' => 'select',
      '#title' => t('Degree'),
      '#description' => t('The degree you are studying at this institution.'),
      '#options' => _urop_degree_listing(),
      '#default_value' => $university ? (intval($university->course) > 400 ? $university->course_other : $university->course) : '',
      '#other' => t('Other (please specify)'),
      '#required' => TRUE,
      '#other_unknown_defaults' => 'other'
    );
    $form['university']['course_start_year'] = array(
      '#type' => 'select',
      '#title' => "Start year",
      '#description' => 'The year in which you started this course.',
      '#options' => _urop_year_list(),
      '#required' => TRUE,
      '#default_value' => $university ? $university->course_start_year : '2012',
    );
    $form['university']['course_end_year'] = array(
      '#type' => 'select',
      '#title' => "Expected completion year",
      '#description' => 'The year in which you expect to complete this course, including any additional diploma (provide details below).',
      '#options' => _urop_year_list(),
      '#required' => TRUE,
      '#default_value' => $university ? $university->course_end_year : '2012',
    );
    $form['university']['results'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your results:'),
      '#description' => t('These must match the results record that you provide.'),
    );
    $form['university']['results']['course_results'] = array(
      '#type' => 'multicrud',
      '#multicrud_elements' => urop_application_universities_courses_for_multicrud(),
      '#default_value' => urop_university_get_results('U', $user_uid),
    );

    $form['university']['diploma'] = array(
      '#type' => 'fieldset',
      '#title' => t('Diploma'),
      '#description' => t('To be completed if you are also studying for a diploma.'),
      '#collapsible' => TRUE,
      '#collapsed' => $university ? strlen($university->dip_name) <= 0 : TRUE,
    );
    $form['university']['diploma']['dip_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Diploma name'),
      '#required' => FALSE,
      '#default_value' => $university ? $university->dip_name : '',
      '#description' => t('Please provide the name of the dimploma you are studying for.'),
    );
    $form['university']['diploma']['dip_start_year'] = array(
      '#type' => 'select',
      '#title' => "Start year",
      '#description' => 'The year in which you started this diploma.',
      '#options' => _urop_year_list(),
      '#required' => TRUE,
      '#default_value' => $university ? $university->dip_start_year : '2012',
    );
    $form['university']['diploma']['dip_end_year'] = array(
      '#type' => 'select',
      '#title' => "Expected completion year",
      '#description' => 'The year in which you expect to complete this diploma. This must match the expected completion year above.',
      '#options' => _urop_year_list(),
      '#required' => TRUE,
      '#default_value' => $university ? $university->dip_end_year : '2012',
    );
    $form['university']['diploma']['results'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your results:'),
      '#description' => t('The results credited towards the diploma for which you are studying.') . '<br />' .
        t('Note that you do not need to relist subjects that were originally credited as part of your main degree and are listed in the section above.'),
    );
    $form['university']['diploma']['results']['diploma_results'] = array(
      '#type' => 'multicrud',
      '#multicrud_elements' => urop_application_universities_courses_for_multicrud(),
      '#default_value' => urop_university_get_results('D', $user_uid),
    );
    $form['previous'] = array(
      '#type' => 'fieldset',
      '#title' => t('Prior degree (if applicable)'),
      '#description' => t('The university that you attended prior to the institution you are applying from.'),
      '#collapsible' => TRUE,
      '#collapsed' => $university ? strlen($university->previous_name) <= 0 : TRUE,
    );
    $form['previous']['previous_name'] = array(
      '#type' => 'textfield',
      '#title' => t('University'),
      '#required' => FALSE,
      '#description' => t('The name of the university that you studied at.'),
      '#default_value' => $university ? $university->previous_name : '',
    );
    $form['previous']['international_degree'] = array(
      '#type' => 'textfield',
      '#title' => t('Degree'),
      '#required' => FALSE,
      '#description' => t('The degree you studied at this institution.'),
      '#default_value' => $university ? $university->international_course : '',
    );
    $form['previous']['results'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your results:'),
      '#description' => t('These must match the results record that you provide.'),
    );
    $form['previous']['results']['previous_results'] = array(
      '#type' => 'multicrud',
      '#multicrud_elements' => urop_application_universities_courses_for_multicrud(),
      '#default_value' => urop_university_get_results('I', $user_uid),
    );
    $form['submit'] = _urop_submit();
    if (urop_application_not_editable($user_uid)) {
      $form['submit']['#disabled'] = TRUE;
      $form['logic'] = array(
        '#value' => DISABLED_MESSAGE,
      );
    }

    // for reset have a look at:
    // http://devengineering.com/best-practices/drupal/cancellation-required-validation-drupal-6
    return $form;
  }

  function _mark_validate($element, &$form_state) {
    $value = intval($element['#value']);
    if (!is_numeric($element['#value']) || !(0 <= $value && $value <= 100)) {
      form_error($element, t('"@value" is not a percentage. Please enter a valid percentage (in the range 0-100).', array("@value" => $element['#value'])));
    }
  }

  function _unit_validate($element, &$form_state) {
    if (strlen($element['#value']) > 9) {
      form_error($element, t('"@value" is too long. Maximum length is 9 characters.', array("@value" => $element['#value'])));
    }
  }

  function _unit_name_validate($element, &$form_state) {
    if (strlen($element['#value']) > 45) {
      form_error($element, t('"@value" is too long. Maximum length is 45 characters.', array("@value" => $element['#value'])));
    }
  }

  function urop_university_get_results($type, $user_uid) {
    $values = array();
    $sql = "SELECT * FROM {urop_results} WHERE uid = '%d' AND type='%s' ORDER BY line_no DESC;";
    $result = db_query(db_rewrite_sql($sql), $user_uid, $type);
    while ($record = db_fetch_object($result)) {
      $values[$record->line_no] = array(
        'rid' => $record->rid,
        'unit_no' => $record->unit_no,
        'unit_name' => $record->unit_name,
        'unit_mark' => $record->unit_mark,
      );
    }
    return $values;
  }

  // http://drupal.org/node/325766
  function _urop_degree_listing() {
    static $degree_list;
    if (!isset($degree_list)) {
      $degree_list = array();
      $sql = "SELECT * FROM {university_courses} ORDER BY cid;";
      $result = db_query(db_rewrite_sql($sql));
      while ($data = db_fetch_object($result)) {
        $degree_list[$data->cid] = $data->description;
      }
    }
    return $degree_list;
  }

  function _urop_get_degree_string($cid) {
    $degree_list = _urop_degree_listing();
    return $degree_list[$cid];
  }

  function urop_application_universities_courses_for_multicrud() {
    $result = array(
      'unit_no' => array(
        '#title' => 'Subject no.',
        '#type' => 'textfield',
        '#size' => 9,
        '#element_validate' => array('_unit_validate'),
      ),
      'unit_name' => array(
        '#title' => 'Subject name',
        '#type' => 'textfield',
        '#size' => 45,
        '#element_validate' => array('_unit_name_validate'),
      ),
      'unit_mark' => array(
        '#title' => 'Mark (%)',
        '#type' => 'textfield',
        '#size' => 3,
        '#element_validate' => array('_mark_validate'),
      ),
      'rid' => array(
        '#type' => 'value',
      ),
    );
    return $result;
  }

  function urop_application_university_details_form_validate($form, &$form_state) {
    $degree_start_year = intval($form_state['values']['course_start_year']);
    $degree_end_year = intval($form_state['values']['course_end_year']);
    if ($degree_start_year > $degree_end_year) {
      form_set_error('course_start_year', 'Your degree start year is after its ending year?');
    }
    if (strlen($form_state['values']['dip_name']) > 0) {
      $diploma_start_year = intval($form_state['values']['dip_start_year']);
      $diploma_end_year = intval($form_state['values']['dip_end_year']);
      if ($diploma_start_year > $diploma_end_year) {
        form_set_error('dip_start_year', 'Your diploma start year is after its ending year?');
      }
      if ($diploma_end_year != $degree_end_year) {
        form_set_error('dip_end_year', 'Your diploma end year must be the same as your degree end year!');
      }
    }
  }

  function urop_application_university_details_form_submit($form, &$form_state) {
    // dpm($form_state['values']); //devel module print
    // if this is the admin user, then we need to use the correct user id:
    // not the global user, but rather the one that was passed in via the url!
    $user_uid = $form_state['values']['uid'];
    _urop_save_results($user_uid, $form, $form_state);
    // university info depends on results...
    _urop_save_university_info($user_uid, $form_state);
    // only redirect when the submit & on button has been pressed...
    if ($form_state['clicked_button']['#value'] == 'Submit') {
      if (urop_application_is_ready($user_uid)) {
        $form_state['redirect'] = 'urop/' . $user_uid . '/submit';
      }
      else {
        $form_state['redirect'] = 'urop/' . $user_uid . '/documents';
      }
    }
    drupal_set_message(t('Your university details have been updated'));
  }

  function _urop_calculate_average($user_uid, $type) {
    //$result = db_result(db_query("SELECT COUNT(*) FROM {urop_results} WHERE uid = '%d' AND type = 'D';", $user_uid));
    //$result = db_result(db_query("SELECT SUM(unit_mark) FROM {urop_results} WHERE uid = '%d' AND type = 'U';", $user_uid));
    return db_result(db_query("SELECT SUM(unit_mark)/COUNT(*) FROM {urop_results} WHERE uid = '%d' AND type = '%s';", $user_uid, $type));
  }

  function _urop_save_university_info($user_uid, $form_state) {
    $table = 'urop_universities';
    $university = new stdClass();
    $key = array();
    $sql = "SELECT COUNT(*) from {$table} WHERE uid = '%d'";
    $count = db_result(db_query($sql, $user_uid));
    if ($count > 0) {
      $key[] = 'uid';
    }
    $university->uid = $user_uid;
    $university->university = $form_state['values']['university'];
    // will either be numeric or 'other'
    if (is_numeric($form_state['values']['course'])) {
      // because of http://drupal.org/node/227677 we can't set the values to null :(
      $university->course = $form_state['values']['course'];
      $university->course_other = _urop_get_degree_string($university->course);
    }
    else {
      $university->course = 1000;
      $university->course_other = $form_state['values']['course'];
    }
    $university->course_start_year = $form_state['values']['course_start_year'];
    $university->course_end_year = $form_state['values']['course_end_year'];
    $university->dip_name = $form_state['values']['dip_name'];
    $university->dip_start_year = $form_state['values']['dip_start_year'];
    $university->dip_end_year = $form_state['values']['dip_end_year'];
    $university->previous_name = $form_state['values']['previous_name'];
    $university->international_course = $form_state['values']['international_degree'];
    $university->course_average = _urop_calculate_average($user_uid, 'U');
    $university->dip_average = _urop_calculate_average($user_uid, 'D');
    drupal_write_record($table, $university, $key);
  }

  function _urop_save_results($user_uid, $form, $form_state) {
    $submit_handler = new __uni_results_on_submit();
    $submit_handler->uid = $user_uid;
    $submit_handler->type = 'U';
    multicrud_submit($form['university']['results']['course_results'], $form_state['values']['course_results'], $submit_handler);
    $submit_handler->type = 'D';
    // should call only if the diploma field is set ?
    multicrud_submit($form['university']['diploma']['results']['diploma_results'], $form_state['values']['diploma_results'], $submit_handler);
    $submit_handler->type = 'I';
    // should call only if the international field is set ?
    multicrud_submit($form['previous']['results']['previous_results'], $form_state['values']['previous_results'], $submit_handler);
  }

  class __uni_results_on_submit extends multicrud_SubmitHandler {

    public $type;
    public $uid;

    // called for every item deleted.
    public function deleteItem($id) {
      /* would have been nice if we got a row id... */
      $sql = "DELETE FROM {'urop_results'} WHERE line_no = '%d' AND uid = '%d' AND type = '%s';";
      db_query($sql, $id, $this->uid, $this->type);
      return TRUE; // item was deleted. // false if not...
    }

    // called on every submission after all the deleted items have been enumerated
    public function flushDelete() {
    }

    // will be called for every updated item. meta containst the weight
    public function updateItem($id, $item, $meta) {
      $table = 'urop_results';
      $results = new stdClass();
      $results->rid = $item['rid']; //row id
      $results->uid = $this->uid; // user id
      $results->type = $this->type; // type (degree/diploma/prior)
      $results->unit_no = $item['unit_no'];
      $results->unit_name = $item['unit_name'];
      $results->unit_mark = $item['unit_mark'];
      $results->line_no = $id; // $meta['weight']?  the line number in the grid.
      drupal_write_record($table, $results, 'rid');
    }

    // called for every new item
    public function insertItem($item, $meta) {
      // $meta -> weight & -> parent
      // $item -> array of prize_name, etc..
      $table = 'urop_results';
      $results = new stdClass();
      $results->uid = $this->uid;
      $results->type = $this->type;
      $results->unit_no = $item['unit_no'];
      $results->unit_name = $item['unit_name'];
      $results->unit_mark = $item['unit_mark'];
      $results->line_no = $meta['weight'];
      drupal_write_record($table, $results);
    }

    // called on every submission, as the last call made.
    public function flush() {
    }

  }

