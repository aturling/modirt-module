(function ($) {
  Drupal.behaviors.modirtSurveyUploadForm  = {
    attach: function (context, settings) {
      // Disable checkboxes if site ID incorrect or not found
      $('#modirt-summary-table .form-checkbox[value=""]').each(function(i, obj) {
        console.log(obj);
      });
    }
  };

})(jQuery);

