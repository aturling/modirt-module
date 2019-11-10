<?php
/*
* @file
* This file specifies the definitions for custom survey node types installed by this module.
*/


/**
 * Returns list of machine names of custom survey types installed
 * in this module.
 *
 * @retval Array of survey type machine names.
 */
function modirt_get_installed_survey_types() {
  return array_keys(modirt_get_installed_survey_type_info());
}


/**
 * Returns array of survey types installed in this module, keyed by survey type
 * machine name, with values being an array of survey info with keys:
 * - name: Type machine name,
 * - description: Type description (same as in node_info()),
 * - collection_frequency: Number of times per year survey is to be
 *   collected, or DIRT_ONE_TIME_COLLECTION if only collected one time.
 * - collection_months: (optional) Specifies the months of collection. For
 *   instance, monthly surveys are expected to be collected every month except
 *   December and January (too cold), and twice a year surveys are expected to
 *   be collected in May and August. If collection_months not specified,
 *   surveys will be collected at evenly spaced intervals throughout the year
 *   according to their collection frequency.
 * 
 * @sa dirt_get_installed_survey_type_info()
 *
 * @retval Array with keys as survey type machine names and
 *  values as array of survey type information.
 */
function modirt_get_installed_survey_type_info() {
  return array(
   'modirt_lab_results_survey' => array(
      'name' => t('Soil Lab Results'),
      'description' => t("The <em>soil lab results</em> from Mizzou/Veum's lab."),
      'collection_frequency' => 2,
      'collection_months' => array('5', '8'),
    ),
  );
}
