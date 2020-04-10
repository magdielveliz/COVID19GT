/**
 * @file
 * Thumbnail hover preview script by M.Kleine-Albers / www.artwaves.de.
 *
 * Loads bigger preview image if a user hovers over a thumbnail. Checks if
 * an image would be loaded outside of the viewport TOP, BOTTOM, LEFT, RIGHT.
 * With low resolutions like 1024 it's not perfect using big preview images.
 */

(function ($) {
  Drupal.behaviors.HoverPreview = {
    attach: function () {

      // CONFIG GLOBAL vars.
      // Determine popup's additional distance from the cursor.
      yOffset = 30;
      xOffset = 10;

      // HOVER.
      $("img.hover-preview").hoverIntent(showPreview, hidePreview);

      function showPreview() {

        // Get title attribute.
        this.t = this.title;
        this.title = "";
        var image_title = (this.t != "") ? "<br/>" + this.t : "";

        // Get position of hovered element.
        var element = $(this);
        var elementPositions = element.offset();
        var thumbWidth = element.outerWidth(true);
        var thumbHeight = element.outerHeight(true);

        // GLOBAL VARIABLES.
        // Get the src of the bigger preview images.
        preview_link = $(this).attr('data-hover-preview');
        // Get the src of the ZOOM image.
        zoom_link = $(this).attr('data-hover-zoom');

        // Preload the zoom image.
        preloaded_zoom = $('<img/>').attr('src', zoom_link);

        // Output of the preview element with image.
        $("body").append(
          '<div id="image-preview-active"><img src="' + preview_link + '" alt="Loading ..." />' +
            '<span id="zoom-text">' + Drupal.t('Press and hold Z key to zoom') + '</span>' +
            '<span id="title-text">' + image_title + '</span>' +
           '</div>'
        );

        // CHECK IF ELEMENT WOULD BE LOADED OUT OF THE VIEWPORT.
        // Width & height of the popup with the larger image triggered on hover.
        var elementHeight = $("#image-preview-active").outerHeight(true);
        var elementWidth = $("#image-preview-active").outerWidth(true);

        // Set initial and change in "if" later if necessary. The image is loaded there x/y.
        // Subtraction for centering the preview to thumb.
        var yPosition = elementPositions.top - (elementHeight / 2) + (thumbHeight / 2);
        // Add with to load it next to hovered thumb.
        var xPosition = elementPositions.left + thumbWidth + xOffset;

        // Size of the viewport, meaning the area of the page, that is completely visible.
        var winHeight = $(window).height();
        var winWidth = $(window).width();

        // If the page has been scrolled, additional calculation must be done.
        // Scroll position is the same as the number of pixels that are hidden from view above the scrollable area.
        // If the scroll bar is at the very top, or if the element is not scrollable, this number will be 0.
        var scrolledPixelTop = $(window).scrollTop();
        // A site should not be vertically scrollable, but maybe.
        var scrolledPixelLeft = $(window).scrollLeft();

        // Calculate the space that the image needs to be displayed next to the thumb image.
        var calcHeightTop = yPosition;
        var calcHeightBottom = yPosition + elementHeight - scrolledPixelTop;
        var calcHeightLeft = xPosition - elementWidth;
        var calcWidthRight = xPosition + thumbWidth + elementWidth + scrolledPixelLeft;

        // Height(y) if it would be loaded out of view TOP.
        if (calcHeightTop < scrolledPixelTop) {
          yPosition = scrolledPixelTop;
        }

        // Height(y) if it would be loaded out of view BOTTOM.
        if (calcHeightBottom > winHeight) {
          yPosition = winHeight - elementHeight + scrolledPixelTop;
        }

        // Needed, because on small pixelwidth BOTH if would be called.
        var rightOut = false;

        // Width(x) if it would be loaded out of view RIGHT.
        if (calcWidthRight > winWidth) {
          xPosition = xPosition - elementWidth - thumbWidth - 2 * xOffset + scrolledPixelLeft;
          rightOut = true;
        }

        // Width(x) it would be loaded out of view LEFT.
        // Attempt not to load image to LEFT, but on low res, this is not possible to avoid.
        // NO Offset to decrease pixels needed.
        if (calcHeightLeft < scrolledPixelLeft && rightOut === true) {
          xPosition = xPosition + elementWidth + thumbWidth;
        }

        // Output and place preview image.
        $("#image-preview-active")
          .css("top", (yPosition) + "px")
          .css("left", (xPosition) + "px")
          .fadeIn('fast');
      }

      // Hide on mouse move out.
      function hidePreview() {
        this.title = this.t;
        $("#image-preview-active").remove();
      }

      // KEY PRESS TO ZOOM.
      $(window).keydown(function (e) {
        // 90 is z Key.
        if (e.which === 90) {
          $("#image-preview-active img").attr("src", zoom_link);
        }
      });
      $(window).keyup(function (e) {
        if (e.which === 90) {
          // Change image to zoomed image-> output it to the code via module, then code here.
          $("#image-preview-active img").attr("src", preview_link);
        }
      });

    }
  };
}(jQuery));
