(function ($) {
  var H_LOADED = 0; // Store whether H was loaded from previous survey (default not loaded)

  /**
   * Updates monthly survey form calculations given that H and K fields are
   * automatically loaded from month to month instead of newly calculated each
   * time.
   */
  Drupal.behaviors.modirtMonthlySurvey = {
    attach: function (context, settings) {
      var H;
      var K;

      // Determine whether H has been auto-loaded from previous surveys
      H = $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-soil-bulk-density-und-0-value').val();
      if (H) {
        H_LOADED = 1;
        // Also calculate K right away, which only depends on H
        K = (1 - H/2.65) * 100;
        K = Math.round(K * 100) / 100;
        $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-total-soil-porosity-und-0-value').val(K);
      }
    }
  };

  /**
   * Override setH() from DIRT module to not recalculate H if it's already
   * loaded from a previous survey.
   */
  Drupal.dirtMonthlySurvey.setH = function () {
    // If H is already filled in, don't recalculate it.
    if (H_LOADED != 1) {
      var F = $('#edit-field-dirt-sample-after-drying-und-0-field-dirt-weight-dry-soil-und-0-value').val();
      var G = $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-volume-soil-sample-und-0-value').val();
      var H;

      if (F && G) {
        H = F / G;
        H = Math.round(H * 100) / 100;
        $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-soil-bulk-density-und-0-value').val(H);
      }
      else {
        // One of F or G missing, so clear H:
        $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-soil-bulk-density-und-0-value').val('');
      }
    }
  };

  /**
   * Override setK() from DIRT module to not recalculate H if it's already
   * loaded from a previous survey.
   */
  Drupal.dirtMonthlySurvey.setK = function () {
    // If H is already filled in, don't recalculate K which only depends on H.
    if (H_LOADED != 1) {
      var H = $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-soil-bulk-density-und-0-value').val();
      var K;

      if (H) {
        K = (1 - H/2.65) * 100;
        K = Math.round(K * 100) / 100;
        $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-total-soil-porosity-und-0-value').val(K);
      }
      else {
        // H missing, so clear K:
        $('#edit-field-dirt-bulk-density-calcs-und-0-field-dirt-total-soil-porosity-und-0-value').val('');
      }
    }
  };

})(jQuery);
