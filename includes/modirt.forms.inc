<?php
/*
* @file
* This file contains functions related to forms.
*/


/**
 * Form alter for all survey types for MO DIRT customizations.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 * @param[in] $type Survey type machine name string.
 */
function modirt_all_survey_forms_alter(&$form, &$form_state, $type) {
  // Add custom CSS
  $form['#attached']['css'][] = drupal_get_path('module', 'modirt') . '/css/modirt_forms.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'modirt') . '/css/modirt_form_header.css';

  // Customize survey headers with MO DIRT logo
  modirt_update_form_header($form, $form_state, $type);

  // Add contact e-mail to submission note
  $field = 'field_dirt_form_submission_note';
  $lang = dirt_get_form_field_language($field, $form);
  $email = 'MODirtWebmaster@danforthcenter.org';
  $msg = '<span id="modirt-submission-note-contact">' . t('If you have any ' .
    'questions on entering data, contact ' .
    '<a href="@email">@email</a>.', array('@email' => $email)) . '</span>';
  $form[$field][$lang][0]['markup']['#markup'] .= $msg;
}


/**
 * Form alter for lab results survey form.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_lab_results_survey_form_alter(&$form, &$form_state) {
  form_load_include($form_state, 'inc', 'dirt', 'includes/dirt.fields');  
  form_load_include($form_state, 'inc', 'modirt', 'includes/modirt.fields');

  // Append lab (MU vs. Veum) to field descriptions:
  $mu_lab_fields = modirt_get_mu_lab_field_names();
  $veum_lab_fields = modirt_get_veum_lab_field_names();

  foreach($mu_lab_fields as $field) {
    $lang = dirt_get_form_field_language($field, $form);

    $form[$field][$lang][0]['value']['#description'] .= '<br>' . '<strong>' . t('Results ' .
      'from Mizzou Lab.') . '</strong>';
  }

  foreach($veum_lab_fields as $field) {
    $lang = dirt_get_form_field_language($field, $form);

    $form[$field][$lang][0]['value']['#description'] .= '<br>' . '<strong>' . t('Results ' .
      'from Veum Lab.') . '</strong>';
  }
}


/**
 * Form alter for group survey form.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_group_information_survey_form_alter(&$form, &$form_state) {
  // Hide date of survey field and default to current date
  _modirt_hide_date_of_survey_form_field($form, $form_state);

  // Change "edit" to "update"
  if (isset($form['#node']->nid)) {
    $name = dirt_get_name_of_survey_type('dirt_group_information_survey');
    drupal_set_title(t('Update @name', array('@name' => $name)));
  }
}


/**
 * Form alter for site description survey form.
 * 
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_site_description_survey_form_alter(&$form, &$form_state) {
  // Add custom JS
  $form['#attached']['js'][] = drupal_get_path('module', 'modirt') . '/js/modirt_forms_select.js';

  // Change "edit" to "update"
  if (isset($form['#node']->nid)) {
    $name = dirt_get_name_of_survey_type('dirt_site_description_survey');
    drupal_set_title(t('Update @name', array('@name' => $name)));
  }

  // Restrict state to Missouri and lock field
  $state_field = 'field_dirt_site_state';
  $state_lang = dirt_get_form_field_language($state_field, $form);
  $form[$state_field][$state_lang][0]['value']['#default_value'] = 'MO';
  dirt_lock_form_field($state_field, $form, TRUE);

  // Hide date of survey field and default to current date
  _modirt_hide_date_of_survey_form_field($form, $form_state);

  // Allow updates for some fields: habitat type, topography,
  // notes, land management (past and present)
  $fields = array(
    'field_dirt_site_habitat_type',
    'field_dirt_site_type_of_crop',
    'field_dirt_site_cropping_system',
    'field_dirt_site_topography',
    'field_dirt_site_desc_notes',
    'field_dirt_site_present_manage',
    'field_dirt_site_past_manage',
  );
  foreach ($fields as $field) {
    $lang = dirt_get_form_field_language($field, $form);
    if (array_key_exists('0', $form[$field][$lang]) && array_key_exists('value', $form[$field][$lang][0])) {
      unset($form[$field][$lang][0]['value']['#attributes']['readonly']);;
      $form[$field][$lang][0]['value']['#attributes']['#disabled'] = FALSE;
      // Remove "lock-field" class
      $existing_classes = isset($form[$field][$lang][0]['value']['#attributes']['class']) ? $form[$field][$lang][0]['value']['#attributes']['class'] : array();
      $form[$field][$lang][0]['value']['#attributes']['class'] = array_diff($existing_classes, array('lock-field'));
    }
    elseif (array_key_exists('#attributes', $form[$field][$lang])) {
      unset($form[$field][$lang]['#attributes']['readonly']);
      $form[$field][$lang]['#attributes']['#disabled'] = FALSE;
      // Remove "lock-field" class
      $existing_classes = isset($form[$field][$lang]['#attributes']['class']) ? $form[$field][$lang]['#attributes']['class'] : array();
      $form[$field][$lang]['#attributes']['class'] = array_diff($existing_classes, array('lock-field'));
    }
    else {
      $form[$field][$lang]['#disabled'] = FALSE;
    }
  }
  $desc = t('*** These fields can be updated in the following surveys if necessary.');
  $form['#groups']['group_dirt_site_desc_con_form']->format_settings['instance_settings']['description'] = $desc;

  // Can update past/present land management fields to make additions but not remove any
  // previously selected, unless logged in as admin.
  // Pass user status to JS.
  drupal_add_js(array('modirt' => array('userIsAdmin' => user_access('administer nodes'))),
    array('type' => 'setting'));

  // Additional validation
  $form['#validate'][] = 'modirt_site_description_form_node_validate'; 
}


/**
 * Validation handler for site description survey form.
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_site_description_form_node_validate($form, &$form_state) {
  form_load_include($form_state, 'inc', 'dirt', 'includes/dirt.forms');

  // Geo coords longitudes should be negative to be in Missouri
  $geo_coords_field = 'field_dirt_site_geo_coords';
  $lon_field = 'field_dirt_site_geo_coords_lon';

  if (array_key_exists($geo_coords_field, $form_state['values'])) {
    $geo_coords_lang = dirt_get_form_field_language($geo_coords_field, $form);
    $lon_lang = dirt_get_form_field_collection_field_language($lon_field, $geo_coords_field, $form);

    foreach($form_state['values'][$geo_coords_field][$geo_coords_lang] as $delta => $val_array) {
      if (is_array($val_array)) {
        $lon = $val_array[$lon_field][$lon_lang][0]['value'];

        if ($lon && ($lon >= 0))
          form_set_error($geo_coords_field . '][' . $lon_lang . '][0][' . $lon_field, t('Longitude must be negative.'));
      }
    }
  }
}


/**
 * Form alter for monthly survey form.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_monthly_survey_form_alter(&$form, &$form_state) {
  // Add custom JS
  $form['#attached']['js'][] = drupal_get_path('module', 'modirt') . '/js/modirt_monthly_form.js';

  // For MO DIRT, volume of soil sample and soil bulk density are only
  // calculated once, then these fields are pre-populated on all future monthly
  // surveys.
  $fields = array(
    'field_dirt_volume_soil_sample',
    'field_dirt_soil_bulk_density',
  );

  // Check if in node/add or node/edit:
  if (!isset($form['#node']->nid)) {
    // We're in node/add form.
    // Begin with fields prepopulated from most recent survey, if applicable
    _modirt_prepopulate_monthly_form_fields($form, $fields);
  }

  // For both node/add and node/edit, lock these fields if nonempty:
  _modirt_lock_prepopulated_monthly_form_fields($form, $fields); 
}


/**
 * Updates site county field widget from text field to "select or other" field.
 */
