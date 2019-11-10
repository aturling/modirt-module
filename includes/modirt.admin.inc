<?php
/*
* @file
* This file includes functions handling the admin interface for MO DIRT.
*/


/**
 * Callback for lab results spreadsheet upload form.
 *
 * This form has three stages:
 * 1) Date and file upload.
 * 2) Review results from processing spreadsheet.
 * 3) Display summary of updates.
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_admin_spreadsheet_upload_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'dirt', 'includes/dirt.admin');

  // Add custom CSS and JS to form
   _modirt_admin_attach_to_spreadsheet_form($form);

  // Get current stage of form (default 1)
  $stage = (isset($form_state['store']['stage'])) ? $form_state['store']['stage'] : 1;

  // Set the form stage
  $form_state['store']['stage'] = $stage;

  // Three stages:
  switch($stage) {
    case 1:
      // Stage 1: Date and file upload
      return modirt_date_and_file_upload_form($form, $form_state);

    case 2:
      // Stage 2: Review results from processing spreadsheet
      return modirt_spreadsheet_review_form($form, $form_state);
   
    case 3:
      // Stage 3: Display summary of updates
      return modirt_spreadsheet_updates_confirmation_form($form, $form_state);
  }
}


/**
 * Callback for date and file upload portion of form (stage 1).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 *
 * @retval Object Form array.
 */
function modirt_date_and_file_upload_form($form, &$form_state) {
  // Add custom CSS and JS to form
   _modirt_admin_attach_to_spreadsheet_form($form);

  // Header and instructions
  $form['modirt_header_and_instructions'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('Use the following interface to add soil lab ' .
      'testing data by uploading the spreadsheet of results.') . '<br>' .
      t('To begin, enter the survey date (month and year) for the results, ' .
      'and use the file upload to submit the spreadsheet to be processed. ' .
      'The spreadsheet must have the format outlined below.') . '</p>',
    '#prefix' => '<h2>' . t('Data Entry for Soil Lab Results') . '</h2>',
    '#weight' => 0,
  );

  // Date of survey (month and year)
  $form['modirt_upload_survey_date'] = array(
    '#type' => 'date_select',
    '#title' => t('Survey date'),
    '#date_format' => DIRT_SURVEY_DATE_SHORT_FORMAT,
    '#description' => t('Survey date for lab results data (month and year). ' .
      'The day will be automatically set to the 15th of the month by ' .
      'convention.'),
    '#date_label_position' => 'within',
    '#date_year_range' => '-5:+5',
    '#default_value' => '',
    '#weight' => 1,
    '#required' => TRUE,
  );

  // Spreadsheet file upload
  $form['modirt_file_upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload the soil lab testing results spreadsheet:'),
    '#description' => '<strong><em>' . t('Important: File format must be XLS ' .
      'or CSV. Please convert the spreadsheet to one of these file types ' .
      'before uploading.') . '</em></strong>',
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xls csv'),
    ),
    '#default_value' => '',
    '#weight' => 2,
    '#required' => TRUE,
  );

  // Spreadsheet upload notes
  _modirt_admin_add_spreadsheet_upload_notes_to_form($form);

  // Process spreadsheet button
  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array(
      'class' => array('form-actions', 'form-wrapper'),
    ),
    '#value' => t('Process spreadsheet'),
    '#validate' => array('modirt_process_spreadsheet_validate'),
    '#submit' => array('modirt_process_spreadsheet_submit'),
    '#weight' => 4,
    );

  return $form;
}


/**
 * Validation handler for spreadsheet upload form (stage 1).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_process_spreadsheet_validate($form, &$form_state) {
  // Check that PHPExcel library exists
  modirt_admin_PHPExcel_installed($form, $form_state);
}


/**
 * Submit handler for spreadsheet upload form (stage 1).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_process_spreadsheet_submit($form, &$form_state) {
  if (!form_get_errors()) {
    // Rebuild form
    $form_state['rebuild'] = TRUE;

    // Increment to next stage
    $form_state['store']['stage']++;

    // Store the submitted date and file ID (fid)
    $form_state['store']['fid'] = $form_state['values']['modirt_file_upload'];
    // Convention is to use 15th day of month for survey date
    $survey_date = $form_state['values']['modirt_upload_survey_date'];
    $survey_year = date('Y', strtotime($survey_date));
    $survey_month = date('m', strtotime($survey_date));
    $survey_date = date(DIRT_SURVEY_DATE_FORMAT, mktime(0, 0, 0, $survey_month, 15, $survey_year));
    $form_state['store']['date'] = $survey_date; 
  }
}


/**
 * Callback for review results from processing spreadsheet portion of form
 * (stage 2).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 *
 * @retval Object Form array.
 */
function modirt_spreadsheet_review_form($form, &$form_state) {
  // Add custom CSS and JS to form
   _modirt_admin_attach_to_spreadsheet_form($form);

  // Get spreadsheet file ID (fid) from store
  $spreadsheet_fid = $form_state['store']['fid'];
 
  // Get the survey date from store
  $survey_date = $form_state['store']['date'];

  if (!empty($spreadsheet_fid)) {
    $spreadsheet_file = file_load($spreadsheet_fid);
    $spreadsheet_file_path = $spreadsheet_file->uri;

    // Process spreadsheet and store results in array
    $results = modirt_admin_process_spreadsheet_file($spreadsheet_file_path);

    // For each row in the spreadsheet, get the user/site id it belongs to and
    // the node id (nid) of the soil lab results survey that should be updated
    // (or set to 0 if new one needs to be created).
    $form_info = modirt_admin_get_users_and_survey_nids($results, $survey_date);

    // Add the summary table and related form items
    if (!empty($form_info))
      modirt_admin_add_spreadsheet_summary_form($form, $form_info, $survey_date);
    
    // Store the spreadsheet parsing results
    $form_state['store']['form_info'] = $form_info;
  }
  else {
    drupal_set_message(t('An error has occurred while processing the ' .
      'spreadsheet. Please contact the site administrator.'), 'error');

    // Log error
    watchdog('modirt', 'Cannot process spreadsheet with empty File ID.',
      array(), WATCHDOG_ERROR);
  }

  return $form;
}


/**
 * Submit handler for processing spreadsheet (stage 2).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_admin_process_field_updates_submit($form, &$form_state) {
  if (!form_get_errors()) {
    // Rebuild form
    $form_state['rebuild'] = TRUE;

    // Increment to next stage
    $form_state['store']['stage']++;

    // Get user-submitted values
    $values = $form_state['values'];

    // Get survey date from store
    $survey_date = $form_state['store']['date'];

    // Get spreadsheet data from store
    $form_info = $form_state['store']['form_info'];

    // Get tableselect values (whether boxes checked or not)
    $table_values = $form_state['values']['modirt_select_table'];

    // Lab results column names
    $columns = array_keys(_modirt_admin_get_mu_lab_results_column_headers() + _modirt_admin_get_veum_lab_results_column_headers());

    // Create array to store confirmation of data population
    $confirm = array();

    // Loop through spreadsheet rows and apply updates if checkbox was checked
    // for that row:
    foreach($form_info as $idx => $info_array) {
      $site_id = $info_array['site_id'];

      if (isset($table_values[$site_id]) && $table_values[$site_id]) {
        $survey_nid = $info_array['survey_nid'];
        $soil_collection_date = $info_array['soil_collection_date'];

        $fields = array();
        foreach ($columns as $column) {
          $fields[$column] = $info_array[$column];
        }

        // Update (or create new) soil lab results survey with lab results,
        // store status in $status array.
        $status = modirt_admin_update_soil_lab_results_survey_with_lab_results($site_id, $survey_nid, $survey_date, $soil_collection_date, $fields);
        $confirm[$site_id] = $status;
      }
    }

    $form_state['store']['confirm'] = $confirm;
  }
}


/**
 * Callback for display summary of updates portion of form (stage 3).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 *
 * @retval Object Form array.
 */
