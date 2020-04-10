(function ($) {

  Drupal.behaviors.scroll_blocks = {
    attach: function (context) {

      /** @namespace Drupal.settings.scroll_blocks */
      if (Drupal.settings.scroll_blocks === undefined) {
        return;
      }

      var scroll_blocks = Drupal.settings.scroll_blocks;

      var $window = $(window);

      for (var module in scroll_blocks) {
        for (var delta in scroll_blocks[module]) {

          // @see https://stackoverflow.com/a/31162232
          (function () {

            var block_id = 'block-' + module + '-' + delta;

            var $block = $('#' + block_id);

            var minHeight = parseInt(scroll_blocks[module][delta].scroll_blocks_min_scroll_distance);
            var maxHeight = parseInt(scroll_blocks[module][delta].scroll_blocks_max_scroll_distance);
            var minWidth = parseInt(scroll_blocks[module][delta].scroll_blocks_min_width);
            var maxWidth = parseInt(scroll_blocks[module][delta].scroll_blocks_max_width);

            var windowWidth = $window.width();

            $window.scroll(function () {
              var yPosition = window.pageYOffset;
              var heightOK = yPosition >= minHeight && yPosition <= maxHeight;

              var minWidthOK = (!isNaN(minWidth) && minWidth !== 0 ) ? (windowWidth >= minWidth) : true;
              var maxWidthOK = (!isNaN(maxWidth) && maxWidth !== 0 ) ? (windowWidth <= maxWidth) : true;

              if (heightOK && minWidthOK && maxWidthOK) {
                // Show block
                showBlock($block);
              }
              else {
                // Hide block
                hideBlock($block);
              }
            });
          })();

        }
      }

      function showBlock($block) {
        if (!$block.data('scroll-blocks--visible')) {

          var mutedByUser = $block.data('scroll-blocks--disable');
          if (mutedByUser === true) {
            return;
          }

          $block.addClass('scroll-blocks--visible');
          if (!$block.find('.close-button').length) {
            var $closeButton = $('<div class="close-button"/>')
                .click(function () {
                  $block.data('scroll-blocks--disable', true);
                  $block.removeClass('scroll-blocks--visible');
                  $block.data('scroll-blocks--visible', false);
                });
            $block.append($closeButton);
          }
          $block.data('scroll-blocks--visible', true);
          console.log('Reveal block');
        }

      }

      function hideBlock($block) {
        if ($block.data('scroll-blocks--visible')) {
          $block.removeClass('scroll-blocks--visible');
          $block.data('scroll-blocks--visible', false);
          console.log('Hide block');
        }

      }
    }
  };

})(jQuery);