function modirt_change_county_field_widget(&$widget) {
  // Cannot change the field type, but widget "select or other" is pretty close
  // to the select list functionality we want.

  $widget['type'] = 'select_or_other';
  $widget['module'] = 'select_or_other';
  $widget['settings']['other'] = 'Other:';
  $widget['settings']['other_size'] = 60;
  $widget['settings']['other_title'] = '';
  $widget['settings']['other_unknown_defaults'] = 'other';
  $widget['settings']['available_options'] = implode(array(
    'Adair',
    'Andrew',
    'Atchison',
    'Audrain',
    'Barry',
    'Barton',
    'Bates',
    'Benton',
    'Bollinger',
    'Boone',
    'Buchanan',
    'Butler',
    'Caldwell',
    'Callaway',
    'Camden',
    'Cape Girardeau',
    'Carroll',
    'Carter',
    'Cass',
    'Cedar',
    'Chariton',
    'Christian',
    'Clark',
    'Clay',
    'Clinton',
    'Cole',
    'Cooper',
    'Crawford',
    'Dade',
    'Dallas',
    'Daviess',
    'DeKalb',
    'Dent',
    'Douglas',
    'Dunklin',
    'Franklin',
    'Gasconade',
    'Gentry',
    'Greene',
    'Grundy',
    'Harrison',
    'Henry',
    'Hickory',
    'Holt',
    'Howard',
    'Howell',
    'Iron',
    'Jackson',
    'Jasper',
    'Jefferson',
    'Johnson',
    'Knox',
    'Laclede',
    'Lafayette',
    'Lawrence',
    'Lewis',
    'Lincoln',
    'Linn',
    'Livingston',
    'McDonald',
    'Macon',
    'Madison',
    'Maries',
    'Marion',
    'Mercer',
    'Miller',
    'Mississippi',
    'Moniteau',
    'Monroe',
    'Montgomery',
    'Morgan',
    'New Madrid',
    'Newton',
    'Nodaway',
    'Oregon',
    'Osage',
    'Ozark',
    'Pemiscot',
    'Perry',
    'Pettis',
    'Phelps',
    'Pike',
    'Platte',
    'Polk',
    'Pulaski',
    'Putnam',
    'Ralls',
    'Randolph',
    'Ray',
    'Reynolds',
    'Ripley',
    'Saline',
    'Schuyler',
    'Scotland',
    'Scott',
    'Shannon',
    'Shelby',
    'St. Charles',
    'St. Clair',
    'St. Francois',
    'St. Louis City',
    'St. Louis County',
    'Ste. Genevieve',
    'Stoddard',
    'Stone',
    'Sullivan',
    'Taney',
    'Texas',
    'Vernon',
    'Warren',
    'Washington',
    'Wayne',
    'Webster',
    'Worth',
    'Wright',
  ), "\n");
}


