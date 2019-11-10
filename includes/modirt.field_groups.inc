<?php
/*
* @file
* This file specifies the definitions for custom field groups created by this module.
*/

/**
 * Returns associative array of field group basenames => bundle names.
 * Bundle name "ALL_SURVEYS" indicates that this group should be
 * attached to all survey content types.
 *
 * @param[in] $modes *Optional* View modes (defaults to form and default).
 */
function modirt_get_field_group_basenames($modes = array('form', 'default')) {
  // Note: group basenames need to stay <= 24 characters so that
  // group name = basename + mode <= 32 characters.

  return array(
    'group_modirt_lab_results' => array(
      'bundles' => array('modirt_lab_results_survey'),
      'modes' => $modes,
    ),
  );
}


/**
 * Returns array of field groups where each array item is
 * an array with keys:
 *
 * 'basename' : group basename (without mode)
 * 'bundle' : bundle group is attached to
 * 'mode' : mode (form vs. default node view),
 *
 * using definitions from
 * dirt_get_field_group_basenames().
 *
 * This function effectively enumerates all field groups
 * installed in this module.
 *
 * @retval Array of arrays with keys 'basename', 'bundle', and 'mode'.
 */
function modirt_get_field_group_info() {
  module_load_include('inc', 'dirt', 'includes/dirt.field_groups');
  module_load_include('inc', 'modirt', 'includes/modirt.node_types');

  $field_groups = modirt_get_field_group_basenames();

  return dirt_get_field_group_info_from_group_basenames($field_groups);
}


/**
 * Returns group object for $group_basename and $mode
 *
 * Input:
 * $group_basename String: field group machine name minus '_$mode' suffix
 * $bundle String: bundle (e.g., node type) field group attached to
 * $mode String:
 *  'form' : return the field group for the node form
 *  'default' : return the field group for the node display
 */
function modirt_get_field_group_object($group_basename, $bundle, $mode) {
  $field_group = new stdClass();

  // Set group name according to basename
  $group_name = $group_basename . '_' . $mode;

  // Initialize required variables
  // The following MUST be set in switch() below for each group
  $label = '';
  $children = array();

  // Default options for all field groups
  // These may be overwritten for some groups in switch() below
  $entity_type = 'node';
  $parent_name = '';
  $format_type = 'fieldset';
  $formatter = 'collapsible';
  $description = '';
  $classes = '';

  // Attempt to get field group weight from stored variable;
  // default to 0 if not set
  $weight = variable_get('dirt_' . $group_basename . '_weight', 0);

  switch($group_basename) {
    case 'group_modirt_lab_results':
      $label = t('Lab Characterization');
      $children = array(
        0 => 'field_modirt_lab_collection_date',
        1 => 'field_modirt_lab_ph',
        2 => 'field_modirt_lab_nitrate',
        3 => 'field_modirt_lab_phosphorus',
        4 => 'field_modirt_lab_phosphorus_ppm',
        5 => 'field_modirt_lab_potassium',
        6 => 'field_modirt_lab_potassium_ppm',
        7 => 'field_modirt_lab_calcium',
        8 => 'field_modirt_lab_calcium_ppm',
        9 => 'field_modirt_lab_magnesium',
       10 => 'field_modirt_lab_magnesium_ppm',
       11 => 'field_modirt_lab_organic_matter',
       12 => 'field_modirt_lab_cec',
       13 => 'field_modirt_lab_na',
       14 => 'field_modirt_lab_active_carbon',
       15 => 'field_modirt_lab_wsa',
      );
      $desc_txt = t("Results of soil testing by the Soil Testing Lab (Mizzou " .
        "Lab) and Dr. Kristen Veum's Lab (Veum's Lab), both from the " . 
        "University of Missouri-Columbia.") . '<br>' .
        t("The following fields are visible to site admins only.");
      $description = ($mode == 'form') ? $desc_txt : '';
      break;
  }

  // Set identifier based on group name and entity_type
  // group_name|entity_type|bundle|mode
  $identifier = $group_name . '|' . $entity_type . '|' . $bundle . '|' . $mode;

  // Now create the field group:

  $field_group->disabled = FALSE;
  $field_group->api_version = 1;
  $field_group->identifier = $identifier;
  $field_group->group_name = $group_name;
  $field_group->entity_type = $entity_type;
  $field_group->bundle = $bundle;
  $field_group->mode = $mode;
  $field_group->parent_name = $parent_name;
  $field_group->data = array(
    'label' => $label,
    'weight' => $weight,
    'children' => $children,
    'format_type' => $format_type,
    'format_settings' => array(
      'formatter' => $formatter,
      'instance_settings' => array(
        'description' => $description,
        'classes' => $classes,
        'required_fields' => 1,
      ),
    ),
  );

  return $field_group;
}
