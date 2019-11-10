<?php
/*
* @file
* This file specifies the definitions for custom fields created by this module.
*/

/**
 * Returns fields created by this module
 */
function modirt_get_installed_fields() {
  return array(
    'field_modirt_lab_date_note' => array(
      'field_name' => 'field_modirt_lab_date_note',
      'type' => 'markup',
      'settings' => array(
        'markup' => array(
          'value' => '<p><em>' . t('The survey date is automatically assigned ' .
                     'to the 15th day of the month for consistency across ' .
                     'surveys.') . '</em></p>',
          'format' => 'full_html',
        ),
      ),
    ),
    'field_modirt_lab_collection_date' => array(
      'field_name' => 'field_modirt_lab_collection_date',
      'type' => 'text',
      'settings' => array(
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_ph' => array(
      'field_name' => 'field_modirt_lab_ph',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_nitrate' => array(
      'field_name' => 'field_modirt_lab_nitrate',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_phosphorus' => array(
      'field_name' => 'field_modirt_lab_phosphorus',
      'type' => 'number_integer',
      'settings' => array(
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_phosphorus_ppm' => array(
      'field_name' => 'field_modirt_lab_phosphorus_ppm',
      'type' => 'number_float',
      'settings' => array(
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_potassium' => array(
      'field_name' => 'field_modirt_lab_potassium',
      'type' => 'number_integer',
      'settings' => array(
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_potassium_ppm' => array(
      'field_name' => 'field_modirt_lab_potassium_ppm',
      'type' => 'number_float',
      'settings' => array(
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_calcium' => array(
      'field_name' => 'field_modirt_lab_calcium',
      'type' => 'number_integer',
      'settings' => array(
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_calcium_ppm' => array(
      'field_name' => 'field_modirt_lab_calcium_ppm',
      'type' => 'number_float',
      'settings' => array(
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_magnesium' => array(
      'field_name' => 'field_modirt_lab_magnesium',
      'type' => 'number_integer',
      'settings' => array(
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_magnesium_ppm' => array(
      'field_name' => 'field_modirt_lab_magnesium_ppm',
      'type' => 'number_float',
      'settings' => array(
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_organic_matter' => array(
      'field_name' => 'field_modirt_lab_organic_matter',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_cec' => array(
      'field_name' => 'field_modirt_lab_cec',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_na' => array(
      'field_name' => 'field_modirt_lab_na',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_active_carbon' => array(
      'field_name' => 'field_modirt_lab_active_carbon',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_lab_wsa' => array(
      'field_name' => 'field_modirt_lab_wsa',
      'type' => 'number_decimal',
      'locked' => TRUE,
      'settings' => array(
        'precision' => 10,
        'scale' => 1,
        'decimal_separator' => '.',
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
    'field_modirt_site_agg_stab' => array(
      'field_name' => 'field_modirt_site_agg_stab',
      'type' => 'list_integer',
      'cardinality' => 1,
      'settings' => array(
        'allowed_values' => array(
          1 => 1,
          2 => 2,
          3 => 3,
          4 => 4,
          5 => 5,
          6 => 6,
        ),
        'dirt_admin_only' => array(
          'edit' => TRUE,
          'view' => FALSE,
        ),
      ),
    ),
  );
}


/**
 * Returns field instances added by this module
 */
function modirt_get_installed_instances() {
  $instances = array();

  /*------------------------*/
  /* Soil Lab Survey Fields */
  /*------------------------*/
  $bundle = 'modirt_lab_results_survey';

  // Keep weight counters so that new fields can be easily be added without
  // manually updating weight numbers
  $weight_in_form = 0;

  $instances['field_modirt_lab_date_note'] = array(
    'field_name' => 'field_modirt_lab_date_note',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Survey Date Note'),
    'widget' => array(
      'type'   => 'markup',
      'weight' => $weight_in_form,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'markup_view',
        'module' => 'markup',
        'weight' => $weight_in_form,
      ),
    ),
  );
  $weight_in_form += 1;

  // Lab Characterization Group Begins Here
  // Assign its weight according to $weight_in_form
  variable_set('dirt_group_modirt_lab_results_weight', $weight_in_form);
  $weight_in_form += 1;

  // New weight counter for inside field group
  $weight_in_group = 0;

  $instances['field_modirt_lab_collection_date'] = array(
    'field_name' => 'field_modirt_lab_collection_date',
    'type' => 'text',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Soil Collection Date'),
    'widget' => array(
      'type'   => 'text_textfield',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Enter the Soil Collection Date in the form ' .
      '@form1 or @form2.', array(
      '@form1' => MODIRT_LAB_DATE_SHORT_HINT,
      '@form2' => DIRT_SURVEY_DATE_HINT)),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_ph'] = array(
    'field_name' => 'field_modirt_lab_ph',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('pHs'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('pH measures how acidic/basic a solution is.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_nitrate'] = array(
    'field_name' => 'field_modirt_lab_nitrate',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('Nitrogen in nitrate (NO<sub>3</sub>-N ppm)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Nitrate as nitrogen in parts per million.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_phosphorus'] = array(
    'field_name' => 'field_modirt_lab_phosphorus',
    'type' => 'number_integer',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Phosphorus (P Bray I lb/A)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Bray and Kurtz extractable phosphorous in pounds per acre.'),
  );

  $weight_in_group += 1;

  $instances['field_modirt_lab_phosphorus_ppm'] = array(
    'field_name' => 'field_modirt_lab_phosphorus_ppm',
    'type' => 'number_float',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Phosphorus (P Bray I ppm)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Bray and Kurtz extractable phosphorous in ppm.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_potassium'] = array(
    'field_name' => 'field_modirt_lab_potassium',
    'type' => 'number_integer',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Potassium (K lb/A)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Exchangeable potassium in pounds per acre.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_potassium_ppm'] = array(
    'field_name' => 'field_modirt_lab_potassium_ppm',
    'type' => 'number_float',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Potassium (K ppm)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Exchangeable potassium in ppm.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_calcium'] = array(
    'field_name' => 'field_modirt_lab_calcium',
    'type' => 'number_integer',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Calcium (Ca lb/A)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Exchangeable calcium in pounds per acre.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_calcium_ppm'] = array(
    'field_name' => 'field_modirt_lab_calcium_ppm',
    'type' => 'number_float',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Calcium (Ca ppm)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Exchangeable calcium in ppm.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_magnesium'] = array(
    'field_name' => 'field_modirt_lab_magnesium',
    'type' => 'number_integer',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Magnesium (Mg lb/A)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Exchangeable magnesium in pounds per acre.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_magnesium_ppm'] = array(
    'field_name' => 'field_modirt_lab_magnesium_ppm',
    'type' => 'number_float',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Magnesium (Mg ppm)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Exchangeable magnesium in ppm.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_organic_matter'] = array(
    'field_name' => 'field_modirt_lab_organic_matter',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('Organic Matter (%OM)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'settings' => array(
      'min' => 0,
      'max' => 100,
    ),
    'description' => t('Percent of Organic Matter.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_cec'] = array(
    'field_name' => 'field_modirt_lab_cec',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('Cation-Exchange Capacity (CEC meq/100g)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Cation Exchange Capacity in milliequivalents of ' .
      'solute for every 100 grams of solvent.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_na'] = array(
    'field_name' => 'field_modirt_lab_na',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('Neutralizable Acidity (N.A. meq/100g)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Neutralizable Acidity in milliequivalents of solute ' .
      'for every 100 grams of solvent.  N.A. is used to calculate the amount' .
      'of the effective neutralizable material (ENM) that will bring the pH ' .
      'of the soil to within the optimum range for crop growth'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_active_carbon'] = array(
    'field_name' => 'field_modirt_lab_active_carbon',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('Active Carbon (lbs/A)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Active Carbon in pounds per acre.'),
  );
  $weight_in_group += 1;

  $instances['field_modirt_lab_wsa'] = array(
    'field_name' => 'field_modirt_lab_wsa',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'type' => 'number_decimal',
    'label' => t('Water Stable Aggregates (%WSA)'),
    'widget' => array(
      'type'   => 'number',
      'weight' => $weight_in_group,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(
          'scale' => 1,
        ),
        'weight' => $weight_in_group,
      ),
    ),
    'description' => t('Percent of Water Stable Aggregates.'),
  );
  $weight_in_group += 1;


  /*-------------------------*/
  /* Site Description Fields */
  /*-------------------------*/
  $bundle = 'dirt_site_description_survey';

  $instances['field_modirt_site_agg_stab'] = array(
    'field_name' => 'field_modirt_site_agg_stab',
    'type' => 'list_integer',
    'entity_type' => 'node',
    'bundle' => $bundle,
    'label' => t('Aggregate Stability Class'),
    'widget' => array(
      'type'   => 'options_select',
      // Place at top of admin-only field group on form:
      'weight' => -10,
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        // Place just above site drawing field group on display:
        'weight' =>  variable_get('dirt_group_dirt_site_drawing_weight', 0),
      ),
    ),
  );

  return $instances;
}


/**
 * Returns subset of fields installed in this module to be added to the data
 * search portal.
 */
function modirt_get_installed_survey_field_names() {
  // First get all field names
  $fields = modirt_get_installed_fields();

  // Return field names
  return array_keys($fields);
}


/**
 * Returns MO DIRT header markup for survey forms.
 *
 * @param[in] $type Survey type (machine name) string.
 *
 * @retval String form header markup.
 */
function modirt_get_common_form_header_markup($type) {
  // Header depends on survey type
  $header = '';

  switch($type) {
    case 'dirt_site_description_survey':
      $header = t('One-time data collection');
      break;

    case 'dirt_group_information_survey':
      $header = t('Group information');
      break;

    case 'dirt_monthly_data_survey':
      $header = t('Monthly data collection');
      break;

    case 'dirt_twice_year_data_survey':
      $header = t('Twice a year data collection');
      break;

    case 'modirt_lab_results_survey':
      $header = t('Veum and MU Extension Labs');
      break;
  }

  $logo_path = drupal_get_path('module', 'modirt') . '/images/MO_DIRT_logo_no_border.gif';
  $site_name = variable_get('site_name', '');

  $form_header_markup  = '<div class="modirt-form-header">';
  $form_header_markup .= '<div class="modirt-form-header-img">';
  $form_header_markup .= '<img src="/' . $logo_path . '"></div>';
  $form_header_markup .= '<div class="modirt-form-header-text">';
  $form_header_markup .= '<h2>';
  $form_header_markup .= t('@sitename ', array('@sitename' => $site_name));
  $form_header_markup .= t('Citizen Science Project Soil Survey Form');
  $form_header_markup .= '</h2>';
  if ($header) $form_header_markup .= '<h3>' . $header . '</h3>';
  $form_header_markup .= '</div></div>';

  return $form_header_markup;
}


/**
 * Returns list of MU lab field names.
 *
 * @retval Array of field (machine) names.
 */
function modirt_get_mu_lab_field_names() {
  return array(
    'field_modirt_lab_ph',
    'field_modirt_lab_nitrate',
    'field_modirt_lab_phosphorus',
    'field_modirt_lab_phosphorus_ppm',
    'field_modirt_lab_potassium',
    'field_modirt_lab_potassium_ppm',
    'field_modirt_lab_calcium',
    'field_modirt_lab_calcium_ppm',
    'field_modirt_lab_magnesium',
    'field_modirt_lab_magnesium_ppm',
    'field_modirt_lab_organic_matter',
    'field_modirt_lab_cec',
    'field_modirt_lab_na',
  );
}


/**
 * Returns list of Veum lab field names.
 *
 * @retval Array of field (machine) names.
 */
function modirt_get_veum_lab_field_names() {
  return array(
    'field_modirt_lab_active_carbon',
    'field_modirt_lab_wsa',
  );
}


