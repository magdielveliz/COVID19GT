<?php


namespace Drupal\views_content_ds\Upgrade;


use Drupal\views_content_ds\Collect\FieldCollectorInterface;

class DowngradeFieldCollector implements FieldCollectorInterface, UpOrDownFieldCollectorInterface {

  /**
   * @var array[]
   */
  private $oldFields = array();

  /**
   * @param string $viewId
   * @param string $displayId
   * @param string $entityType
   * @param string $localizedViewName
   * @param string $localizedDisplayName
   */
  function addViewsContentDisplay($viewId, $displayId, $entityType, $localizedViewName, $localizedDisplayName) {
    $oldFieldName = 'views_content_ds__' . $viewId;
    $newFieldName = 'views_content_ds__' . $viewId . '__' . $displayId;
    $this->oldFields[$newFieldName] = array($oldFieldName, $displayId);
  }

  /**
   * @param array[] $settings
   *   Field settings for a specific view mode.
   *
   * @return string[]
   *   Renamed field names by original field name.
   */
  function fieldSettingsTransform(array &$settings) {

    $renameMap = array();
    foreach ($settings as $newFieldName => $fieldSettings) {

      if (empty($fieldSettings['format']) || 'default' !== $fieldSettings['format']) {
        continue;
      }

      if (empty($this->oldFields[$newFieldName])) {
        continue;
      }
      list($oldFieldName, $displayId) = $this->oldFields[$newFieldName];

      // Rename the field.
      unset($settings[$newFieldName]);
      $fieldSettings['format'] = $displayId;
      $settings[$oldFieldName] = $fieldSettings;
      $renameMap[$newFieldName] = $oldFieldName;
    }

    return $renameMap;
  }
}
