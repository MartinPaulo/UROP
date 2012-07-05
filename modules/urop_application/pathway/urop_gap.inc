<?php
/**
 * @file
 * Contains a form that allows the user to describe their gap year.
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

function urop_gap_form($form_state, $user_uid) {
  $form['gap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gap years'),
    '#description' => '<p><b>A <a href="http://en.wikipedia.org/wiki/Gap_year" target="_blank">' .
      t('gap year:') . '</a></b> ' . t('Time taken off to travel between life stages...'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['gap']['articles'] = array(
        '#type' => 'multicrud',
        '#multicrud_elements' => array(
          'started' => array(
            '#title' => t('Start date'),
            '#type' => 'textfield',
            '#size' => 8,
            '#description' => "dd/mm/yyyy",
  ),
          'ended' => array(
            '#title' => t('End date'),
            '#type' => 'textfield',
            '#size' => 8,
            '#description' => "dd/mm/yyyy",
  ),
          'desc' => array(
            '#title' => t('Description'),
            '#type' => 'textfield',
            '#size' => 50,
            '#required' => TRUE,
            '#description' => "Just a short sentance or two.",
  ),
          'pid' => array(
            '#type' => 'value',
  ),
  ),
  //'#default_value' => urop_gaps_get_values($user_uid),
  );
  $form['gap']['submit'] = _urop_submit();
  return $form;
}
