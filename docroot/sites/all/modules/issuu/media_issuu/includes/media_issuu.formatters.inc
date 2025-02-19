<?php
/**
 * @file
 * Integration with File Entity module for display.
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_issuu_file_formatter_info() {
  $formatters = array();
  $formatters['media_issuu_document'] = array(
    'label' => t('Issuu Document'),
    'file types' => array('default'),
    'default settings' => array(
      'width' => media_issuu_variable_get('width'),
      'height' => media_issuu_variable_get('height'),
    ),
    'view callback' => 'media_issuu_file_formatter_embed_view',
    'settings callback' => 'media_issuu_file_formatter_embed_settings',
  );
  $formatters['media_issuu_image'] = array(
    'label' => t('Issuu Preview Image'),
    'file types' => array('default'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_issuu_file_formatter_image_view',
    'settings callback' => 'media_issuu_file_formatter_image_settings',
  );
  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_issuu_file_formatter_embed_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  /* WYSIWYG does not yet support document inside a running editor instance. */
  if ($scheme === 'issuu' && empty($file->override['wysiwyg'])) {
    $element = array(
      '#theme' => 'media_issuu_document',
      '#uri' => $file->uri,
    );
    foreach (array('width', 'height') as $setting) {
      $element['#' . $setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_issuu_file_formatter_embed_settings($form, &$form_state, $settings) {
  $element = array();
  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
  );
  $element['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
  );
  return $element;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_issuu_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);
  if ($scheme === 'issuu') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $image_style = $display['settings']['image_style'];
    $valid_image_styles = image_style_options(FALSE);
    if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
      $element = array(
        '#theme' => 'image',
        '#path' => $wrapper->getOriginalThumbnailPath(),
      );
    }
    else {
      $element = array(
        '#theme' => 'image_style',
        '#style_name' => $image_style,
        '#path' => $wrapper->getLocalThumbnailPath(),
      );
    }
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_issuu_file_formatter_image_settings($form, &$form_state, $settings) {
  $element = array();
  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );
  return $element;
}

/**
 * Implements hook_file_default_displays().
 */
function media_issuu_file_default_displays() {
  $default_displays = array();

  /* Default settings for displaying as a document. */
  $default_document_settings = array(
    'media_large' => array(
      'width' => 480,
      'height' => 360,
    ),
    'media_original' => array(
      'width' => 640,
      'height' => 480,
    ),
  );
  foreach ($default_document_settings as $view_mode => $settings) {
    $display_name = 'document__' . $view_mode . '__media_issuu_document';
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 1,
      'settings' => $settings,
    );
  }

  /* Default settings for displaying a document preview image. We enable preview */
  /* images even for view modes that also play document, for use inside a */
  /* running WYSIWYG editor. The higher weight ensures that the document */
  /* display is used where possible. */
  $default_image_styles = array(
    'media_preview' => 'square_thumbnail',
    'media_large' => 'large',
    'media_original' => '',
  );
  foreach ($default_image_styles as $view_mode => $image_style) {
    $display_name = 'document__' . $view_mode . '__media_issuu_image';
    $default_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 2,
      'settings' => array('image_style' => $image_style),
    );
  }

  return $default_displays;
}