function modirt_spreadsheet_updates_confirmation_form($form, &$form_state) {
  // Add custom CSS and JS to form
   _modirt_admin_attach_to_spreadsheet_form($form);

  $confirm = $form_state['store']['confirm'];

  // Add confirmation markup
  $form['modirt_instructions'] = array(
    '#markup' => t('Below are the results of updating the soil lab surveys ' .
      'with the spreadsheet data.') . '<br><br>',
    '#prefix' => '<h2>' . t('Confirmation') . '</h2>',
    '#weight' => -20, // push to top of page
  );

  // Create the table
  $header = _modirt_admin_get_confirmation_table_header();
  $rows = _modirt_admin_get_confirmation_table_rows($confirm);

  $form['modirt_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data found.'),
    '#attributes' => array(
      'id' => 'modirt-summary-table',
      'class' => array('display'),
    ),
    '#weight' => -10,
  );
 
  // Reset form (next time start all over at stage 1)
  $form_state['rebuild'] = TRUE;
  unset($form_state['store']['stage']);

  $form['modirt_submit_reset_button'] = array(
    '#type' => 'submit',
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#value' => t('Upload another spreadsheet'),
    '#weight' => 100,
    '#attributes' => array(
      'class' => array('form-actions', 'form-wrapper'),
    ),
  );

  return $form;
}


/**
 * Modify the Site ID lookup form to include multiple lookups via spreadsheet
 * upload.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_admin_alter_site_id_lookup_form(&$form, &$form_state) {
  // Modify form depending on which stage we're at:
  $stage = isset($form_state['store']['stage']) ? $form_state['store']['stage'] : 0;

  switch($stage) {
    case 1:
      // Modify instructions markup to include multiple lookup option
      $form['dirt_header_and_instructions']['#markup'] .= '<br>' . t('Begin ' .
        'by choosing whether to perform an') . '<strong>' . t('individual ' .
        'site lookup') . '</strong> ' . t('or a') . ' <strong>' . 
        t('multiple sites lookup') . '</strong>.';

      // Add overall Site ID lookup header
      $form['#prefix'] = '<h2>' . t('Site ID Lookup') . '</h2>'; 
  
      // Lookup types available:
      $lookup_types = array(
        t('Individual site lookup'),
        t('Multiple sites lookup'),
      );

      $items = array();
      $items[] = array(
        '<strong>' . $lookup_types[0] . ':</strong>' . t(' Perform an ' .
          'individual lookup by selecting the habitat type from the drop-' .
          'down list and typing in the first name and last name of the ' .
          'participant.'),
      );
      $items[] = array(
        '<strong>' . $lookup_types[1] . ':</strong>' . t(' Upload a ' .
        'spreadsheet of users and habitats to look up multiple Site IDs at ' .
        'the same time.'),
      );
      $form['modirt_lookup_types'] = array(
        '#theme' => 'item_list',
        '#type' => 'ul',
        '#items' => $items,
        '#suffix' => t('For either method, click') . ' <strong>' . t('Submit') .
          '</strong> ' . t('at the bottom of the page to submit the ' .
          'information for lookup.'),
        '#weight' => -2,
      );

      // Add lookup select
      $form['modirt_lookup_type_select'] = array(
        '#type' => 'radios',
        '#title' => t('Select lookup type'),
        '#default_value' => 0,
        '#options' => $lookup_types,
        '#required' => TRUE,
        '#weight' => -1,
      );

      // Save states
      $individual = array(
        ':input[name="modirt_lookup_type_select"]' => array('value' => 0),
      );
      $multiple = array(
        ':input[name="modirt_lookup_type_select"]' => array('value' => 1),
      );

      // Conditionally display individual vs. multiple site lookup fields
      $form['dirt_site_lookup']['#states'] = array(
        'visible' => $individual,
      );
      $form['dirt_site_lookup']['#title'] = $lookup_types[0];
      $fields = array(
        'habitat_type_select',
        'first_name',
        'last_name',
      );
      foreach($fields as $field) {
        $form['dirt_site_lookup'][$field]['#required'] = FALSE;
        $form['dirt_site_lookup'][$field]['#states'] = array(
          'visible' => $individual,
          'required' => $individual,
        );
      }

      // Add multi lookup
      $form['modirt_multi_site_lookup'] = array(
        '#type' => 'fieldset',
        '#title' => $lookup_types[1],
        '#description' => t('Use the following upload field to look up ' .
          'multiple Site IDs. The spreadsheet must have the format outlined ' .
          'below.'),
        '#weight' => 1,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#states' => array(
          'visible' => $multiple,
        ),
      );

      // Spreadsheet file upload
      $form['modirt_multi_site_lookup']['file_upload_modirt'] = array(
        '#type' => 'managed_file',
        '#title' => t('Upload spreadsheet:'),
        '#description' => '<strong><em>' . t('Important: File format must be ' .
          'XLS or CSV. Please convert the spreadsheet to one of these file ' .
          'types before uploading.') . '</em></strong>',
        '#upload_location' => 'public://',
        '#upload_validators' => array(
          'file_validate_extensions' => array('xls csv'),
        ),
        '#default_value' => '',
        '#weight' => 4,
        '#states' => array(
          'visible' => $multiple,
          'required' => $multiple,
        ),
      );

      // Spreadsheet format notes
      $items = array();
      $items[] = array(
        t('Habitat')
      );
      $items[] = array(
        t('Participant Name (or Full Name)'),
      );
      $form['modirt_multi_site_lookup']['file_upload_format_notes'] = array(
        '#theme' => 'item_list',
        '#type' => 'ul',
        '#items' => $items,
        '#prefix' => '<strong>' . t('Spreadsheet format') . ':</strong> ' . 
          t('The spreadsheet must have at least two columns with headers'),
        '#suffix' => t('These headers must match exactly. The spreadsheet ' .
          'may have additional columns; any other columns will be ignored in ' .
          'the lookup process. Each row of the spreadsheet should include ' .
          'the habitat type and full name of participant for one site.'),
        '#weight' => 3,
        '#states' => array(
          'visible' => $multiple,
        ),
      );

      // Override submit button to handle both cases, and add validation.
      $form['submit']['#submit'] = array('modirt_admin_process_site_id_lookup_submit');
      $form['submit']['#validate'] = array('modirt_admin_process_site_id_lookup_validate');

      break;

    case 3:
      // This is a new stage for multi lookups via spreadsheet (skipping stage
      // 2 since it doesn't apply here).
      // If we're in this stage, we know that multi lookup was selected.
      
      // Process spreadsheet and store results in array
      $spreadsheet_fid = $form_state['store']['file_upload_modirt'];
      $spreadsheet_file = file_load($spreadsheet_fid);
      $spreadsheet_file_path = $spreadsheet_file->uri;
      $results = modirt_admin_process_spreadsheet_file($spreadsheet_file_path);

      $success = FALSE; // Assume unsuccessful until proven otherwise

      if (!empty($results)) {
        // Look at header row to get column index for full name and habitat
        // columns.
        $indices = modirt_admin_get_column_indices_from_spreadsheet_header($results);

        if (array_key_exists('full_name', $indices) && array_key_exists('habitat', $indices)) {
          $success = TRUE;  // Spreadsheet has expected format
         
          // Remove header row now that we're done with it
          unset($results[0]);

          // For each row in the spreadsheet, find the user/site id it belongs to
          $form_info = modirt_admin_find_site_ids_from_spreadsheet($results, $indices);

          // Add the summary table and related form items
          modirt_admin_add_site_id_lookup_results_table($form, $form_info);
        } 
      }

      if (!$success) {
        drupal_set_message(t('Spreadsheet could not be processed. Please ' .
          'make sure that the spreadsheet has the expected format.'), 'error');

        watchdog('modirt', 'Uploaded spreadsheet does not have the expected ' .
          'format. Processed spreadsheet rows:<br><pre>%rows</pre>',
          array('%rows' => print_r($results, true)), WATCHDOG_ERROR);
      }

      // Reset form - next time will begin at stage 1:
      $form_state['rebuild'] = TRUE;
      unset($form_state['store']['stage']);

      $form['modirt_reset_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Perform another lookup'),
        '#prefix' => '<div id="perform-another-lookup-button">',
        '#suffix' => '</div>',
        '#attributes' => array(
          'class' => array('form-actions', 'form-wrapper'),
        ),
       '#weight' => 100,
      );

      break; 
  }
}


/**
 * Validation handler for Site ID lookup form.
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_admin_process_site_id_lookup_validate($form, &$form_state) {
  // Check that PHPExcel library exists
  modirt_admin_PHPExcel_installed($form, $form_state);

  // Check that required fields were entered
  $required_fields = array();
  if ($form_state['values']['modirt_lookup_type_select'] == 0)
    $required_fields = array(
      'habitat_type_select' => t('Habitat type'),
      'first_name' => t('First name'),
      'last_name' => t('Last name'),
    );
  else
    $required_fields = array('file_upload_modirt' => t('File upload'));

  foreach($required_fields as $field => $field_type) {
    if ($form_state['values'][$field] == '')
      form_set_error($field, $field_type . ' ' . t('is required.'));
  }
}


/**
 * Submit handler for the Site ID lookup form (overriding submit handler
 * from DIRT module).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_admin_process_site_id_lookup_submit($form, &$form_state) {
  if (!form_get_errors()) {
    // Store lookup type selection
    $form_state['store']['modirt_lookup_type_select'] = $form_state['values']['modirt_lookup_type_select'];

    if ($form_state['store']['modirt_lookup_type_select'] == 1) {
      // Store submitted spreadsheet file
      $form_state['store']['file_upload_modirt'] = $form_state['values']['file_upload_modirt'];

      // Rebuild form
      $form_state['rebuild'] = TRUE;

      // If we advance to stage 2, the DIRT module form function will attempt
      // the individual site lookup with fields that don't exist.
      // Since there's no way to override this, let's move to a new stage,
      // stage 3. Then we can define new functionality for the multi lookup in
      // this new stage (in the form_alter function).
      $form_state['store']['stage'] = 3;
    }
    else {
      // Call original submit to perform individual lookup, rebuild form, etc.
      dirt_process_site_id_lookup_submit($form, $form_state);
    }
  }
}


/**
 * Process the submitted spreadsheet with PHPExcel library.
 *
 * @param[in] $file_path The URI to the spreadsheet file.
 */
