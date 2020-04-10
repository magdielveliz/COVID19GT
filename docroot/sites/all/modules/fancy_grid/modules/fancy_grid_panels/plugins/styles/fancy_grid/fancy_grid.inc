<?php
/**
 * @file
 * Register fancy_grid panels style plugin.
 *
 */

/**
* Implements hook_panels_styles().
*/
$plugin =  array(
  'fancy_grid' => array(
    'title' => t('Fancy Grid'),
    'description'   => t('Flexible and configurable grid system for drupal panels and views'),
    'render pane' => 'fancy_grid_panels_render_pane',
    'pane settings form' => 'fancy_grid_panels_pane_settings_form',
    'render region' => 'fancy_grid_panels_render_region',
    'settings form' => 'fancy_grid_panels_region_settings_form',
  ),
);

/**
* Theme funciton for pane rendering.
*/
function theme_fancy_grid_panels_render_pane($vars) {
  $settings = $vars['settings'];
  $content = $vars['content'];
  $settings['contexts'] = isset($vars['pane']->configuration['contexts']) ?
    $vars['pane']->configuration['contexts'] : array();
  $extra_classes = array();

  // Avoid that panes get context to themself.
  unset($settings['contexts']['pane']);

  // Apply user defined css class to wrapper
  if (isset($vars['pane']->css['css_class'])) {
    $extra_classes[] = $vars['pane']->css['css_class'];
    unset($vars['content']->css_class);
  }

  $output = theme('panels_pane', array('content' => $vars['content'], 'pane' => $vars['pane'], 'display' => $vars['display']));

  $output = _fancy_grid_wrap_element($output, $settings, $extra_classes);

  return $output;
}

/**
* Pane configuration form.
*/
function fancy_grid_panels_pane_settings_form($style_settings) {
  $form = array();

  $form = _fancy_grid_element_config_form('pane', $style_settings);

  return $form;
}

/**
* Theme funciton for panel/region rendering.
*/
function theme_fancy_grid_panels_render_region($vars) {
  $settings = $vars['settings'];

  $extra_classes = array();
  $extra_classes[] = 'grid-region';
  $extra_classes[] = 'region';
  $extra_classes[] = 'region-' . $vars['region_id'];

  if (count($vars['panes']) == 0) {
    return FALSE;
  }

  $panes = implode("\n", $vars['panes']);

  $output = _fancy_grid_wrap_element($panes, $settings, $extra_classes);

  return $output;
}

/**
* Panel/Region configuration form.
*/
function fancy_grid_panels_region_settings_form($style_settings) {
  $form = array();

  $form = _fancy_grid_element_config_form('region', $style_settings);

  return $form;
}
