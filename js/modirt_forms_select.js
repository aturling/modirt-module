(function ($) {
  Drupal.behaviors.modirtSurveyFormChanges  = {
    /**
     * Changes the site description survey form to make some fields updatable.
     */
    attach: function (context, settings) {
      // Remove "other" in county select
      $('.field-name-field-dirt-site-county select option[value="select_or_other"]').attr('disabled','disabled');
      $('.field-name-field-dirt-site-county select option[value="select_or_other"]').hide();

      // Denote updatable
      var denotesUpdatable = " ***";
      $('.field-name-field-dirt-site-habitat-type label').append(denotesUpdatable);
      $('.field-name-field-dirt-site-type-of-crop label').append(denotesUpdatable);
      $('.field-name-field-dirt-site-cropping-system label').append(denotesUpdatable);
      $('.field-name-field-dirt-site-topography label').append(denotesUpdatable);
      $('.field-name-field-dirt-site-present-manage label:not(.option)').append(denotesUpdatable);
      $('.field-name-field-dirt-site-past-manage label:not(.option)').append(denotesUpdatable);
      //$('.field-name-field-dirt-site-manage-updates label.form-item-title').append(denotesUpdatable);

      if (!Drupal.settings.modirt.userIsAdmin) {
        // Lock already checked past and present management boxes, but allow for checking additional boxes:
        var checkedPresentLandManagementBoxes = $('.field-name-field-dirt-site-present-manage input[type=checkbox]:checked').map(function(){
          return $(this).val();
        }).get();

        var checkedPastLandManagementBoxes = $('.field-name-field-dirt-site-past-manage input[type=checkbox]:checked').map(function(){
          return $(this).val();
        }).get();

        // if select_or_other checked, make the "other" field readonly and wrap in a div
        if (jQuery.inArray("select_or_other", checkedPresentLandManagementBoxes) != -1) {
          $('.field-name-field-dirt-site-present-manage input.select-or-other-other').attr("readonly", true);
          $('.field-name-field-dirt-site-present-manage input.select-or-other-other').addClass('lock-field');
          $('.form-item-field-dirt-site-present-manage-und-select-select-or-other').wrap('<div class="hide-other"></div>');
          var otherText = '<div class="form-item"><label>Other:</label></div>' + $('.field-name-field-dirt-site-present-manage input.select-or-other-other').val();
          $('.field-name-field-dirt-site-present-manage').append(otherText);
          $('.hide-other').hide();
          $('.field-name-field-dirt-site-present-manage input.select-or-other-other').hide();
          $('.field-name-field-dirt-site-present-manage .description').hide();
        }
        if (jQuery.inArray("select_or_other", checkedPastLandManagementBoxes) != -1) {
          $('.field-name-field-dirt-site-past-manage input.select-or-other-other').attr("readonly", true);
          $('.field-name-field-dirt-site-past-manage input.select-or-other-other').addClass('lock-field');
          $('.form-item-field-dirt-site-past-manage-und-select-select-or-other').wrap('<div class="hide-other"></div>');
          var otherText = '<div class="form-item"><label>Other:</label></div>' + $('.field-name-field-dirt-site-past-manage input.select-or-other-other').val();
          $('.field-name-field-dirt-site-past-manage').append(otherText);
          $('.hide-other').hide();
          $('.field-name-field-dirt-site-past-manage input.select-or-other-other').hide();
          $('.field-name-field-dirt-site-past-manage .description').hide();
        }

        $('.field-name-field-dirt-site-present-manage input[type=checkbox]').once('prevent-uncheck-present-manage', function() {
          $(this).click(function (e) {
            var checkboxVal = $(this).attr("value");
            if (!this.checked && (jQuery.inArray(checkboxVal, checkedPresentLandManagementBoxes) != -1)) {
              e.preventDefault();
              return false;
            }
          });
        });

        $('.field-name-field-dirt-site-past-manage input[type=checkbox]').once('prevent-uncheck-past-manage', function() {
          $(this).click(function (e) {
            var checkboxVal = $(this).attr("value");
            if (!this.checked && (jQuery.inArray(checkboxVal, checkedPastLandManagementBoxes) != -1)) {
              e.preventDefault();
              return false;
            }
          });
        });
      }
    }
  };

})(jQuery);
