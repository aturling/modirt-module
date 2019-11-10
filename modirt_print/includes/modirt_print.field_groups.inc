<?php
/*
* @file
* This file specifies the definitions for custom field groups created by this module.
*/

/**
 * Returns array of field groups where each array item is
 * an array with keys:
 *
 * 'basename' : group basename (without mode)
 * 'bundle' : bundle group is attached to
 * 'mode' : print view mode.
 *
 * using definitions from
 * dirt_get_field_group_basenames().
 *
 * This function effectively enumerates all field groups
 * installed in this module.
 *
 * @retval Array of arrays with keys 'basename', 'bundle', and 'mode'.
 */
function modirt_print_get_field_group_info() {

  module_load_include('inc', 'dirt', 'includes/dirt.field_groups');
  module_load_include('inc', 'modirt', 'includes/modirt.field_groups');

  // Add field groups to print view mode:
  $modes = array(PRINT_VIEW_MODE);

  // Get field groups from DIRT module
  $dirt_field_groups = dirt_get_field_group_basenames($modes);

  // Get field groups from MO DIRT module
  $modirt_field_groups = modirt_get_field_group_basenames($modes);

  // Combine groups
  $field_groups = array_merge($dirt_field_groups, $modirt_field_groups);

  return dirt_get_field_group_info_from_group_basenames($field_groups);
}