function modirt_admin_process_spreadsheet_file($file_path) {
  $results = array();

  if (($library = libraries_detect('PHPExcel')) && !empty($library['installed']) && libraries_load_files($library)) {
    $input_file_type = PHPExcel_IOFactory::identify($file_path);
    $obj_reader = PHPExcel_IOFactory::createReader($input_file_type);
    $obj_PHPExcel = $obj_reader->load($file_path);
    $worksheet = $obj_PHPExcel->getActiveSheet();

    watchdog('modirt', 'Processing spreadsheet with filename %filename',
      array('%filename' => $file_path), WATCHDOG_INFO);

    $row_idx = -1;
    foreach ($worksheet->getRowIterator() as $row) {
      // Begin by locating the row containing the header columns.
      // Skip all rows above the header row.
      if ($row_idx < 0) {
        // Search for header row
        foreach($row->getCellIterator() as $cell) {
          $value = _modirt_admin_clean_comp_string($cell->getValue());

          // Check for "full name" or "participant name" since all spreadsheet
          // templates have this column.
          if (($value == 'full name') || ($value == 'participant name')) {
            // We're at the header row, begin populating array with this row and
            // all following:
            $row_idx = 0;
            break;
          }
          unset($value);
        }
      }

      // At this point, either header row was found above or $row_idx is still
      // equal to -1.
      if ($row_idx >= 0) {
        // Check if first column is not null, otherwise assume row is empty.
        $first_value = $worksheet->getCellByColumnAndRow(0, $row->getRowIndex())->getValue();

        if (!is_null($first_value)) {
          // Loop through and save values
          foreach($row->getCellIterator() as $cell) {
            $results[$row_idx][] = $cell->getFormattedValue();
          }
        }

        ++$row_idx;
      }
    }
  } 
  else {
    drupal_set_message(t('An error has occurred while processing the ' .
      'spreadsheet. Please contact the site administrator.'), 'error');

    // Log error
    watchdog('modirt', 'Error processing spreadsheet with filename ' .
      '%filename: PHPExcel library not detected.',
      array('%filename' => $file_path), WATCHDOG_ERROR);
  }

  return $results;
}


/**
 * Loop through spreadsheet rows and get site id and node id (nid) of soil lab
 * results survey to be updated with info from fields.
 *
 * @param[in] $results Spreadsheet rows array.
 * @param[in] $survey_date Survey date string.
 *
 * @retval Associative array of site id, name, and <field_name> keys for each of
 *   the lab results fields. (Set nid equal to empty string if new survey form
 *   needs to be created.)
 */
function modirt_admin_get_users_and_survey_nids($results, $survey_date) {
  $form_info = array();

  $success = FALSE; // Assume unsuccessful unless all steps below work out

  if (!empty($results)) {
    // Get column header indices
    $indices = modirt_admin_get_column_indices_from_spreadsheet_header($results, TRUE);

    // Minimally need full name and site ID
    if (array_key_exists('full_name', $indices) && array_key_exists('site_id', $indices)) {
      // Remove header row now that we're done with it
      unset($results[0]);

      // Spreadsheet format appears to be correct
      $success = TRUE;

      foreach($results as $idx => $row) {
        // Set the site id, name, and survey date from the spreadsheet row (has
        // site id).
        _modirt_admin_set_site_id_and_fields_from_spreadsheet_row($form_info, $idx, $row, $indices, TRUE);

        // Find the survey nid(s) corresponding to soil lab results forms to be
        // updated for site id(s).
        $date_idx = array_key_exists('soil_collection_date', $indices) ? $indices['soil_collection_date'] : '';
        _modirt_admin_set_soil_collection_date_from_spreadsheet_row($form_info, $idx, $row, $date_idx);
        _modirt_admin_set_survey_nid_from_spreadsheet_row($form_info, $idx, $row, $survey_date);

        // Set the field values in $form_info
        _modirt_admin_set_lab_field_values_from_spreadsheet_row($form_info, $idx, $row, $indices);
      }
    }
  }

  if (!$success) {
    drupal_set_message(t('Spreadsheet could not be processed. Please make ' .
      'sure that the spreadsheet has the expected format.'), 'error');

    // Log error
    watchdog('modirt', 'Uploaded spreadsheet does not have the expected ' .
      'format. Processed spreadsheet rows:<br><pre>%rows</pre>',
      array('%rows' => print_r($results, true)), WATCHDOG_ERROR);
  }

  return $form_info;
}


