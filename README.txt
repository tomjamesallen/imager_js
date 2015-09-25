README.txt
==========
This is a Drupal 7 implementation of the BBC news team's Imager.js.
https://github.com/BBC-News/Imager.js/

> Imager.js is an alternative solution to the issue of how to handle responsive 
> image loading, created by developers at BBC News.


Installation
------------
Enable the imager_js module, along with its dependencies.

This project uses Drupal's libraries module to handle loading the Imager.js
project. You will need to download the full Imager.js repository from github
and place it in your libraries directory. `sites/all/libraries` or 
`sites/<domain>/libraries`. This directory is automatically created by the 
libraries module.

The outer directory containing the Imager.js project should be named
`Imager.js`, if you download the zip from the project page you may have to
rename the directory from `Imager.js-master` or similar. You should end up with
the file `Imager.min.js` located at the path 
`[libraries-path]/Imager.js/dist/Imager.min.js`. If the minified file is not
found, the module will also attempt to use the non-minified file located at 
`[libraries-path]/Imager.js/Imager.js`.

Once the Imager.js project is in place you should be ready to use the imager_js
field formatter.


Setup
-----
Should work anywhere where you might use a field formatter. Tested in views and
for displaying fields on nodes.


Options
-------
### Imager styles
Select which image styles are available to the formatter. Imager.js will then
select the most appropriate image for its current context.

### Background image fallback
Renders a version of the image as a background image on the wrapper. This can be
used as a fallback for devices that don't have js enabled, or can be used as a 
placeholder that will show before the full res image loads. Select an image 
style to use, or select none to disable.

### Noscript fallback image
Renders a fallback image inside noscript tags. Select an image style to use, or 
select none to disable.

### Wrapper element
Whether to wrap the image in a span or a div. This makes no visual difference,
but if the image is to be rendered inside an inline level element such as an 
anchor, then a span should be used rather than a div.

### Inline script
Render script tags inline to trigger Imager's .add() method. Rendering script 
inline will mean that the .add() method is called as early as possible. If 
disabled the .add() method will be triggered on Drupal's behaviors attach. 
Should be disabled if scripts are moved to footer. Please note, inline script 
tags are stripped from the output of views' ajax mode.

### Intrinsic ratio
Sets image container to correct aspect ratio using the intrinsic ratio method. 
This will add an inline style to the wrapper element with a padding-bottom
value.

### Image size multiplier
Base multiplier to increase the size of the images. This can be used to 
compensate for lower quality images or higher image compression rates. 
Technically this lowers size of the breakpoint at which the images are switched,
so you may wish to include larger image styles in the options.


JS API
------
Please see https://github.com/BBC-News/Imager.js/blob/master/docs/js-api.md for
JS API options. 

The imager.js instance set up by Drupal can be accessed from the
`Drupal.imagerJs` namespace.

In addition to the API options offered by imager.js, this module set up custom
jQuery events for when images are replaced, or have loaded. It will also an 
'image-loaded' class to the wrapper element, which can be used to add CSS
animations, such as fade-ins when an image loads.

    ;(function ($, window) {

      $(document).ready(function () {
        $(document).bind('drupalImagerJs.onImageReplaced', function (event) {
          console.log(event);
        });

        $(document).bind('drupalImagerJs.onImageLoaded', function (event) {
          console.log(event);
        });
      });

    })(jQuery, window);

Css example.
    /* Fade in example css - copy css into theme */
    .drupal-imager-js-wrapper img {
      opacity: 0;
      transition: all 300ms ease;
    }
    .drupal-imager-js-wrapper.image-loaded img {
      opacity: 1;
    }
