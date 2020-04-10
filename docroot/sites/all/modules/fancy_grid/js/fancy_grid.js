/**
 * @file
 * Fancy Grid javascript helpers.
 *
 */
(function ($) {
  'use strict';

  Drupal.behaviors.fancy_grid = {
    attach: function (context, settings) {
      $('body').once('fancy_grid-debug', function() {
        if ($(".grid-wrapper").length) {
          var $displayGrid = $('<input/>')
            .addClass('display-grid form-submit')
            .attr('type', 'button')
            .attr('value', Drupal.t('Display grid'))
            .css('position', 'fixed')
            .css('top', '.25rem')
            .css('right', '1em')
            .css('z-index', '9999')
            .bind('click', function () {
              $('body').toggleClass('display-grid');

              if ($('body').hasClass('display-grid')) {
                $(this).attr('value', Drupal.t('Hide grid'));
              } else {
                $(this).attr('value', Drupal.t('Display grid'));
              }
            });

          $('body', context).append($displayGrid);
        }
      });
    }
  };

})(jQuery);