/**
 * Get column indices from spreadsheet header, i.e., an associative array
 * specifying which spreadsheet column contains which data (since the
 * columns could be in any order).
 *
 * @param[in] $results Array of spreadsheet data, indexed by row.
 * @param[in] $contains_lab_results *Optional* Whether spreadsheet contains
 *   lab results columns (defaults to false). If so, the lab results
 *   columns are expected to be in $results.
 *
 * @retval Associative array of field name => column index in spreadsheet.
 */
function modirt_admin_get_column_indices_from_spreadsheet_header($results, $contains_lab_results = FALSE) {
  // Initialize return
  $indices = array();

  // Get all lab field column headers
  $lab_fields = _modirt_admin_get_mu_lab_results_column_headers() + _modirt_admin_get_veum_lab_results_column_headers();

  // Normalize column header strings to remove special characters, switch to all
  // lowercase, etc. to make comparison easier and case-insensitive.
  foreach($lab_fields as $field_name => $label) {
    $lab_fields[$field_name] = _modirt_admin_clean_comp_string($label);
  }

  // Add site ID and soil collection date as headers to look for as well.
  $lab_fields['site_id'] = 'site id';
  $lab_fields['soil_collection_date'] = 'soil collection date';

  // At this point we know first row of $results is header row
  foreach($results[0] as $col_idx => $header) {
    // Clean spreadsheet header string too for comparisons
    $header_cleaned = _modirt_admin_clean_comp_string($header);

    // Check for participant name
    if (($header_cleaned == 'participant name') || ($header_cleaned == 'full name')) {
      $indices['full_name'] = $col_idx;
      continue;
    }

    // Look for lab results columns if applicable
    if ($contains_lab_results) {
      foreach($lab_fields as $field_name => $label) {
        if (strpos($header_cleaned, $label) !== FALSE) {
          // Found label in header
          $indices[$field_name] = $col_idx;

          // Remove field from list, no reason to keep searching for its column position
          unset($lab_fields[$field_name]);
       
          break;
        }
      }
    }
    elseif ($header_cleaned == 'habitat') {
      $indices['habitat'] = $col_idx;
      continue;
    }
  }

  return $indices;  
}


/**
 * Find Site IDs from spreadsheet information.
 *
 * @param[in] $results Array of spreadsheet data, indexed by row, where first
 *   row is the first data row (i.e., header row and any other non-data rows
 *   have been excluded).
 * @param[in] $indices Associative array of column index for column data, keyed by
 *   column name (e.g., 'full_name').
 *
 * @retval Associative array of site id, first and last name, and habitat keys,
 *   indexed first by spreadsheet row, where site id value is the site ID for
 *   the name and habitat fields in that row, if found.
 */
function modirt_admin_find_site_ids_from_spreadsheet($results, $indices) {
  // Initialize return
  $form_info = array();

  // Loop through spreadsheet rows and look up Site ID from full name and
  // habitat. Set first name, last name, habitat, and site id(s) (may be more
  // than one potential match). Last parameter is false because site ID column
  // isn't already present (we're doing lookups).
  foreach ($results as $idx => $row) {
    _modirt_admin_set_site_id_and_fields_from_spreadsheet_row($form_info, $idx, $row, $indices, FALSE);
  }

  return $form_info;
}


/**
 * Create site ID lookup table results and add to form object.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 */
function modirt_admin_add_site_id_lookup_results_table(&$form, $form_info) {
  $form['modirt_explanation'] = array(
    '#type' => 'markup',
    '#markup' => t('The uploaded spreadsheet was processed; below are the ' .
      'results. The participant first name, last name, and habitat type were ' .
      'extracted from each row, and this information was used to locate the ' .
      "participant's survey Site ID in the system."),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#weight' => -20,
  );

  $header = _modirt_admin_get_multi_site_lookup_table_header();
  $rows = _modirt_admin_get_multi_site_lookup_table_rows($form_info);

  $form['modirt_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data found.'),
    '#attributes' => array(
      'id' => 'site-id-lookup-table',
      'class' => array('display'),
    ),
    '#weight' => -10,
  );
}


/**
 * Create summary table and add field(s) to form.
 *
 * @param[in,out] $form Form array.
 * @param[in] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $survey_date Survey date string.
 */
function modirt_admin_add_spreadsheet_summary_form(&$form, $form_info, $survey_date) {
  // Add instructions
  $form['modirt_instructions'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('The uploaded spreadsheet was processed; below ' .
      'are the results. Some rows may be missing if the site ID in the ' .
      'spreadsheet was invalid or unable to be processed.') . '</p><p>' .
      t('The participant full name corresponding to the site ID is ' .
      'included to verify that it matches the name in the spreadsheet. The ' .
      'survey column indicates which existing Soil Lab Results survey in the ' .
      'system will be updated with the lab results from the spreadsheet. If ' .
      'there is no existing Soil Lab Results survey with the selected survey ' .
      'date for the Site ID, a new survey will be automatically generated ' .
      'and updated with the lab results.') . '</p><p>' . t('Check the box at ' .
      'the beginning of each row to have the Soil Lab Results survey for ' .
      'that row populated with the lab results field values. Leave the ' .
      'checkbox unchecked to skip that row (e.g., if the names do not match, ' .
      'or the values do not seem to have been correctly parsed from the ' .
      'spreadsheet.)') . '</p><p>' . t('When finished reviewing the table, ' .
      'click on the Update button at the bottom of the page to commit these ' .
      ' values to the indicated Soil Lab Results surveys.') . '</p>',
    '#prefix' => '<h2>' . t('Review data') . '</h2><p>',
    '#suffix' => '</p>',
    '#weight' => -20,
  );

  // Add survey date header
  $form['modirt_survey_date_header'] = array(
    '#type' => 'markup',
    '#markup' => '<h4>' . t('Lab Results for Survey Date: @date',
      array('@date' => $survey_date)) . '</h4>',
    '#suffix' => '<p>' . t('Check the box to update/create new Soil Lab ' .
      'Results survey for that row.') . '</p>',
    '#weight' => -19,
  );

  // Add the select table of surveys to add/update
  $header = _modirt_admin_get_lab_survey_select_table_header();
  $options = _modirt_admin_get_lab_survey_select_table_options($form_info, $survey_date);

  // Default to checked
  $default_value = array();
  foreach($form_info as $idx => $info_array) {
    $site_id = $info_array['site_id'];
    $default_value[$site_id] = TRUE;
  }

  $form['modirt_select_table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No results found.'),
    '#attributes' => array(
      'id' => 'modirt-summary-table',
      'class' => array('display'),
    ),
    '#multiple' => TRUE,
    '#default_value' => $default_value,
    '#weight' => -10,
  );

  // Submit
  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array(
       'class' => array('form-actions', 'form-wrapper'),
    ),
    '#value' => 'Update selected surveys',
    '#validate' => array('modirt_admin_process_field_updates_validate'),
    '#submit' => array('modirt_admin_process_field_updates_submit'),
    '#weight' => 2,
  );
}


/**
 * Validation handler for confirmation form (stage 2).
 *
 * @param[in] $form Form array.
 * @param[in,out] $form_state Form state array.
 */
function modirt_admin_process_field_updates_validate($form, &$form_state) {
  // Must have at least one box checked
  $one_checked = FALSE;
  foreach($form_state['values']['modirt_select_table'] as $site_id => $checked) {
    if ($checked) {
      $one_checked = TRUE;
      break;
    }
  }

  if (!$one_checked)
    form_set_error('modirt_select_table', t('At least one checkbox must be checked.')); 
}


/**
 * Update the soil lab results survey given by survey nid with spreadsheet field
 * values.
 *
 * @param[in] $site_id Site ID integer.
 * @param[in] $survey_nid Lab survey node ID (nid) to update. If empty, a new
 *   lab survey will be created for this site.
 * @param[in] $survey_date Survey date string.
 * @param[in] $soil_collection_date Soil collection date string.
 * @param[in] $fields Array of field values to set in lab survey.
 */
