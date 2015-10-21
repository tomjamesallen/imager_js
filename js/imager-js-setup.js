/**
 * @file
 * Handles loading of images and exposes external API.
 */

/* global Imager */
;(function ($, window) {
  'use strict';

  // Setup event triggering for image replace and then image load for each
  // image.
  var forEachImageReplaced = function (image, imager) {
    // Trigger onImageReplaced event.
    $(document).trigger({
      type: 'drupalImagerJs.onImageReplaced',
      image: image,
      imager: imager
    });

    // Bind to image .load.
    $(image).load(function () {
      // Add image-loaded class to parent for styling use.
      $(image).parent().addClass('image-loaded');

      // Trigger onImageLoaded event.
      $(document).trigger({
        type: 'drupalImagerJs.onImageLoaded',
        image: image,
        imager: imager
      });
    });
  };

  // Create instance of Imager.
  Drupal.imagerJs = new Imager('.drupal-imager-js', {
    // Enable multipleSrcs.
    availableWidths: function (image) {
      return this.multipleSrcs(image);
    },
    // Set up callbacks when images are replaced.
    onImagesReplaced: function (images) {
      // Bind individual events for each image.
      for (var id in images) {
        if (images.hasOwnProperty(id)) {
          forEachImageReplaced(images[id], this);
        }
      }
    }
  });

  // Create behavior to scan for new images.
  Drupal.behaviors.imagerJs = {
    attach: function (context, settings) {
      // Call Imager's .add() method to scan for new images.
      Drupal.imagerJs.add();
    }
  };

})(jQuery, window);
