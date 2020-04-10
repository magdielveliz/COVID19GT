<?php
/**
 * @file
 * Register fancy_grid views plugins.
 *
 */

/**
 * Implements hook_views_plugins().
 */
function fancy_grid_views_views_plugins() {
  $plugins = array(
    'style' => array(
      'fancy_grid' => array(
        'title' => t('Fancy Grid'),
        'handler' => 'views_plugin_style_fancy_grid',
        'theme' => 'views_view_fancy_grid',
        'theme path' => drupal_get_path('module', 'fancy_grid_views') . '/theme',
        'help' => t('Render your View as Fancy Grid.'),
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-unformatted',
      ),
    ),
  );

  return $plugins;
}