function modirt_admin_update_soil_lab_results_survey_with_lab_results($site_id, $survey_nid, $survey_date, $soil_collection_date, $fields) {
  $status = array();

  // If survey nid is empty, create new survey. Otherwise, load survey in system
  // to update.
  $node = (empty($survey_nid)) ?  _modirt_admin_create_new_lab_results_survey_node($site_id) : node_load($survey_nid);
  $nid = '';

  // Update lab results fields
  try {
    $node_wrapper = entity_metadata_wrapper('node', $node);
    foreach($fields as $field_name => $value) {
      // If new field value exists, overwrite existing value, otherwise preserve
      // existing value
      if ($value != '') $node_wrapper->$field_name->set($value);
    }

    // Set soil collection date
    if (!empty($soil_collection_date))
      $node_wrapper->field_modirt_lab_collection_date->set($soil_collection_date);

    // Set date of survey
    $node_wrapper->field_dirt_date_of_survey->set(strtotime(date($survey_date)));

    // Save changes
    $node_wrapper->save();
    node_save($node);

    // Get node nid
    $nid = $node->nid;    
  }
  catch (EntityMetadataWrapperException $e) {
    watchdog_exception('modirt', $e);
  }

  // Return success/failure
  $status['success'] = false;
  $status['nid'] = $nid;
  if (!empty($nid)) $status['success'] = TRUE;

  return $status;  
}


/**
 * Returns whether Site ID is a valid Data Entry account.
 * 
 * @param[in] $site_id Site ID integer to check.
 *
 * @retval Boolean whether Site ID is a valid Data Entry account.
 */
function modirt_admin_is_valid_site_id($site_id) {
  $user_obj = user_load($site_id);

  // Get data entry user role ID
  $data_entry_user_rid = variable_get('dirt_data_entry_user_role');

  return (!empty($user_obj) && user_has_role($data_entry_user_rid, $user_obj));
}


/**
 * Checks that PHPExcel library is installed, which is needed to process Excel
 * spreadsheets. Call this function within a validation handler. If PHPExcel
 * library missing, sets form error.
 *
 * @param[in,out] $form Form array.
 * @param[in,out] $form_state Form state array.
 *
 * @retval Boolean Returns TRUE if PHPExcel library installed, FALSE otherwise.
 */
function modirt_admin_PHPExcel_installed(&$form, &$form_state) {
  // Initialize return value (assume failure and set to TRUE upon success).
  $retVal = FALSE;

  $library = libraries_detect('PHPExcel');

  if ($library && (version_compare($library['version'], '1.7.0') >= 0))
    $retVal = TRUE;
  else
    form_set_error('modirt_file_upload', t('Required library PHPExcel is ' .
      'missing or version is <= 1.7. Please contact the site administrator.'));

  return $retVal;
}


/**
 * Helper function to add the spreadsheet upload notes (required format,
 * etc.)
 *
 * @param[in,out] $form Form array.
 */
function _modirt_admin_add_spreadsheet_upload_notes_to_form(&$form) {
  // Spreadsheet upload notes
  $form['modirt_upload_notes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Required spreadsheet format'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 3,
  );

  // Template downloads
  $sample_file = '/' . drupal_get_path('module', 'modirt') . '/files/Results_sample.xls';
  $template_file = '/' . drupal_get_path('module', 'modirt') . '/files/Results_template.xls';

  $form['modirt_upload_notes']['modirt_template_downloads'] = array(
    '#type' => 'markup',
    '#markup' => t('Download a spreadsheet template ') .
      l(t('here'), $template_file) . '.<br>' . t('Download a spreadsheet ' .
      'sample ') . l(t('here'), $sample_file) . '.',
    '#prefix' => '<div id="modirt-spreadsheet-template-download">',
    '#suffix' => '</div>',
    '#weight' => 0,
  );

  // Spreadsheet format notes (required columns, etc.)
  $form['modirt_upload_notes']['modirt_spreadsheet_format'] = array(
    '#type' => 'container',
    '#weight' => 1,
    '#attributes' => array(
      'id' => 'modirt-spreadsheet-format-notes',
    ),
  );

  // Columns for all lab results
  $items = array();
  $items[] = array(
    t('Site ID'),
  );
  $items[] = array(
    t('Participant Name (or Full Name)'),
  );
  $items[] = array(
    t('Soil Collection Date'),
  );
  $form['modirt_upload_notes']['modirt_spreadsheet_format']['modirt_columns_all'] = array(
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#items' => $items,
    '#prefix' => t('The spreadsheet must have at least three columns with ' .
      'headers:'),
    '#suffix' => t('The soil collection date column should either include ' .
      'the dates in the format @form1 or @form2. It should also have ' .
      'the column headers for the lab results fields.', array(
      '@form1' => DIRT_SURVEY_DATE_HINT,
      '@form2' => MODIRT_LAB_DATE_SHORT_HINT)) . '<br>',
    '#weight' => 0,
  );

  // MU lab results columns:
  $lab_results_columns = _modirt_admin_get_mu_lab_results_column_headers();

  $items = array();
  foreach($lab_results_columns as $field_name => $label) {
    $items[] = array($label);
  }
  $form['modirt_upload_notes']['modirt_spreadsheet_format']['modirt_columns_mu'] = array(
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#items' => $items,
    '#prefix' => t('The spreadsheet with results from MU Lab should have the ' .
      'following columns:'),
    '#weight' => 1,
  );

  // Veum lab results columns:
  $lab_results_columns = _modirt_admin_get_veum_lab_results_column_headers();

  $items = array();
  foreach($lab_results_columns as $field_name => $label) {
    $items[] = array($label);
  }
  $form['modirt_upload_notes']['modirt_spreadsheet_format']['modirt_columns_veum'] = array(
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#items' => $items,
    '#prefix' => t('The spreadsheet with results from Veum Lab should have ' .
      'the following columns:'),
    '#weight' => 2,
  );
}


/**
 * Helper function to set the site id(s) corresponding to the spreadsheet row.
 * If site id column exists, use that, otherwise find the site id by doing a
 * lookup on first name, last name, and habitat type.
 * Additionally set first name, last name, and site id, (and habitat fields if
 * used in lookup) in $form_info.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $row Spreadsheet row array.
 * @param[in] $indices Associative array of column index for column data, keyed by
 *   column name (e.g., 'full_name', 'site_id').
 * @param[in] $has_site_id Boolean whether $form_info array contains site id column.
 *   (If not, site id lookup will be performed.)
 */
function _modirt_admin_set_site_id_and_fields_from_spreadsheet_row(&$form_info, $idx, $row, $indices, $has_site_id) {
  // Get full name from spreadsheet row and set name components in $form_info
  $fullname_idx = $indices['full_name'];
  _modirt_admin_set_name_from_spreadsheet_row($form_info, $idx, $row, $fullname_idx);

  if ($has_site_id) {
    // Simply set the site id from the column
    $site_idx = $indices['site_id'];
    $form_info[$idx]['site_id'] = (isset($row[$site_idx]) && is_numeric($row[$site_idx]) && modirt_admin_is_valid_site_id($row[$site_idx]) ) ? $row[$site_idx] : '';

    // Look up full name from site id / uid
    _modirt_admin_set_name_from_site_id($form_info, $idx, $form_info[$idx]['site_id']);
  }
  else {
    // Get habitat type from spreadsheet row and set type in $form_info
    $habitat_idx = array_key_exists('habitat', $indices) ? $indices['habitat'] : '';
    $habitat_types = _modirt_admin_get_and_set_habitat_type_from_spreadsheet_row($form_info, $idx, $row, $habitat_idx);

    // Get site id(s) matching first name, last name, and habitat type, and set in $form_info
    $first_name = $form_info[$idx]['first_name'];
    $last_name = $form_info[$idx]['last_name'];
    _modirt_admin_set_site_id_from_name_and_habitat($form_info, $idx, $first_name, $last_name, $habitat_types);
  }
  
}