/**
 * Updates form header to include MO DIRT logo.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 * @param[in] $type Survey type (machine name) string.
 */
function modirt_update_form_header(&$form, &$form_state, $type) {
  form_load_include($form_state, 'inc', 'modirt', 'includes/modirt.fields');

  // Get header field markup based on survey type
  $header_field = 'field_dirt_form_header';
  $markup = modirt_get_common_form_header_markup($type);

  // Update form element
  $lang = dirt_get_form_field_language($header_field, $form);
  $form[$header_field][$lang][0]['markup']['#markup'] = $markup;
}


/**
 * Helper function to pre-populate monthly form fields that are
 * calculated only once per site. These fields belong to the soil
 * bulk density calculations field collection.
 *
 * @param[in,out] $form Form array.
 * @param[in] $fields Array of fields to prepopulate (all belonging
 *   to the soil bulk density calcs field collection).
 */
function _modirt_prepopulate_monthly_form_fields(&$form, $fields) {
  // Do a database query for both fields on all monthly surveys
  // If there's a value in either field, go ahead and lock it
  // So for each field, either there will be one (locked) value or
  // no value (hasn't been entered yet).
  
  foreach ($fields as $field) {
    $val = _modirt_get_non_null_bulk_density_field($field);
    if ($val) _modirt_prepopulate_bulk_density_field($form, $field, $val);
  }
}


