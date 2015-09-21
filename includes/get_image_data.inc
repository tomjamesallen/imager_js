<?php

/**
 * Function to gather all of the image data required to render the image.
 *
 * Returns an array of data.
 */
function _imager_js_get_image_data($item, $display) {
  // Create empty image data array.
  $image_data = array();
  
  // Save image settings/defaults to image data.
  $image_data['settings'] = _imager_js_process_settings($display);

  // Save the item data to the image data array.
  $image_data['item'] = $item;

  // Get the image srcs.
  $image_data['srcs'] = _imager_js_return_image_srcs($image_data);

  // Get the background_image_fallback src.
  $image_data['background_image_fallback_src'] = _imager_js_return_background_image_fallback_src($image_data);

  // Get the background_image_fallback src.
  $image_data['noscript_fallback_src'] = _imager_js_return_noscript_fallback_src($image_data);

  // Create attributes array.
  $image_data['attributes'] = array();

  // Create the attributes array for the wrapper element.
  $image_data['attributes']['wrapper'] = _imager_js_create_wrapper_attributes($image_data);

  // Create the attributes array for the place holder element.
  $image_data['attributes']['placeholder'] = _imager_js_create_placeholder_attributes($image_data);

  // Create the attributes array for the noscript fallback element.
  $image_data['attributes']['noscript_fallback'] = _imager_js_create_noscript_fallback_attributes($image_data);

  return $image_data;
}

/**
 * Return combined defaults and settings array.
 */
function _imager_js_process_settings($display) {
  $settings = $display['settings'];
  $defaults = _imager_js_field_formatter_default_settings();
  return array_merge($defaults, $settings);
}

/**
 * Create and return the image srcs for each of the image styles.
 */
function _imager_js_return_image_srcs($image_data) {
  // Get image uri and image styles to pass to _imager_js_create_image_sources()
  $uri = $image_data['item']['uri'];
  $image_styles = $image_data['settings']['image_styles'];

  // Return the image srcs.
  return _imager_js_create_image_sources($uri, $image_styles);
}

/**
 * Create and return the image src for the background_image_fallback.
 */
function _imager_js_return_background_image_fallback_src($image_data) {
  if ($image_data['settings']['background_image_fallback'] === 'none') {
    return;
  }

  // Get image uri and image styles to pass to _imager_js_create_image_sources()
  $uri = $image_data['item']['uri'];
  $image_styles = array($image_data['settings']['background_image_fallback']);
    
  // Return the image srcs.
  $image_srcs = _imager_js_create_image_sources($uri, $image_styles);

  // Cycle image srcs and return the first src. 
  foreach ($image_srcs as $image_src) {
    return $image_src['src'];
  }
  
  return;
}

/**
 * Create and return the image src for the noscript_fallback.
 */
function _imager_js_return_noscript_fallback_src($image_data) {
  if ($image_data['settings']['noscript_fallback'] === 'none') {
    return;
  }

  // Get image uri and image styles to pass to _imager_js_create_image_sources()
  $uri = $image_data['item']['uri'];
  $image_styles = array($image_data['settings']['noscript_fallback']);
    
  // Return the image srcs.
  $image_srcs = _imager_js_create_image_sources($uri, $image_styles);

  // Cycle image srcs and return the first src. 
  foreach ($image_srcs as $image_src) {
    return $image_src['src'];
  }
  
  return;
}

/**
 * Get multiple image sources for an image uri.
 * Pass a uri and an array of image styles and this function
 * will create the required images and return an array of
 * their srcs.
 * @param  string $uri
 *         The uri of the original file. Normally starts public://
 * 
 * @param  array  $image_styles
 *         Optional array of image styles.
 * 
 * @return array
 *         Returns an array of srcs.
 */
