/**
 * @file
 * Add the option to read multiple file srcs for images of different sizes.
 */

/* global Imager */
;(function (window, document) {
  'use strict';

  // Prefix to look for in dataset.
  var dataSrcPrefix = 'src-';

  var getIndividualSrc = function (attr, imageParent) {
    var value = imageParent.dataset[attr];

    // Check whether attribute starts 'src-'.
    if (attr.indexOf(dataSrcPrefix) !== 0) {
      return;
    }

    // Get just the width segment of the data attribute's key.
    var srcWidth = attr.replace(dataSrcPrefix, '');

    // Return here if we can't get an integer from srcWidth.
    if (typeof parseInt(srcWidth) !== 'number') {
      return;
    }

    // Convert srcWidth to integer.
    srcWidth = parseInt(srcWidth);

    return {
      srcWidth: srcWidth,
      value: value
    };
  };

  // This is where we need to run the logic to determine the best source.
  var useMultipleSrcs = function (image) {

    // If image width is 0 then return the gif src as we don't want to load up
    // and image.
    if (image.clientWidth === 0) {
      return this.gif.currentSrc;
    }

    // Get the current instance.
    var imagerInstance = this;

    // Get the parent node, this is the element that the image srcs will be
    // saved to.
    var imageParent = image.parentNode;

    // Array to save available srcs to.
    var srcs = [];

    // Array to save available widths to.
    var widths = [];

    // Placeholder var to save data to temporarily for each loop.
    var src;

    // We need to cycle through the dataset on the imageParent to find
    // src-[width] attributes.
    for (var attr in imageParent.dataset) {
      if (!imageParent.dataset.hasOwnProperty(attr)) {
        continue;
      }

      // Get the src data.
      src = getIndividualSrc(attr, imageParent);

      if (!src) {
        continue;
      }

      // Push srcWidth to widths array.
      widths.push(src.srcWidth);

      // Save image src to srcs, keyed by width.
      srcs[src.srcWidth] = src.value;
    }

    // If we don't have any widths then return here.
    if (!widths.length) {
      return;
    }

    // Sort widths array.
    widths.sort(function (a, b) {
      return a - b;
    });

    // Min width.
    var minWidth = image.clientWidth * imagerInstance.devicePixelRatio;

    // Get best width.
    var bestWidth = Imager.getClosestValue(minWidth, widths);

    // Get image src for best width.
    var bestSrc = srcs[bestWidth];

    return bestSrc;
  };

  // Test for Imager function.
  if (typeof window.Imager !== 'function') {
    return;
  }

  window.Imager.prototype.multipleSrcs = useMultipleSrcs;

})(window, document);
