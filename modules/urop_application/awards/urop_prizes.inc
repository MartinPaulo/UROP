<?php
/**
 * @file
 * Contains a form that allows the user to describe their prizes received after the UROP program.
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
function urop_prize_details_form($form_state, $user_uid) {
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user_uid,
  );
  $form['prize'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prizes'),
    '#description' => t('Any prizes that you might have won since you left UROP.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['prize']['prizes'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
      'prize_name' => array(
        '#title' => 'Name',
        '#type' => 'textfield',
        '#size' => 30,
        '#required' => TRUE,
  ),
      'prize_uni' => array(
        '#title' => 'Awarding body',
        '#type' => 'textfield',
        '#size' => 30,
        '#required' => TRUE,
  ),
      'prize_date' => array(
        '#type' => 'select',
        '#title' => 'Year recieved',
        '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
  ),
      'prize_value' => array(
        '#title' => 'Value (if applicable)',
        '#type' => 'textfield',
        '#size' => 8,
        '#element_validate' => array('urop_prize_value_validate'),
  ),
       'pid' => array(
          '#type' => 'value',
  ),
  ),
    '#default_value' => urop_prize_get_values($user_uid),
  );
  $form['prize']['submit'] = _urop_submit();
  return $form;
}
function urop_prize_value_validate($element, &$form_state) {
  if (!preg_match('/^\d+/', $element['#value'])) {
    form_error($element, t('Please define a valid amount.'));
  }
}

function urop_prize_get_values($user_uid) {
  $values = array();
  $sql = "SELECT * FROM {urop_prizes} WHERE uid = '%d' ORDER BY line_no;";
  $result = db_query(db_rewrite_sql($sql), $user_uid);
  while ($record = db_fetch_object($result)) {
    $values[$record->line_no] = array(
      'pid' => $record->pid,
      'prize_name' => $record->prize_name,
      'prize_uni' => $record->prize_uni,
      'prize_date' => $record->prize_date,
      'prize_years' => $record->prize_years,
      'prize_value' => $record->prize_value,
    );
  }
  return $values;
}


function urop_prize_details_form_submit($form, &$form_state) {
  $user_uid = $form_state['values']['uid'];
  $submit_handler = new _prize_on_submit();
  $submit_handler->uid = $user_uid;
  multicrud_submit($form['prize']['prizes'], $form_state['values']['prizes'], $submit_handler);
  //dpm($form_state['values']);
  //die('<pre>' . $form_state['values'][''] .  '</pre>');
}

class _prize_on_submit extends multicrud_SubmitHandler {

  public $uid;

  // called for every item deleted.
  public function deleteItem($id) {
    $sql = "DELETE FROM {urop_prizes} WHERE line_no = '%d' AND uid = '%d'";
    db_query($sql, $id, $this->uid);
    return TRUE; // item was deleted. // false if not...
  }

  // called on every submission after all the deleted items have been enumerated
  public function flushDelete() {
  }

  // will be called for every updated item. meta containst the weight
  public function updateItem($id, $item, $meta) {
    $table = 'urop_prizes';
    $record = new stdClass();
    $record->pid = $item['pid'];
    $record->uid = $this->uid;
    $record->prize_name = $item['prize_name'];
    $record->prize_uni = $item['prize_uni'];
    $record->prize_date = $item['prize_date'];
    $record->prize_years = $item['prize_years'];
    $record->prize_value = $item['prize_value'];
    $record->line_no = $id; // $meta['weight']?
    drupal_write_record($table, $record);
  }

  // called for every new item
  public function insertItem($item, $meta) {
    // $meta -> weight & -> parent
    // $item -> array of prize_name, etc..
    $table = 'urop_prizes';
    $record = new stdClass();
    $record->uid = $this->uid;
    $record->prize_name = $item['prize_name'];
    $record->prize_uni = $item['prize_uni'];
    $record->prize_date = $item['prize_date'];
    $record->prize_years = $item['prize_years'];
    $record->prize_value = $item['prize_value'];
    $record->line_no = $meta['weight'];
    drupal_write_record($table, $record);
  }

  // called on every submission, as the last call made.
  public function flush() {
    drupal_set_message("Your prize records have been updated.");
  }
}