/**
 * Helper function to set the soil collection date from the row (expected
 * format: either MODIRT_LAB_DATE_SHORT_FORMAT or DIRT_SURVEY_DATE_FORMAT).
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $row Spreadsheet row array.
 * @param[in] $date_idx Index of soil collection date column in spreadsheet.
 */
function _modirt_admin_set_soil_collection_date_from_spreadsheet_row(&$form_info, $idx, $row, $date_idx) {
  $soil_collection_date = '';  // Default empty

  if ($date_idx != '') {
    $soil_collection_date = $row[$date_idx];

    // Replace dashes with slashes for a consistent format
    $soil_collection_date = str_replace('-', '/', $soil_collection_date);

    // Check validity of date string if nonempty
    if (!empty($soil_collection_date)) {
      // Hard-coded in as MM/DD/YYYY or MM/YYYY.
      // Would need to update regex if survey date formats change.
      $date_regex = "/^\d{1,2}(\/\d{1,2}){0,1}\/\d{4}$/";

      if (!preg_match($date_regex, $soil_collection_date))
        $soil_collection_date = 0; // Invalid format
    }
  }

  $form_info[$idx]['soil_collection_date'] = $soil_collection_date;
}


/**
 * Helper function to the soil lab results survey to be updated for the site id
 * and survey date, and store in $form_info.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $row Spreadsheet row array.
 * @param[in] $survey_date Survey date string.
 */
function _modirt_admin_set_survey_nid_from_spreadsheet_row(&$form_info, $idx, $row, $survey_date) {
  $nid = _modirt_admin_get_soil_lab_results_survey_for_site_id_and_date($form_info[$idx]['site_id'], $survey_date);

  $form_info[$idx]['survey_nid'] = $nid;
}


/**
 * Helper function to set the first name and last name from the full name in the
 * spreadsheet row.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $row Spreadsheet row array.
 * @param[in] $fullname_idx Column index integer containing full name in spreadsheet row.
 */
function _modirt_admin_set_name_from_spreadsheet_row(&$form_info, $idx, $row, $fullname_idx) {
  // Get full name
  $full_name = (array_key_exists($fullname_idx, $row)) ? $row[$fullname_idx] : '';

  // Get first and last name from full name
  $first_name = '';
  $last_name = '';
  if (!empty($full_name)) {
    $name_parts = explode(' ', $full_name);

    // Assuming name is simply first name <space> last name.
    $first_name = (array_key_exists(0, $name_parts)) ? $name_parts[0] : '';
    $last_name = (array_key_exists(1, $name_parts)) ? $name_parts[1] : '';

    // Remove any extra whitespace from names
    $first_name = trim(str_replace("\xc2\xa0", '', $first_name));
    $last_name = trim(str_replace("\xc2\xa0", '', $last_name));
  }

  $form_info[$idx]['first_name'] = $first_name;
  $form_info[$idx]['last_name'] = $last_name;
  $form_info[$idx]['full_name'] = $first_name . ' ' . $last_name;
}


/**
 * Helper function to get the soil lab results survey nid for the site ID with
 * survey date, or return empty string if no existing survey is in the system.
 *
 * @param[in] $site_id Site ID to perform soil lab survey lookup on.
 * @param[in] $survey_date Survey date string to perform soil lab survey
 *   lookup on with matching survey date.
 *
 * @retval String for node ID of lab survey matching site ID and survey date,
 *   if found, or empty string otherwise.
 */
function _modirt_admin_get_soil_lab_results_survey_for_site_id_and_date($site_id, $survey_date) {
  // Initialize return
  $nid = ''; 
 
  if (!empty($site_id)) {
    // Since time is stored in database, look for dates matching Y-m-d beginning at
    // midnight and ending just before midnight of the next day.
    $date_start = date('Y-m-d H:i:s', strtotime($survey_date));
    $date_end = date('Y-m-d H:i:s', strtotime($survey_date . ' + 1 day'));

    // Query on all nodes
    $query = db_select('node', 'n');

    // Limit to this Site ID
    $query->condition('n.uid', $site_id);

    // Limit to lab surveys
    $query->condition('n.type', 'modirt_lab_results_survey');

    // Get node ID field
    $query->fields('n', array('nid')); 
    
    // Limit to surveys matching survey date
    $query->leftJoin('field_data_field_dirt_date_of_survey', 'd', 'n.nid = d.entity_id');
    $query->condition('d.field_dirt_date_of_survey_value', $date_start, '>=');
    $query->condition('d.field_dirt_date_of_survey_value', $date_end, '<');

    $result = $query->execute()->fetchAssoc(); // Expect one result
    if ($result)  $nid = $result['nid'];
  }

  return $nid;
}


/**
 * Helper function to set the lab field values from the spreadsheet row.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $row Spreadsheet row array.
 * @param[in] $indices Associative array of column index for column data, keyed by
 *   column name (e.g., 'full_name', 'site_id').
 */
function _modirt_admin_set_lab_field_values_from_spreadsheet_row(&$form_info, $idx, $row, $indices) {
  $lab_fields = array_keys(_modirt_admin_get_mu_lab_results_column_headers() + _modirt_admin_get_veum_lab_results_column_headers());

  foreach($lab_fields as $field) {
    $col_idx = (array_key_exists($field, $indices)) ? $indices[$field] : '';

    if (array_key_exists($col_idx, $row) && ($row[$col_idx] != '')) {
      // Store based on field type (decimal vs. integer/float):
      $field_info = field_info_field($field);
      $type = $field_info['type'];
      if ($type == 'number_decimal') {
        $scale = $field_info['settings']['scale'];
        $decimal_separator = $field_info['settings']['decimal_separator'];
      
        // Store rounded to scale with specified decimal separator from field info,
        // but do not use comma separator:
        $form_info[$idx][$field] = number_format(round($row[$col_idx], $scale), 1, $decimal_separator, '');
      }
      elseif ($type == 'number_integer') {
        // Round to nearest integer
        $form_info[$idx][$field] = round($row[$col_idx]);
      }
      else {
        // Store as-is
        $form_info[$idx][$field] = $row[$col_idx];
      }
    }
    else {
      $form_info[$idx][$field] = '';
    }
  }

  unset($lab_fields);
}


/**
 * Helper function to set the full name from the site id / uid in database.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 */
function _modirt_admin_set_name_from_site_id(&$form_info, $idx, $site_id) {
  $first_name = '';
  $last_name = '';

  // Run db query
  if (!empty($site_id)) {
    $query = db_select('users', 'u'); // Search users
    $query->condition('u.uid', $site_id); // Restrict to user with uid = site_id
    $query->leftJoin('field_data_field_dirt_user_first_name', 'first_name', 'u.uid = first_name.entity_id');
    $query->leftJoin('field_data_field_dirt_user_last_name', 'last_name', 'u.uid = last_name.entity_id');
    $query->fields('first_name', array('field_dirt_user_first_name_value'));
    $query->fields('last_name', array('field_dirt_user_last_name_value'));
    $result = $query->execute()->fetchAssoc();  // One result expected
    if ($result) {
      $first_name = $result['field_dirt_user_first_name_value'];
      $last_name = $result['field_dirt_user_last_name_value'];
    }
  }

  $form_info[$idx]['full_name_db'] = $first_name . ' ' . $last_name;
}


