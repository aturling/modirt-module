<?php
/*
* @file
* This file contains functions that customize the data portal for MO DIRT.
*/


/**
 * Adds MO DIRT customizations to the data entry portal page.
 *
 * @param[in,out] $page Page array.
 */
function modirt_data_entry_portal_alter(&$page) {
  // Placeholder
}


/**
 * Adds MO DIRT customizations to the data search portal page.
 *
 * @param[in,out] $page Page array.
 */
function modirt_data_search_portal_alter(&$page) {
  // Add CSS/JS
  $path_to_modirt = drupal_get_path('module', 'modirt');
  $modirt_url = $path_to_modirt;

  drupal_add_css($path_to_modirt . '/css/modirt_datatable.css');
  drupal_add_js($path_to_modirt . '/js/modirt_data_search_portal.js');
  // Send path to modirt module to JS
  drupal_add_js(array('modirt' => array('modirtURL' => $modirt_url)),
    array('type' => 'setting'));

  // Change title
  $title = t('MO DIRT Data Search Portal');
  drupal_set_title($title);

  // Update title in breadcrumb too
  // Should be last item in breadcrumb array, so remove it and replace it.
  $breadcrumb = drupal_get_breadcrumb();
  array_pop($breadcrumb);
  $breadcrumb[] = $title;
  drupal_set_breadcrumb($breadcrumb);

  // Changes for anonymous users (not logged in)
  if (array_key_exists('login_container', $page['content']['system_main']['overview']['block_container'])) {
    // Change login header
    $markup = t('MO DIRT participant and EPSCoR scientist login to access additional data');
    $page['content']['system_main']['overview']['block_container']['login_container']['header']['#markup'] = $markup;

    // Change data request message
    $citizen_sci_path = variable_get('dirt_citizen_science_portal_path', DIRT_CITIZEN_SCI_PATH_DEFAULT);
    $data_request_path = $citizen_sci_path . '/account-request-form';
    $markup = t('Missouri EPSCoR scientists can request full access to the ' .
      'data by registering') . ' ' .
      l(t('here'), $data_request_path, array('attributes' => array(
        'target'=>'_blank',
        'title'=>t('Request a data viewing account'),
      ))) . '.';
    $page['content']['system_main']['overview']['block_container']['data_access']['request_link']['#markup'] = $markup;
    $page['content']['system_main']['overview']['block_container']['data_access']['request_link']['#prefix'] = '<p>';
    $page['content']['system_main']['overview']['block_container']['data_access']['request_link']['#suffix'] = '</p>';
  }

  // Add data usage survey
  $survey_url = 'https://docs.google.com/forms/d/e/1FAIpQLSfPuPX66xomW2ZqCuS9aQeOT0S1440ywVAtAp3lt42Ul2wv7g/viewform?usp=pp_url';
  $page['content']['system_main']['overview']['block_container']['data_access']['data_usage_survey'] = array(
    '#type' => 'markup',
    '#markup' => t('Please share with us how you have used the MO DIRT data') .
      ' ' . l(t('here'), $survey_url, array('attributes' => array(
      'target'=>'_blank',
      'title'=>t('How do you use the MO DIRT data?'),))) . '.',
  );
}

