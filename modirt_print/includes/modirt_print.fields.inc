<?php
/*
* @file
* This file specifies the definitions for field instances modified by this module.
*/


/**
 * Returns print display information for field instances. Each array member is
 * not a field instance on its own, but it may be merged with the existing field
 * instance to add the print display.
 */
function modirt_print_get_instances() {
  // There were too many customizations to do anything other than hard-code in
  // the print display field instances for those that need to be updated.
  // Need to change display weight in most cases, also need to hide some fields.
  $instances = array();

  $bundles = array(
    0 => 'dirt_site_description_survey',
    1 => 'dirt_monthly_data_survey',
    2 => 'dirt_twice_year_data_survey',
    3 => 'dirt_group_information_survey',
    4 => 'modirt_lab_results_survey',
  );

  /*--------------------------------*/
  /* Site Description Survey Fields */
  /*--------------------------------*/
  $bundle = $bundles[0];

  // Weight counter
  $weight_in_display = 0;

  // Print display
  $print = PRINT_VIEW_MODE;

  // Hide markup fields (notes)
  $instances['field_dirt_site_geo_coords_note'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 10,
      ),
    ),
  );

  $instances['field_dirt_site_map_unit_note'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),  
  );

  $instances['field_dirt_site_drawing_guide'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 12,
      ),
    ),  
  );
 
  // Site Description group
  // Need to reset default weights
  // New weight counter for inside field group
  $weight_in_group = 0;

  $instances['field_dirt_site_mean_annual_temp'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_annual_precip'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_elevation'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_slope'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_aspect'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_geo_coords'] = array(
    'display' => array(
      $print => array(
        'label' => 'above',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_group,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_map_unit_name'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_map_unit_symbol'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_landscape_posit'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_habitat_type'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_type_of_crop'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_cropping_system'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_topography'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_present_manage'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_past_manage'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_group,
      ),
    ),
  );
  $weight_in_group += 1;

  $instances['field_dirt_site_manage_updates'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_group,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => TRUE,
        ),
      ),
    ),
  );
  $weight_in_group += 1;

  // Done with site description group
  // Set next fields below site description group weight
  $weight_in_display = variable_get('dirt_group_dirt_site_desc_con_weight', 0) + 1;

  $instances['field_dirt_site_layer_data'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_modirt_site_agg_stab'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_site_drawing'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'image',
        'weight' => $weight_in_display,
        'settings' => array(
          'image_style' => 'medium',
          'image_link' => '',
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_site_drawing_scale'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_site_habitat_photos'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_site_habitat_photo'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'image',
        'weight' => 0,
        'settings' => array(
          'image_style' => 'medium',
          'image_link' => '',
        ),
      ),
    ),
  );

  $instances['field_dirt_site_desc_notes'] = array(
    'display' => array(
      $print => array(
        'label' => 'above',
        'weight' => $weight_in_display,
        'type' => 'text_default',
      ),
    ),
  );
  $weight_in_display += 1;


  /*----------------------------*/
  /* Monthly Data Survey Fields */
  /*----------------------------*/
  $bundle = $bundles[1];

  // Weight counter
  $weight_in_display = 0;

  $instances['field_dirt_temp_cloud_cover'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_avg_soil_temp_5cm'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'settings' => array(
          'scale' => 2,
        ),
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_cloud_cover'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_temp_cloud_notes'] = array(
    'display' => array(
      $print => array(
        'label' => 'above',
        'weight' => $weight_in_display,
        'type' => 'text_default',
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_color_number_by_hour'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_co2c_room_temp_range'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_co2c_field_temp_range'] = array(
    'display' => array(
      $print => array(
        'label' => 'inline',
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  // Hide these fields
  $instances['field_dirt_co2c_room_temp_lower'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 15,
      ),
    ),
  );

  $instances['field_dirt_co2c_room_temp_upper'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 16,
      ),
    ),
  );

  $instances['field_dirt_co2c_room_temp_avg'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 17,
      ),
    ),
  );

  $instances['field_dirt_co2c_field_temp_lower'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 18,
      ),
    ),
  );

  $instances['field_dirt_co2c_field_temp_upper'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 19,
      ),
    ),
  );

  $instances['field_dirt_co2c_field_temp_avg'] = array(
    'display' => array(
      $print => array(
        'type' => 'hidden',
        'weight' => 20,
      ),
    ),
  );

  $instances['field_dirt_soil_resp_notes'] = array(
    'display' => array(
      $print => array(
        'label' => 'above',
        'weight' => $weight_in_display,
        'type' => 'text_default',
      ),
    ),
  );
  $weight_in_display += 1;

  // Push these down to the bottom of the display
  $weight_in_display = 50;

  $instances['field_dirt_sample_before_drying'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_sample_after_drying'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_container_weight_note'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'weight' => $weight_in_display,
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_bulk_density_calcs'] = array(
    'display' => array(
      $print => array(
        'label' => 'hidden',
        'type' => 'field_collection_table_view',
        'weight' => $weight_in_display,
        'settings' => array(
          'edit' => '',
          'delete' => '',
          'add' => '',
          'hide_empty' => TRUE,
          'empty' => FALSE,
        ),
      ),
    ),
  );
  $weight_in_display += 1;

  $instances['field_dirt_bulk_density_notes'] = array(
    'display' => array(
      $print => array(
        'label' => 'above',
        'weight' => $weight_in_display,
        'type' => 'text_default',
      ),
    ),
  );
  $weight_in_display += 1;


  /*---------------------------------*/
  /* Twice a Year Data Survey Fields */
  /*---------------------------------*/
  $bundle = $bundles[2];

  // Weight counter
  $weight_in_display = 0;

  // Hide these fields
  $fields_to_hide = array(
    'field_dirt_soil_n_value',
    'field_dirt_soil_n_ppm',
    'field_dirt_soil_n_category',
    'field_dirt_soil_p_value',
    'field_dirt_soil_p_ppm',
    'field_dirt_soil_p_category',
    'field_dirt_soil_k_value',
    'field_dirt_soil_k_ppm',
    'field_dirt_soil_k_category',
    'field_dirt_soil_ph_value',
    'field_dirt_soil_ph_category',
    'field_dirt_soil_ac_range',
    'field_dirt_soil_quality',
  );

  foreach($fields_to_hide as $field) {
    $instances[$field] = array(
      'display' => array(
        $print => array(
          'type' => 'hidden',
          'weight' => $weight_in_display,
        ),
      ),
    );
    $weight_in_display += 1;
  }


  /*-------------*/
  /* All Surveys */
  /*-------------*/

  // For all bundles, hide form header, form submission note, and admin-only
  // fields.
  //foreach($bundles as $bundle) {
    $instances['field_dirt_form_header'] = array(
      'display' => array(
         $print => array(
          'type' => 'hidden',
          'module' => 'markup',
          'weight' => -3,
        ),
      ),
    );

    $instances['field_dirt_form_submission_note'] = array(
      'display' => array(
        $print => array(
          'type' => 'hidden',
          'weight' => -2,
        ),
      ),
    );

    $instances['field_dirt_datatable_in_portal'] = array(
      'display' => array(
        $print => array(
          'type' => 'hidden',
          'weight' => 55,
        ),
      ),
    );

    $instances['field_dirt_datatable_admin_notes'] = array(
      'display' => array(
        $print => array(
          'type' => 'hidden',
          'weight' => 56,
        ),
      ),
    );

   /*
    $instances['field_dirt_date_of_survey'] = array(
      'display' => array(
        $print => array(
          'label' => 'inline',
          'weight' => -1,
          'type' => 'date_default',
          'format_type' => DIRT_SURVEY_DATE_TYPE,
          'settings' => array(
             'format_type' => DIRT_SURVEY_DATE_TYPE,
          ),
        ),
      ),
    );
   */
  //}

  return $instances;
}

