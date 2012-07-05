<?php

  /**
   * @file
   * This file contains the admin interface.
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

  function urop_admin() {
    $result = '<p>' . t('Welcome administrator!');
    $result .= drupal_get_form('urop_round_manager');
    $result .= drupal_get_form('urop_who_to_contact');
    $result .= drupal_get_form('urop_node_to_show');
    $result .= drupal_get_form('urop_disk_free_warning');
    return $result;
  }

  function urop_who_to_contact() {
    $form['email'] = array(
      '#type' => 'fieldset',
      '#title' => t('E-mail addresses'),
      '#description' => t('E-mail addresses that will be used to reach UROP.'),
      '#collapsible' => TRUE,
    );
    $form['email']['contact_mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact e-mail address'),
      '#description' => "The e-mail address on each page for users that want to request help. eg: help@urop.edu.au",
      '#required' => TRUE,
      '#default_value' => variable_get('urop_contact_email', 'urop-bio21@unimelb.edu.au'),
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['email']['feedback_mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Feedback e-mail address'),
      '#description' => "The e-mail address that user feedback will be sent to. eg: feedback@urop.edu.au",
      '#required' => TRUE,
      '#default_value' => variable_get('urop_feedback_email', 'urop-bio21@unimelb.edu.au'),
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['email']['submit'] = _urop_submit();
    return $form;
  }

  function urop_who_to_contact_validate($form, &$form_state) {
    $reason = '';
    if (!valid_email($form_state['values']['feedback_mail'], $reason)) {
      $error_msg = t('The feedback email address is not valid. ');
      $error_msg .= $reason;
      form_set_error('feedback_mail', check_plain($error_msg));
    }
    if (!valid_email($form_state['values']['contact_mail'], $reason)) {
      $error_msg = t('The contact email address is not valid. ');
      $error_msg .= $reason;
      form_set_error('contact_mail', check_plain($error_msg));
    }
  }

  function urop_who_to_contact_submit($form, &$form_state) {
    $feedback_email = $form_state['values']['feedback_mail'];
    variable_set('urop_feedback_email', $feedback_email);
    drupal_set_message(t('The feedback e-mail has been changed to: @email', array('@email' => $feedback_email)));
    $contact_email = $form_state['values']['contact_mail'];
    variable_set('urop_contact_email', $contact_email);
    drupal_set_message(t('The contact e-mail has been changed to: @email', array('@email' => $contact_email)));
  }

  function urop_node_to_show() {
    $form['node'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pages'),
      '#description' => t('The pages that will be shown to users...'),
      '#collapsible' => TRUE,
    );
    $form['node']['new_applicant'] = array(
      '#type' => 'textfield',
      '#title' => t('New applicant'),
      '#description' => "The node id that is to be used to show new applicants instructions.",
      '#required' => TRUE,
      '#default_value' => variable_get('urop_new_applicant', '1'),
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['node']['past_member'] = array(
      '#type' => 'textfield',
      '#title' => t('Past member'),
      '#description' => "The node id that is be shown to past members when they select Education/Carreer details",
      '#required' => TRUE,
      '#default_value' => variable_get('urop_past_member', '2'),
      '#size' => 50,
      '#maxlength' => 50,
    );
    $form['node']['submit'] = _urop_submit();
    return $form;
  }

  function urop_node_to_show_validate($form, &$form_state) {
    _urop_validate_node($form_state['values']['new_applicant'], 'new_applicant');
    _urop_validate_node($form_state['values']['past_member'], 'past_member');
  }

  function urop_node_to_show_submit($form, &$form_state) {
    $new_applicant = $form_state['values']['new_applicant'];
    variable_set('urop_new_applicant', $new_applicant);
    drupal_set_message(t('The node to show new applicants has been changed to: @id', array('@id' => $new_applicant)));
    $past_member = $form_state['values']['past_member'];
    variable_set('urop_past_member', $past_member);
    drupal_set_message(t('The node to show past members has been changed to: @id', array('@id' => $past_member)));
  }

  function _urop_validate_node($node_id, $field_name) {
    if (ctype_digit($node_id) == FALSE) {
      $error_msg = t('The ') . $field_name . t(' field needs to be an integer.');
      form_set_error($field_name, check_plain($error_msg));
    }
    else {
      if (node_load($node_id) == FALSE) {
        $error_msg = t('The ') . $field_name . t(' does not point to an existing node!');
        form_set_error($field_name, check_plain($error_msg));
      }
    }
  }

  function urop_round_manager() {
    $form['round'] = array(
      '#type' => 'fieldset',
      '#title' => t('Round Manager'),
      '#collapsible' => TRUE,
    );

    $current_round_tag = _urop_get_current_round_tag();

    $round_open = variable_get(UROP_ROUND_OPEN, 0);
    if ($round_open > 0) {
      $form['round']['contact_information'] = array(
        '#value' => "<p>The round is currently open.</p>" .
          "<ul>" .
          "<li>It is currently: <b>" . $current_round_tag . "</b></li>" .
          "</ul>",
      );
      $form['round']['submit'] = _urop_submit();
      $form['round']['submit']['#value'] = 'Close Round!';
    }
    else {
      list($next_round, $next_year) = _urop_calculate_next_round();
      $next_round_tag = $next_round . "-" . $next_year;
      $form['round']['contact_information'] = array(
        '#value' => "<p>The round is currently closed.</p>" .
          "<ul>" .
          "<li>The last round was: <b>" . $current_round_tag . "</b></li>" .
          "<li>The next round will be: <b>" . $next_round_tag . "</b> </li>" .
          "</ul>",
      );
      $form['round']['submit'] = _urop_submit();
      $form['round']['submit']['#value'] = 'Open Round!';
    }
    return $form;
  }

  function _urop_calculate_next_round() {
    list($current_year, $current_round) = _urop_get_current_round_info();
    $next_round = $current_round + 1;
    $round_year = $current_year;
    if ($next_round > 2) {
      $next_round = 1;
      $round_year = $current_year + 1;
      return array($next_round, $round_year);
    }
    return array($next_round, $round_year);
  }

  function urop_round_manager_submit($form, &$form_state) {
    if (urop_is_round_closed()) {
      variable_set(UROP_ROUND_OPEN, 1);
      list($round_no, $round_year) = _urop_calculate_next_round();
      variable_set(UROP_ROUND_YEAR, $round_year);
      variable_set(UROP_ROUND_NUMBER, $round_no);
      drupal_set_message(t('Round @round_no-@round_year is open!', array('@round_no' => $round_no, '@round_year' => $round_year)));
    }
    else {
      variable_set(UROP_ROUND_OPEN, 0);
      drupal_set_message(t('Round is closed!'));
    }
  }


  /**
   * From, based on: http://www.linuxjournal.com/article/9585?page=0,3
   * Validates an email address.
   * Provide email address (raw input)
   * Returns TRUE if the email address has the email address format and the domain exists, FALSE otherwise.
   * The reason for the failure is written into the Reason argument
   */
  function valid_email($email, &$reason) {
    $is_valid = TRUE;
    $at_index = strrpos($email, "@");
    if (is_bool($at_index) && !$at_index) {
      $reason = t('An e-mail addresses must contain an "@" symbol.');
      $is_valid = FALSE;
    }
    else {
      $domain = substr($email, $at_index + 1);
      $local = substr($email, 0, $at_index);
      $local_len = strlen($local);
      $domain_len = strlen($domain);
      if ($local_len < 1 || $local_len > 64) {
        $reason = t('The bit before the "@" must be between 1 and 64 characters in length.');
        $is_valid = FALSE;
      }
      elseif ($domain_len < 1 || $domain_len > 255) {
        $reason = t('The part after the "@" must be between 1 and 155 characters in length.');
        $is_valid = FALSE;
      }
      elseif ($local[0] == '.' || $local[$local_len - 1] == '.') {
        $reason = t('The bit before the "@" cannot start with a "."');
        $is_valid = FALSE;
      }
      elseif (preg_match('/\\.\\./', $local)) {
        $reason = t('The bit before the "@" has too many consecutive "." characters.');
        $is_valid = FALSE;
      }
      elseif (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain)) {
        $reason = t('There is an invalid character in the domain.');
        $is_valid = FALSE;
      }
      elseif (preg_match('/\\.\\./', $domain)) {
        $reason = t('The domain has too many consecutive "." characters.');
        $is_valid = FALSE;
      }
      elseif (!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\", "", $local))) {
        if (!preg_match('/^"(\\\\"|[^"])+"$/', str_replace("\\\\", "", $local))) {
          $reason = t('The bit before the "@" contains a character that needs to be in quotes.');
          $is_valid = FALSE;
        }
      }
      if ($is_valid && !(checkdnsrr($domain, "MX") || checkdnsrr($domain, "A"))) {
        // domain not found in DNS
        $reason = t('The domain does not appear to be a valid one.');
        $is_valid = FALSE;
      }
    }
    return $is_valid;
  }

  function urop_get_disk_space_usage() {
    $df = urop_format_size(disk_free_space("/"));
    $result = '<div class="messages status">';
    $result .= "Current Disk free space: $df.";
    $result .= '</div>';
    return $result;
  }

  function urop_disk_free_warning() {
    $form['disk_warning'] = array(
      '#type' => 'fieldset',
      '#title' => t('Disk Space Warning'),
      '#collapsible' => TRUE,
    );
    $form['disk_warning']['currently_free'] = array(
      '#value' => urop_get_disk_space_usage()
    );
    $form['disk_warning']['size_in_mb'] = array(
      '#type' => 'textfield',
      '#title' => t('Limit (MB)'),
      '#description' => "The limit before the systems starts to warn of low disk space, in MB.",
      '#required' => TRUE,
      '#default_value' => variable_get(UROP_MINIMUM_DISK_FREE, 100),
      '#size' => 6,
      '#maxlength' => 6,
    );
    $form['disk_warning']['submit'] = _urop_submit();

    return $form;
  }

  function urop_disk_free_warning_validate($form, &$form_state) {
    if (!is_numeric($form_state['values']['size_in_mb'])) {
      form_set_error('size_in_mb', 'The disk size specified is not a number.');
    }
  }

  function urop_disk_free_warning_submit($form, &$form_state) {
    $size_chosen = $form_state['values']['size_in_mb'];
    variable_set(UROP_MINIMUM_DISK_FREE, $size_chosen);
  }