function _imager_js_create_image_sources($uri, $image_styles) {
  $images = array();

  // Get image styles.
  $image_styles_in_system = image_styles();

  foreach ($image_styles as $style_name) {
    if (gettype($style_name) !== 'string') {
      continue;
    }
    $style = $image_styles_in_system[$style_name];

    // If the style name isn't valid then break here.
    if (!isset($image_styles_in_system[$style_name])) {
      continue;
    }

    // Checks path for image style.
    $derivative_uri = image_style_path($style_name, $uri);
    
    // Check for (and create) image style version of image.
    $image_exists = file_exists(image_style_path($style_name, $uri));
    
    // Test for whether image exists.
    if (!$image_exists) {
      // If image doesn't already exist then create it.
      if (!image_style_create_derivative($style, $uri, $derivative_uri)) {
        // If function returns false then we have an error, in which case we 
        // should skip this loop.
        continue;
      }
    }

    // Get image url from uri.
    $file_src = file_create_url($derivative_uri);

    $image_width = NULL;

    // Cycle through effects to get image width and height.
    // Please note that if using any effects other than scale, the width value
    // may will not necessarily be correct.
    foreach ($style['effects'] as $effect) {
      if (isset($effect['data']['width'])) {
        if ($image_width == NULL) {
          $image_width = $effect['data']['width'];
        }
        else {
          if ($effect['data']['width'] < $image_width) {
            $image_width = $effect['data']['width'];
          }
        }
      }
    }

    if (!$image_width) {
      continue;
    }

    // Save to images array keyed by width.
    $images[$style_name] = array(
      'width' => $image_width,
      'src' => $file_src,
      'style' => $style
    );
  }

  return $images;
}

/**
 * Create the attributes array for the wrapper element.
 */
function _imager_js_create_wrapper_attributes($image_data) {
  // Get display settings.
  $settings = $image_data['settings'];

  // Create empty attributes array.
  $attributes = array();

  // Add class.
  $attributes['class'] = 'drupal-imager-js-wrapper';

  if ($settings['set_intrinsic_ratio']) {
    $attributes['class'] .= ' intrinsic-ratio';
  }

  // If intrinsic ratio is set then add style attribute with padding bottom
  // value.
  if ($settings['set_intrinsic_ratio']) {
    // Get height and width values.
    $height = $image_data['item']['height'];
    $width = $image_data['item']['width'];

    // If we have height and width then calculate the padding-bottom value and 
    // create the CSS rule and add it to the style attribute.
    if ($height && $width) {
      $aspect = $height/$width;
      $padding_bottom = ($aspect*100) . '%';
      $style = "padding-bottom:{$padding_bottom};";

      // If we don't already have a style attribute create it, otherwise append to
      // the existing style attribute.
      if (!isset($attributes['style'])) {
        $attributes['style'] = $style;
      }
      else {
        $attributes['style'] .= $style;
      }
    }
  }

  // If background image fallback is set, then add the background image to the
  // style attribute.
  if ($image_data['background_image_fallback_src']) {
    $src = $image_data['background_image_fallback_src'];
    $style = "background-image:url({$src});";

    // If we don't already have a style attribute create it, otherwise append to
    // the existing style attribute.
    if (!isset($attributes['style'])) {
      $attributes['style'] = $style;
    }
    else {
      $attributes['style'] .= $style;
    }
  }
  
  // Create data-src attributes.
  foreach ($image_data['srcs'] as $src) {
    // Get default width.
    $width = $src['width'];

    // We need to divide the width by the image size multiplier.
    $image_size_multiplier = $settings['image_size_multiplier'];

    if ($image_size_multiplier) {
      $width = floor($width/$image_size_multiplier);
    }

    $attributes["data-src-{$width}"] = $src['src'];
  }

  return $attributes;
}

/**
 * Create the attributes array for the place holder element.
 */
function _imager_js_create_placeholder_attributes($image_data) {
  // Create empty attributes array.
  $attributes = array();

  // Imager class - this is the class that is passed to Imager when it's
  // initialised.
  $attributes['class'] = 'drupal-imager-js';

  // Data-src, required by Imager.
  $attributes['data-src'] = '{width}';

  // Data-alt, is transferred to the image element as the alt tag.
  $attributes['data-alt'] = $image_data['item']['alt'];

  return $attributes;
}

/**
 * Create the attributes array for the noscript fallback element.
 */
function _imager_js_create_noscript_fallback_attributes($image_data) {
  // Create empty attributes array.
  $attributes = array();

  // Add a class so that the image can be styled differently if necessary.
  $attributes['class'] = 'drupal-imager-js-noscript-img';

  // Data-alt, is transferred to the image element as the alt tag.
  $attributes['alt'] = $image_data['item']['alt'];

  // Add image src to attributes.
  if ($image_data['noscript_fallback_src']) {
    $attributes['src'] = $image_data['noscript_fallback_src'];
  }

  return $attributes;
}

