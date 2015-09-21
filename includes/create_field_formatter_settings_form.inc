<?php

/**
 * Create field formatter settings form.
 */
function _imager_js_create_field_formatter_settings_form($settings) {

  // Get the available image styles.
  $available_image_styles = array();
  foreach (image_styles() as $image_style) {
    if (isset($image_style['label'])) {
      $available_image_styles[$image_style['name']] = $image_style['label'];
    }
    else {
      $available_image_styles[$image_style['name']] = $image_style['name'];
    }
  }

  // Get default styles.
  // If they are already set in settings then use these.
  if (count($settings['image_styles']) !== 0) {
    $default_image_styles = $settings['image_styles'];
  }
  // Else get the default styles.
  else {
    // This is not getting picked up.
    $default_image_style_keys = array_keys(imager_js_image_default_styles());
    $default_image_styles = array_combine($default_image_style_keys, $default_image_style_keys);
  }

  $element = array();

  $form_element['image_styles'] = array(
    '#type'           => 'select',
    '#multiple'       => TRUE,
    '#title'          => t('Image styles'),
    '#description'    => t('The available image styles.'),
    '#default_value'  => $default_image_styles,
    '#options'        => $available_image_styles,
  );

  // Add 'none' option to image styles.
  $available_image_styles['none'] = '-- None --';

  $form_element['background_image_fallback'] = array(
    '#type'           => 'select',
    '#multiple'       => FALSE,
    '#title'          => t('Background image fallback'),
    '#description'    => t('Renders a version of the image as a background image on the wrapper. This can be used as a fallback for devices that don\'t have js enabled, or can be used as a placeholder that will show before the full res image loads. Select an image style to use, or select none to disable.'),
    '#default_value'  => $settings['background_image_fallback'],
    '#options'        => $available_image_styles,
  );

  $form_element['noscript_fallback'] = array(
    '#type'           => 'select',
    '#multiple'       => FALSE,
    '#title'          => t('Noscript fallback image'),
    '#description'    => t('Renders a fallback image inside noscript tags. Select an image style to use, or select none to disable.'),
    '#default_value'  => $settings['noscript_fallback'],
    '#options'        => $available_image_styles,
  );

  $form_element['wrapper_element'] = array(
    '#type'           => 'select',
    '#title'          => t('Wrapper element'),
    '#description'    => t('Wrapper element type.'),
    '#default_value'  => $settings['wrapper_element'],
    '#options'        => array(
      'div' => '<div/>',
      'span'  => '<span/>',
    ),
  );

  $form_element['inline_script'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Inline script'),
    '#description'    => t('Render script tags inline to trigger Imager\'s .add() method. Rendering script inline will mean that the .add() method is called as early as possible. If disabled the .add() method will be triggered on Drupal\'s behaviors attach. Should be disabled if scripts are moved to footer. Please note, inline script tags are stripped from the output of views\' ajax mode.'),
    '#default_value'  => $settings['inline_script'],
  );

  $form_element['set_intrinsic_ratio'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Intrinsic ratio'),
    '#description'    => t('Sets image container to correct aspect ratio using the intrinsic ratio method. This will add an inline style to the wrapper element with a padding-bottom value.'),
    '#default_value'  => $settings['set_intrinsic_ratio'],
  );

  $form_element['image_size_multiplier'] = array(
    '#type'           => 'select',
    '#title'          => t('Image size multiplier'),
    '#description'    => t('Base multiplier to increase the size of the images. This can be used to compensate for lower quality images or higher image compression rates. Technically this lowers size of the breakpoint at which the images are switched, so you may wish to include larger image styles in the options.'),
    '#default_value'  => $settings['image_size_multiplier'],
    '#options'        => array(
      '1'    => '1',
      '1.25'  => '1.25',
      '1.5'   => '1.5',
      '1.75'  => '1.75',
      '2'     => '2',
      '2.25'  => '2.25',
      '2.5'   => '2.5',
      '2.75'  => '2.75',
      '3'     => '3',
    ),
  );

  return $form_element;
}