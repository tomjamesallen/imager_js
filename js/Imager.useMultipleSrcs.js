/**
 * Add the option to read multiple file srcs for images of different sizes.
 */
;(function (window, document) {

  var useMultipleSrcs = function (image) {
    // This is where we need to run the logic to determine the best source.

    // Get the current instance.
    var imagerInstance = this;

    // Get the parent node, this is the element that the image srcs will be
    // saved to.
    var imageParent = image.parentNode;

    // Array to save available srcs to.
    var srcs = [];

    // Array to save available widths to.
    var widths = [];

    // Prefix to look for in dataset.
    var dataSrcPrefix = 'src-';

    // We need to cycle through the dataset on the imageParent to find
    // src-[width] attributes.
    for (var attr in imageParent.dataset) {

      // Wrapper function.
      (function (attr, imageParent) {
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

        // Push srcWidth to widths array.
        widths.push(srcWidth);

        // Save image src to srcs, keyed by width.
        srcs[srcWidth] = value;

      })(attr, imageParent);
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
  if (typeof window.Imager !== 'function') { return; }

  window.Imager.prototype.multipleSrcs = useMultipleSrcs;

})(window, document);