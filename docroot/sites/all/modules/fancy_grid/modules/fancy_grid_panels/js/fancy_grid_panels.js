/**
 * @file
 * Fancy Grid panels javascript helpers to make the module work with IPE.
 *
 */
(function ($) {
  'use strict';

  if (Drupal.ajax !== undefined) {

    // This command initializes the fancy_grid for panels ipe wrappers
    Drupal.ajax.prototype.commands.initFancyGrid = function (ajax, response, status) {

      // Get ipe and the pane wrappers
      var $ipe = $('.panels-ipe-display-container');
      var $wrappers = $ipe.find('.panels-ipe-portlet-wrapper');

      // Add fancy_grid classes to all wrappers
      $wrappers.each(function () {
        var $wrapper = $(this);
        var $gridelement = $wrapper.find('.grid-element');

        // Add new classes
        $wrapper.addClass($gridelement.attr('class'));

        // Bind sortable events to change layout.
        $('div.panels-ipe-sort-container').each(function () {
          $(this).bind('sortstart', function (event, ui) {
            $(this).addClass('sorting');
          });
          $(this).bind('sortstop', function (event, ui) {
            $(this).removeClass('sorting');
          });
        });
      });
    };

    // This command initializes the fancy_grid for panels ipe wrappers
    Drupal.ajax.prototype.commands.updateFancyGridElement = function (ajax, response, status) {
      var $pane = $(response.selector);

      // Remove old classes
      $pane.removeClassFuzzy('grid-');

      // Add new classes
      $pane.addClass(response.data.cls);
    };

  }

  // Remove all classes from element who contain string
  $.fn.removeClassFuzzy = function (string) {
    var classes = $(this).attr('class').split(' ').filter(function (item) {
      return item.lastIndexOf(string, 0) !== 0;
    });
    $(this).attr('class', classes.join(' '));
  };


})(jQuery);
