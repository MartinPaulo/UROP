<?php
/**
 * @file
 * This contains a form that allows applicants to update and view any prizes that
 * they may have received post UROP.
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
function urop_awards($user) {
  $result = '<div class="description">Please expand and complete any relevant portion.</div>';
  require_once('urop_scholarships.inc');
  $result .= drupal_get_form('urop_phd_scholarship_details_form', $user->uid);
  require_once('urop_other_scholarships.inc');
  $result .= drupal_get_form('urop_other_scholarship_details_form', $user->uid);
  require_once('urop_prizes.inc');
  $result .= drupal_get_form('urop_prize_details_form', $user->uid);
  require_once('urop_publications.inc');
  $result .= drupal_get_form('urop_publication_form', $user->uid);
  require_once('urop_articles.inc');
  $result .= drupal_get_form('urop_article_details_form', $user->uid);
  require_once('urop_presentations.inc');
  $result .= drupal_get_form('urop_presentation_details_form', $user->uid);
  require_once('urop_patents.inc');
  $result .= drupal_get_form('urop_patent_details_form', $user->uid);
  $result .= _urop_form_footer();
  return $result;
}
