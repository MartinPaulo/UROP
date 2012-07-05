<?php
/**
 * @file
 * This builds a set of forms that allows applicants to describe their post UROP lives.
 *    *
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
function urop_pathways($user) {
  $result = '<div class="description">Please expand and complete any relevant portion.</div>';
  require_once('urop_honours.inc');
  $result .= drupal_get_form('urop_honours_form', $user->uid);
  require_once('urop_phd.inc');
  $result .= drupal_get_form('urop_phd_form', $user->uid);
  require_once('urop_masters.inc');
  $result .= drupal_get_form('urop_masters_form', $user->uid);
  require_once('urop_graduate.inc');
  $result .= drupal_get_form('urop_graduate_form', $user->uid);
  require_once('urop_post_doc.inc');
  $result .= drupal_get_form('urop_post_doc_form', $user->uid);
  require_once('urop_assistant.inc');
  $result .= drupal_get_form('urop_assistant_form', $user->uid);
  require_once('urop_jobs.inc');
  $result .= drupal_get_form('urop_jobs_form', $user->uid);
  require_once('urop_gap.inc');
  $result .= drupal_get_form('urop_gap_form', $user->uid);
  $result .= _urop_form_footer();
  return $result;
}