/**
 * Helper function to set the habitat type from the spreadsheet row.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $row Spreadsheet row array.
 * @param[in] $habitat_idx Column index integer of habitat type data in spreadsheet row.
 *
 * @retval Array of habitat type strings to search for string matches.
 */
function _modirt_admin_get_and_set_habitat_type_from_spreadsheet_row(&$form_info, $idx, $row, $habitat_idx) {
  $habitat_type_str = (array_key_exists($habitat_idx, $row)) ? $row[$habitat_idx] : '';
  $form_info[$idx]['habitat_type'] = $habitat_type_str;

  // Habitat types aren't formatted well in spreadsheet; just search on all the
  // terms in the string.
  $habitat_type_str = strtolower($habitat_type_str);
  $habitat_types = explode(' ', $habitat_type_str);  // Add string parts
  $habitat_types[] = $habitat_type_str; // Add in full string too

  // Special cases:
  if (substr($habitat_type_str, 0, 6) === 'animal')
    $habitat_types[] = 'animal pasture';
  elseif ($habitat_type_str === 'cropland')
    $habitat_types = array('cropland tillage', 'cropland no-tillage');

  return $habitat_types;
}


/**
 * Helper function to set the site id(s) corresponding to the spreadsheet row
 * by matching first name, last name, and habitat type with a site id in the
 * system.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $first_name First name string.
 * @param[in] $last_name Last name string.
 * @param[in] $habitat_types Array of habitat type strings.
 */
function _modirt_admin_set_site_id_from_name_and_habitat(&$form_info, $idx, $first_name, $last_name, $habitat_types) {
  // Run database query to find users matching first and last name (possibly
  // will get multiple results).
  $uids = dirt_admin_query_db_for_user_ids($first_name, $last_name);

  // Run a second database query to get habitat type(s) for user id(s) and
  // narrow list of user ids down to the one site id matching the spreadsheet
  // row, if possible.
  _modirt_admin_get_and_set_site_ids_from_habitat_types($form_info, $idx, $uids, $habitat_types);
}


/**
 * Helper function to get habitat types for uids and compare to habitat types
 * array to search for site id matching spreadsheet row.
 *
 * @param[in,out] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $idx Form info row index integer.
 * @param[in] $uids Array of user ID(s) matching user-entered participant name.
 * @param[in] $habitat_types Array of habitat type strings.
 */
function _modirt_admin_get_and_set_site_ids_from_habitat_types(&$form_info, $idx, $uids, $habitat_types) {
  $site_ids = array();
  $habitats = array();

  foreach($uids as $uid) {
    // Get site description survey for uid/site id
    $nid = dirt_get_most_recent_survey_of_type_for_user($uid, 'dirt_site_description_survey');

    if ($nid) {
      // Get habitat type from survey
      $habitat_type = '';
      
      try {
        $node_wrapper = entity_metadata_wrapper('node', $nid);
        $habitat_type = $node_wrapper->field_dirt_site_habitat_type->value();
      }
      catch (EntityMetadataWrapperException $e) {
        watchdog_exception('modirt', $e);
      }

      // Check if habitat type matches user input. If so, add site ID as a
      // match.
      if (in_array(strtolower($habitat_type), $habitat_types, true)) {
         $site_ids[] = $uid;
         $habitats[] = $habitat_type;
      }
    }

    unset($nid);
  }

  $form_info[$idx]['site_ids'] = $site_ids;
  $form_info[$idx]['habitat_type_db'] = $habitats;
}


/**
 * Helper function to get MU lab results column headers.
 *
 * @retval Associative array of column header strings keyed by their field name.
 */
function _modirt_admin_get_mu_lab_results_column_headers() {
  // Not using t() here because columns need to match spreadsheet template.

  return array(
    'field_modirt_lab_ph' => 'pHs',
    'field_modirt_lab_na' => 'N.A. meq/100g',
    'field_modirt_lab_organic_matter' => '%OM',
    'field_modirt_lab_phosphorus' => 'P Bray I lb/A',
    'field_modirt_lab_calcium' => 'Ca lb/A',
    'field_modirt_lab_magnesium' => 'Mg lb/A',
    'field_modirt_lab_potassium' => 'K lb/A',
    'field_modirt_lab_cec' => 'CEC meq/100g',
    'field_modirt_lab_nitrate' => 'NO3-N ppm',
  );
}


/**
 * Helper function to get Veum lab results column headers.
 *
 * @retval Associative array of column header strings keyed by their field name.
 */
function _modirt_admin_get_veum_lab_results_column_headers() {
  // Not using t() here because columns need to match spreadsheet template.

  return array(
    'field_modirt_lab_wsa' => '%WSA',
    'field_modirt_lab_active_carbon' => 'AC (lbs/A)',
  );
}


/**
 * Helper function to get multi site ID lookup results table header (stage 3).
 */
function _modirt_admin_get_multi_site_lookup_table_header() {
  return array(
    0 => array(
      'data' => t('Row', array(), array('context' => 'Table row')),
      'class' => array('row-num'),
    ),
    1 => array(
      'data' => t('Site ID'),
      'class' => array('site-id'),
    ),
    2 => array(
      'data' => t('Full Name'),
      'class' => array('full-name'),
    ),
    3 => array(
      'data' => t('First Name'),
      'class' => array('first-name'),
    ),
    4 => array(
      'data' => t('Last Name'),
      'class' => array('last-name'),
    ),
    5 => array(
      'data' => t('Habitat (Spreadsheet)'),
      'class' => array('habitat-type'),
    ),
    6 => array(
      'data' => t('Habitat (Database)'),
      'class' => array('habitat-type-db'),
    ),
  );
}


/**
 * Helper function to get multi site ID lookup results table rows (stage 3).
 *
 * @param[in] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 */
function _modirt_admin_get_multi_site_lookup_table_rows($form_info) {
  $rows = array();

  foreach($form_info as $idx => $info_array) {
    // Get site ID markup
    $site_ids = $info_array['site_ids'];
    $site_id_markup = t('No match found');
    if (!empty($site_ids)) {
      dirt_admin_create_site_id_links($site_ids);
      $site_id_markup  = (count($site_ids) > 1) ? t('Multiple matches found') . ': ' : '';
      $site_id_markup .= implode(', ', $site_ids);
    }
    
    // Get habitats (from database lookup)
    $habitats = $info_array['habitat_type_db'];
    $habitats = array_unique($habitats);
    $habitat_markup = (count($habitats) > 1) ? t('Multiple matches') . ': ' : '';
    $habitat_markup .= implode(', ', $habitats); 

    $rows[] = array(
      'data' => array(
        array(
          'data' => $idx,
          'class' => array('row-num'),
        ),
        array(
          'data' => $site_id_markup,
          'class' => array('site-id'),
        ),
        array(
          'data' => $info_array['full_name'],
          'class' => array('full-name'),
        ),
        array(
          'data' => $info_array['first_name'],
          'class' => array('first-name'),
        ),
        array(
          'data' => $info_array['last_name'],
          'class' => array('last-name'),
        ),
        array(
          'data' => $info_array['habitat_type'],
          'class' => array('habitat-type'),
        ),
        array(
          'data' => $habitat_markup,
          'class' => array('habitat-type-db'),
        ),
      ),
    );

    unset($site_ids);
    unset($site_id_markup);
    unset($habitats);
    unset($habitat_markup);
  }

  return $rows;
}


/**
 * Helper function to get lab survey select table header (stage 2).
 *
 * @retval Array of table header columns.
 */