/**
 * Helper function to run database query for monthly surveys by logged-in user
 * to get non-null field value for field belonging to bulk density calcs
 * field collection.
 *
 * @param[in] $field Field name string, belonging to bulk density calcs field
 *   collection.
 *
 * @retval String field value if exists, empty string otherwise.
 */
function _modirt_get_non_null_bulk_density_field($field) {
  $val = '';

  global $user;
  $uid = $user->uid;
  $fc_field = 'field_dirt_bulk_density_calcs';

  $query = db_select('node', 'n');
  $query->condition('n.uid', $uid);
  $query->condition('n.type', 'dirt_monthly_data_survey');
  $query->leftJoin('field_data_' . $fc_field, $fc_field, 'n.nid = ' . $fc_field . '.entity_id');
  $query->leftJoin('field_data_' . $field, $field, $field . '.entity_id = ' . $fc_field . '.' . $fc_field . '_value');
  $query->isNotNull($field . '.' . $field . '_value');
  $query->fields($field, array($field . '_value'));
  $result = $query->execute()->fetchAssoc(); // expect one result

  if ($result) $val = $result[$field . '_value'];

  return $val;
}


/**
 * Helper function to prepopulate bulk density calcs field collection
 * field with given value.
 *
 * @param[in,out] $form Form array.
 * @param[in] $field Field name string, belonging to bulk density calcs field
 *   collection.
 * @param[in] $val Value to pre-populate field with.
 */
function _modirt_prepopulate_bulk_density_field(&$form, $field, $val) {
  $fc_field = 'field_dirt_bulk_density_calcs';
  $fc_lang = dirt_get_form_field_language($fc_field, $form);
  $field_lang = dirt_get_form_field_collection_field_language($field, $fc_field, $form);
  $form[$fc_field][$fc_lang][0][$field][$field_lang][0]['value']['#default_value'] = $val;
}


/**
 * Helper function to lock bulk density calculations field collection
 * fields, if nonempty.
 *
 * @param[in,out] $form Form array.
 * @param[in] $fields Array of fields to lock if nonempty (all belonging
 *   to the soil bulk density calcs field collection).
 */
function _modirt_lock_prepopulated_monthly_form_fields(&$form, $fields) {
  $fc_field = 'field_dirt_bulk_density_calcs';
  $fc_lang = dirt_get_form_field_language($fc_field, $form);

  foreach($fields as $field) {
    $field_lang = dirt_get_form_field_collection_field_language($field, $fc_field, $form);
    if ($form[$fc_field][$fc_lang][0][$field][$field_lang][0]['value']['#default_value'])
      dirt_lock_field_collection_form_field($field, 0, $fc_field, $form);
  }
}


/**
 * Helper function to hide date of survey field in form and default
 * to current date. Only applies to non-admin users.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function _modirt_hide_date_of_survey_form_field(&$form, &$form_state) {
  if (!user_access('administer nodes')) {
    // Get current date
    $today = format_date(time(), 'custom', 'Y-m-d H:i:s');

    // Set default survey date value to current date
    $date_field = 'field_dirt_date_of_survey';
    $date_lang = dirt_get_form_field_language($date_field, $form);
    $form[$date_field][$date_lang][0]['#default_value']['value'] = $today;

    // Hide date of survey field in form
    // Cannot use access=FALSE here for non-admins so hide date
    // with CSS:
    //$form[$date_field]['#access'] = FALSE; // <-- don't do this
    $form[$date_field]['#attributes']['class'][] = 'form-element-hidden';
  }
}

