<?php

use Drupal\views_content_ds\Upgrade\UpOrDown;


/**
 * Implements hook_drush_command()
 *
 * @return array
 */
function views_content_ds_drush_command() {
  $items = array();
  $items['views-content-ds-up'] = array(
    'description' => 'Upgrade from views_content_ds-7.x-1.x to -7.x-2.x'
  );
  $items['views-content-ds-down'] = array(
    'description' => 'Downgrade from views_content_ds-7.x-1.x to -7.x-2.x'
  );
  return $items;
}

/**
 * Drush command callback for 'drush views-content-ds-up'
 */
function drush_views_content_ds_up() {
  UpOrDown::create('up')->run();
}

/**
 * Drush command callback for 'drush views-content-ds-up'
 */
function drush_views_content_ds_down() {
  UpOrDown::create('down')->run();
}
