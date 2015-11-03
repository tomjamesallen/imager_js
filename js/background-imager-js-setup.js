/**
 * @file
 * Handles loading of images and exposes external API.
 */

/* global BackgroundImager */
;(function ($, window) {
  'use strict';

  // Setup event triggering for image replace and then image load for each
  // image.
  var forEachImageReplaced = function ($image, bgImager) {
    // Trigger onImageReplaced event.
    $(document).trigger({
      type: 'drupalBgImagerJs.onImageReplaced',
      $image: $image,
      bgImager: bgImager
    });

    // Trigger onImageLoaded event.
    $(document).trigger({
      type: 'drupalBgImagerJs.onImageLoaded',
      $image: $image,
      bgImager: bgImager
    });
  };

  // Create instance of BackgroundImager.
  Drupal.bgImagerJs = BackgroundImager('.drupal-bg-imager-js', {
    imageReadyClass: 'image-loaded',

    // Set up callbacks when images are replaced.
    onImageLoadedOnce: function ($image) {
      forEachImageReplaced($image, this);
    }
  });

  // Create behavior to scan for new images.
  Drupal.behaviors.bgImagerJs = {
    attach: function (context, settings) {
      // Call BackgroundImager's .add() method to scan for new images.
      Drupal.bgImagerJs.add();
      // Call BackgroundImager's .update() method to check sizing for existing
      // images.
      Drupal.bgImagerJs.update();
    }
  };

})(jQuery, window);
