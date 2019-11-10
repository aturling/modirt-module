(function ($) {
  /**
   * Add Missouri county lines to Data Search Portal map.
   *
   * Note that this will only work if Google may access the site (as in, it's
   * not behind a firewall.)
   */
  Drupal.behaviors.modirtMapCounties = {
    attach: function (context, settings) {
      // Get map
      var map = Drupal.behaviors.dirtDatatable.getMap();

      if (map) {
        // Counties KML file location
        // TODO: change this!
        var urlName = "http://modirt.missouriepscor.org/sites/all/modules/custom/modirt_datatables/data/missouri_counties.kml" + "?recent";
        //var urlName = "http://modirt.missouriepscor.org/" + Drupal.settings.modirt.modirtURL + "/files/missouri_counties.kml" + "?recent";

        var layer = new google.maps.KmlLayer({
          url: urlName,
          preserveViewport: true
        });

        layer.setMap(map);
      }
    }
  };

})(jQuery);
