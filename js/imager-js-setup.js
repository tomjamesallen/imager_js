/* global Imager */

/**
 * @file
 * Handles loading of images and exposes external API.
 */
;(function ($, window) {
  'use strict';

  // Handle the eventuality that Imager.js has not been correctly installed in
  // the libraries directory.
  if (typeof Imager !== 'function') {
    if (typeof console === 'object') {
      var errorMessage = "ERROR [imager_js]: Imager object not found. Please check that the Imager.js project has been downloaded from https://github.com/BBC-News/Imager.js/ and put in the libraries directory, with the directory name 'Imager.js'.";
      console.log(errorMessage);
    }

    // We need to add an empty function and save it to Drupal.imagerJs.add so
    // that when Drupal.imagerJs.add() is called from the markup output by the
    // imager_js field formatter it doesn't error.
    Drupal.imagerJs = {
      add: function () {}
    };

    // Return here to prevent any setup.
    return;
  }

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