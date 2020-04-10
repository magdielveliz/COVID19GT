<?php


namespace Drupal\views_content_ds\Upgrade;


use Drupal\views_content_ds\Collect\FieldCollectorInterface;

class UpgradeFieldCollector implements FieldCollectorInterface, UpOrDownFieldCollectorInterface {

  /**
   * @var array[]
   */
  private $newFields = array();

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
    $this->newFields[$oldFieldName][$displayId] = $newFieldName;
  }

  /**
   * @param array[] $settings
   *   Field settings for a specific view mode.
   *
   * @return string[]
   *   Renamed field names by original field name.
   */
  public function fieldSettingsTransform(array &$settings) {

    $renameMap = array();
    foreach ($settings as $oldFieldName => $fieldSettings) {

      if (empty($fieldSettings['format']) || 'default' === $fieldSettings['format']) {
        continue;
      }
      $displayId = $fieldSettings['format'];

      if (!isset($this->newFields[$oldFieldName][$displayId])) {
        continue;
      }
      $newFieldName = $this->newFields[$oldFieldName][$displayId];

      // Rename the field.
      unset($settings[$oldFieldName]);
      $fieldSettings['format'] = 'default';
      $settings[$newFieldName] = $fieldSettings;
      $renameMap[$oldFieldName] = $newFieldName;
    }

    return $renameMap;
  }
}
