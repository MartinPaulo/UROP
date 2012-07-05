<?php
/**
 * @file
 * This contains a form that allows applicants to update and view any university degrees
 * that they may received after UROP.
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
function urop_university_form($user) {
  $result = drupal_get_form('urop_form_university', $user->uid);
  $result .= _urop_form_footer();
  return $result;
}

function urop_form_university($form_state, $user_uid) {
  $form['uid'] = _urop_user_value($user_uid);
  $form['university'] = array(
    '#type' => 'fieldset',
    '#title' => t('Degrees'),
    '#description' => t('Any degrees you have completed since you left UROP.'),
    );
  $form['university']['articles'] = array(
    '#type' => 'multicrud',
    '#multicrud_elements' => array(
       'degree_name' => array(
          '#title' => t('Degree name'),
          '#type' => 'textfield',
          '#size' => 50,
        ),
        'degree_abbreviation' => array(
          '#title' => t('Abbreviation'),
          '#type' => 'textfield',
          '#size' => 5,
        ),
       'degree_year' => array(
          '#title' => t('Year conferred'),
          '#type' => 'select',
          '#options' => _urop_year_list(),
  '#default_value' => '2012',
  // 	'#default_value' => $university ? $university->course_start_year : '2012',
        ),
       'degree_month' => array(
          '#title' => t('Month conferred'),
          '#type' => 'select',
          '#options' => _urop_month_list(),
          ),
       'pid' => array(
          '#type' => 'value',
        ),
      ),
     '#default_value' => urop_universitys_get_values($user_uid),
    );
  $form['submit'] = _urop_submit();
  return $form;
}

function urop_universitys_get_values($user_uid) {
   $values = array();
//   $sql = "SELECT * FROM {urop_universitys} WHERE uid = '%d' ORDER BY line_no;";
//   $result = db_query(db_rewrite_sql($sql), $user_uid);
//   while ($record = db_fetch_object($result)) {
//     $values[$record->line_no] = array(
//       'pid' => $record->pid,
//       'pub_name' => $record->pub_name,
//       'pub_issue' => $record->pub_issue,
//       'pub_title' => $record->pub_title,
//       'pub_authors' => $record->pub_authors,
//       );
//   }
   return $values;
}

function urop_university_details_form_submit($form, &$form_state) {
   $user_uid = $form_state['values']['uid'];
//   $submitHandler = new _university_on_submit($user_uid);
//   multicrud_submit($form['university']['articles'], $form_state['values']['articles'], $submitHandler);
//   //dpm($form_state['values']);
//   //die('<pre>' . $form_state['values'][''] .  '</pre>');
}

class _university_on_submit extends multicrud_SubmitHandler {

  private $table = 'urop_universitys';
  private $uid;

  public function __construct($user_uid) {
    $this->uid = $user_uid;
  }

  // called for every item deleted.
  public function deleteItem($line_no) {
    $sql = "DELETE FROM {$this->table} WHERE line_no = '%d' AND uid = '%d'";
    db_query($sql, $line_no, $this->uid);
    return TRUE; // item was deleted. // false if not...
  }

  // called on every submission after all the deleted items have been enumerated
  public function flushDelete() {
  }

  // will be called for every updated item. meta containst the weight (line no)
  public function updateItem($line_no, $item, $meta) {
    $record = new stdClass();
    $record->pid = $item['pid'];
    $record->uid = $this->uid;
    $record->pub_name = $item['pub_name'];
    $record->pub_issue = $item['pub_issue'];
    $record->pub_title = $item['pub_title'];
    $record->pub_authors = $item['pub_authors'];
    $record->line_no = $line_no;
    drupal_write_record($this->table, $record);
  }

  // called for every new item
  public function insertItem($item, $meta) {
    // $meta -> weight & -> parent
    // $item -> array of prize_name, etc..
    $record = new stdClass();
    $record->uid = $this->uid;
    $record->pub_name = $item['pub_name'];
    $record->pub_issue = $item['pub_issue'];
    $record->pub_title = $item['pub_title'];
    $record->pub_authors = $item['pub_authors'];
    $record->line_no = $meta['weight'];
    drupal_write_record($this->table, $record);
  }

  // called on every submission, as the last call made.
  public function flush() {
     drupal_set_message("Your university records have been updated.");
  }

}