function _modirt_admin_get_lab_survey_select_table_header() {
  // Site ID and Name columns
  $header = array(
    'site_id' => t('Site ID'),
    'full_name' => t('Name (File)', array(), array('context' => 'Name from file')),
    'full_name_db' => t('Name (DB)', array(), array('context' => 'Name from database')),
  );

  // Column headers for MU lab results data
  $lab_results_columns = _modirt_admin_get_mu_lab_results_column_headers();
  foreach($lab_results_columns as $field_name => $label) {
    $header[$field_name] = $label;
  }

  // Column headers for Veum lab results data
  $lab_results_columns = _modirt_admin_get_veum_lab_results_column_headers();
  foreach($lab_results_columns as $field_name => $label) {
    $header[$field_name] = $label;
  }

  // Collection date and link to survey
  $header['soil_collection_date'] = t('Collection Date');
  $header['survey_nid'] = t('Lab Survey');

  return $header;
}


/**
 * Helper function to get lab survey select table options (stage 2).
 *
 * @param[in] $form_info Associative array, indexed by spreadsheet row, with
 *   each row containing keys representing spreadsheet or lookup data.
 * @param[in] $survey_date Survey date string.
 *
 * @retval Array of options formatted for tableselect form element that allow
 *   admin to check which lab surveys to add/update, indexed by Site ID.
 */
function _modirt_admin_get_lab_survey_select_table_options($form_info, $survey_date) {
  $options = array();

  // Get lab results headers
  $mu_lab_results_columns = _modirt_admin_get_mu_lab_results_column_headers();
  $veum_lab_results_columns = _modirt_admin_get_veum_lab_results_column_headers();

  foreach($form_info as $idx => $info_array) {
    // Get data for this row
    $site_id = $info_array['site_id'];
    $row_class = (empty($site_id)) ? 'no-checkbox' : 'checkbox';
    
    // Create option row if Site ID valid
    if (!empty($site_id)) {
      $site_ids = array($site_id);
      dirt_admin_create_site_id_links($site_ids);
      $site_id_markup = $site_ids[0];
 
      $full_name = $info_array['full_name'];
      $full_name_db = $info_array['full_name_db'];
 
      // Add warning to markup if names do not match
      if (_modirt_admin_clean_comp_string($full_name) != _modirt_admin_clean_comp_string($full_name_db))
        $full_name_db .= '<div class="names-warning">' . t('Warning: names do ' .
          'not match') . '</div>';

      $soil_collection_date_markup = $info_array['soil_collection_date'];
      if ($soil_collection_date_markup == '0')
        $soil_collection_date_markup = t('Invalid date format');

      $survey_nid = $info_array['survey_nid'];
      if (empty($survey_nid))
        $survey_nid = (empty($site_id))? t('N/A', array(), array('context' => 'Not applicable')) : t('New survey');
      else
        $survey_nid = l($survey_nid, 'node/' . $survey_nid, array('attributes' => array('target'=>'_blank')));

      $survey_nid_markup = '<div id="survey-nid-site-' . $site_id . 
        '" class="survey-nid">' . $survey_nid . '</div>';

      $options[$site_id] = array(
        'site_id' => $site_id_markup,
        'full_name' => $full_name,
        'full_name_db' => $full_name_db,
        '#weight' => $idx,
      );

      // Add lab results
      foreach($mu_lab_results_columns as $field_name => $label) {
        $options[$site_id][$field_name] = $info_array[$field_name];
      }
      foreach($veum_lab_results_columns as $field_name => $label) {
        $options[$site_id][$field_name] = $info_array[$field_name];
      }

      $options[$site_id]['soil_collection_date'] = $soil_collection_date_markup;
      $options[$site_id]['survey_nid'] = $survey_nid_markup;
    }
  }

  return $options;
}


/**
 * Helper function to get confirmation table header (stage 3).
 *
 * @retval Array of table header columns.
 */
function _modirt_admin_get_confirmation_table_header() {
  return array(
    0 => array(
      'data' => t('Site ID'),
      'class' => array('site-id'),
    ),
    1 => array(
      'data' => t('Lab Results Survey Link'),
      'class' => array('survey-nid'),
    ),
    2 => array(
      'data' => t('Status'),
      'class' => array('status'),
    ),
  );
}


/**
 * Helper function to get confirmation table rows (stage 3).
 *
 * @param[in] $confirm Array of lab results upload status, keyed by
 *   site ID.
 */
function _modirt_admin_get_confirmation_table_rows($confirm) {
  $rows = array();

  foreach ($confirm as $site_id => $status) {
    // Get site ID link
    $site_ids = array($site_id);
    dirt_admin_create_site_id_links($site_ids);
    $site_id_link = $site_ids[0];

    // Get survey nid link
    $survey_nid = $status['nid'];
    if (empty($survey_nid)) $survey_nid = '';
    else $survey_nid = l($survey_nid, 'node/' . $survey_nid, array('attributes' => array('target'=>'_blank')));
    $survey_nid_markup = '<div id="survey-nid-site-' . $site_id . '" class="survey-nid">' . $survey_nid . '</div>';

    // Get status
    $status_err = t('Error updating. ') .
      l(t('View site log'), 'admin/reports/dblog', array(
        'attributes' => array('target'=>'_blank')
      )) . t(' for more information.');
    $status_markup = ($status['success']) ? t('Successfully updated') : $status_err;

    $rows[] = array(
      'data' => array(
        array(
          'data' => $site_id_link,
          'class' => array('site-id'),
        ),
        array(
          'data' => $survey_nid_markup,
          'class' => array('survey-nid'),
        ),
        array(
          'data' => $status_markup,
          'class' => array('status'),
        ),
      ),
      'id' => 'row-' . $site_id,
    );
  }

  return $rows;
}


/**
 * Helper function to create new lab survey node.
 *
 * @param[in] $site_id Site ID to create new survey for.
 *
 * @retval Object newly created survey node object.
 */
function _modirt_admin_create_new_lab_results_survey_node($site_id) {
  // Create node skeleton directly (rest of fields will be populated when
  // calling node_submit and node_save).
  $node = new stdClass();
  $node->type = 'modirt_lab_results_survey';
  $node->status = 1;
  $node->uid = $site_id;
  $node->promote = 0;
  $node->language = LANGUAGE_NONE;
 
  // Survey date required, set placeholder value for now (will be updated with rest of fields)
  $node->field_dirt_date_of_survey[LANGUAGE_NONE][0]['value'] = format_date(strtotime('now'), 'custom', 'Y-m-d H:i:s'); 

  // Prepare and save:
  node_submit($node);
  node_save($node);

  return $node;
}


/**
 * Helper function to clean string for comparison: switch to all lowercase
 * (for case-insensitive comparison), remove some special characters.
 *
 * @param[in] $comp String to be compared to another string.
 *
 * @retval String cleaned for comparision.
 */
function _modirt_admin_clean_comp_string($comp) {
  // Change to all lowercase:
  $comp_cleaned = strtolower($comp);

  // Remove some special characters:
  $comp_cleaned = str_replace(',', '', $comp_cleaned); // remove commas
  $comp_cleaned = str_replace('(', '', $comp_cleaned); // remove parentheses
  $comp_cleaned = str_replace(')', '', $comp_cleaned); // ""

  return $comp_cleaned;
}


/**
 * Helper function to add CSS and JS to admin sreadsheet upload form.
 *
 * @param[in,out] $form Form array.
 */
function _modirt_admin_attach_to_spreadsheet_form(&$form) {
  $form['#attached']['css'][] = drupal_get_path('module', 'modirt') . '/css/modirt_admin_page.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'modirt') . '/js/modirt_survey_upload.js';
}

