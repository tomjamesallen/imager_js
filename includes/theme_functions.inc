<?php

/**
 * @file
 * Theme functions.
 */

/**
 * Render the image placeholder element.
 */
function theme_imager_js_placeholder($variables) {
  // If we are using the background-imager.js plugin we do not need a js
  // placeholder.
  if ($variables['settings']['plugin'] === 'background-imager.js') {
    return '';
  }
  // Set default elem to div.
  $elem = 'div';

  // If wrapper element is set to span in settings, then set elem to span.
  if ($variables['settings']['wrapper_element'] === 'span') {
    $elem = 'span';
  }

  $attr = $variables['attributes']['placeholder'];
  $rendered_attr = drupal_attributes($attr);

  return "<{$elem}{$rendered_attr}></{$elem}>";
}

/**
 * Render the noscript fallback.
 */
function theme_imager_js_noscript_fallback($variables) {
  if (!$variables['noscript_fallback_src']) {
    return;
  }
  $attr = $variables['attributes']['noscript_fallback'];
  $rendered_attr = drupal_attributes($attr);
  return "<noscript><img{$rendered_attr}/></noscript>";
}

/**
 * Render the script element.
 */
function theme_imager_js_script($variables) {
  if ($variables['settings']['inline_script']) {
    if ($variables['settings']['plugin'] === 'imager.js') {
      return '<script>Drupal.imagerJs.add();</script>';
    }
    if ($variables['settings']['plugin'] === 'background-imager.js') {
      return '<script>Drupal.bgImagerJs.add();</script>';
    }
  }
  return;
}

/**
 * Wrapper element. Contains js placeholder noscript fallback.
 */
function theme_imager_js_wrapper($variables) {
  // Set default elem to div.
  $elem = 'div';

  // If wrapper element is set to span in settings, then set elem to span.
  if ($variables['settings']['wrapper_element'] === 'span') {
    $elem = 'span';
  }

  // Create inner vairable and set value to that of the rendered placeholder.
  $inner = $variables['rendered']['placeholder'];

  // If we have a noscript fallback then append that.
  if ($variables['rendered']['noscript_fallback']) {
    $inner .= $variables['rendered']['noscript_fallback'];
  }

  // Get the wrapper element's attributes.
  $attr = $variables['attributes']['wrapper'];
  $rendered_attr = drupal_attributes($attr);

  // Return rendered wrapper.
  return "<{$elem}{$rendered_attr}>{$inner}</{$elem}>";
}

/**
 * Master theme function. Assembles other elements.
 */
function theme_imager_js_master($variables) {
  $output = $variables['rendered']['wrapper'];

  if ($variables['rendered']['script']) {
    $output .= $variables['rendered']['script'] .= "\n";
  }

  return $output;
}
