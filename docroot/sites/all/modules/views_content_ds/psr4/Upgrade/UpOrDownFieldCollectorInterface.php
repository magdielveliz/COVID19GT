<?php


namespace Drupal\views_content_ds\Upgrade;


interface UpOrDownFieldCollectorInterface {

  /**
   * @param array[] $settings
   *   Field settings for a specific view mode.
   *
   * @return string[]
   *   Renamed field names by original field name.
   */
  public function fieldSettingsTransform(array &$settings);
} 
