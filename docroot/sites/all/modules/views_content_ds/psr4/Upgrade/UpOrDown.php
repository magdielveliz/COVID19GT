<?php


namespace Drupal\views_content_ds\Upgrade;


use Drupal\views_content_ds\Collect\ViewsProcessor;

class UpOrDown {

  /**
   * @var UpOrDownFieldCollectorInterface
   */
  private $collector;

  /**
   * @param string $direction
   *   Either 'up' or 'down'.
   *
   * @throws \InvalidArgumentException
   * @return UpOrDown
   */
  static function create($direction) {
    switch ($direction) {
      case 'up':
        $collector = new UpgradeFieldCollector();
        break;
      case 'down':
        $collector = new DowngradeFieldCollector();
        break;
      default:
        throw new \InvalidArgumentException("Unsupported direction.");
    }
    $viewsProcessor = new ViewsProcessor(entity_get_info(), $collector);
    $viewsProcessor->processAllViews(views_get_all_views());
    return new self($collector);
  }

  /**
   * @param UpOrDownFieldCollectorInterface $collector
   */
  function __construct(UpOrDownFieldCollectorInterface $collector) {
    $this->collector = $collector;
  }

  /**
   * Upgrade from 7.x-1.x to 7.x-2.x
   */
  function run() {
    $newFieldNamesById = $this->fieldSettingsRenameFields();
    if (empty($newFieldNamesById)) {
      return;
    }
    $this->layoutSettingsRenameFields($newFieldNamesById);
  }

  /**
   * @return string[]
   */
  private function fieldSettingsRenameFields() {
    $fieldSettingsById = $this->loadDsFieldSettings();
    $newFieldNamesById = array();
    foreach ($fieldSettingsById as $id => $settings) {
      $newFieldNames = $this->collector->fieldSettingsTransform($settings);
      if (empty($newFieldNames)) {
        continue;
      }
      $newFieldNamesById[$id] = $newFieldNames;
      $this->saveDsFieldSettings($id, $settings);
    }
    return $newFieldNamesById;
  }

  /**
   * @return array[]
   *   Field settings by view mode id.
   *   View mode ids are like "taxonomy_term|categories|full"
   */
  private function loadDsFieldSettings() {
    $q = \db_select('ds_field_settings', 'dsfs');
    $q->fields('dsfs', array('id', 'settings'));
    $settingsById = array();
    /** @var \stdClass $row */
    foreach ($q->execute()->fetchAll() as $row) {
      $settingsById[$row->id] = \unserialize($row->settings);
    }
    return $settingsById;
  }

  /**
   * @param string $id
   * @param array $fieldSettings
   */
  private function saveDsFieldSettings($id, array $fieldSettings) {
    $q = db_update('ds_field_settings');
    $q->condition('id', $id);
    $q->fields(array(
      'settings' => \serialize($fieldSettings),
    ));
    $q->execute();
  }

  /**
   * @return array[]
   *   Layout settings by view mode id.
   *   View mode ids are like "taxonomy_term|categories|full"
   */
  private function loadDsLayoutSettings() {
    $q = \db_select('ds_layout_settings', 'dsls');
    $q->fields('dsls', array('id', 'settings'));
    $settingsById = array();
    /** @var \stdClass $row */
    foreach ($q->execute()->fetchAll() as $row) {
      $settingsById[$row->id] = \unserialize($row->settings);
    }
    return $settingsById;
  }

  /**
   * @param string $id
   * @param array $layoutSettings
   */
  private function saveDsLayoutSettings($id, array $layoutSettings) {
    $q = db_update('ds_layout_settings');
    $q->condition('id', $id);
    $q->fields(array(
      'settings' => \serialize($layoutSettings),
    ));
    $q->execute();
  }

  /**
   * @param string[] $newFieldNamesById
   */
  private function layoutSettingsRenameFields(array $newFieldNamesById) {
    $layoutSettingsById = $this->loadDsLayoutSettings();
    foreach ($newFieldNamesById as $id => $newFieldNames) {
      if (!isset($layoutSettingsById[$id])) {
        continue;
      }
      $layoutSettings = $layoutSettingsById[$id];
      $settingsHaveChanged = $this->viewModeLayoutSettingsRenameFields($layoutSettings, $newFieldNames);
      if ($settingsHaveChanged) {
        // Save the changes.
        $this->saveDsLayoutSettings($id, $layoutSettings);
      }
    }
  }

  /**
   * @param array $layoutSettings
   * @param string[] $newFieldNames
   *
   * @return bool
   *   TRUE, if something has changed in the settings.
   */
  private function viewModeLayoutSettingsRenameFields(array &$layoutSettings, array $newFieldNames) {
    $settingsHaveChanged = FALSE;
    if (!empty($layoutSettings['regions'])) {
      foreach ($layoutSettings['regions'] as &$regionFields) {
        foreach ($regionFields as &$fieldName) {
          if (isset($newFieldNames[$fieldName])) {
            $fieldName = $newFieldNames[$fieldName];
            $settingsHaveChanged = TRUE;
          }
        }
      }
    }
    if (!empty($layoutSettings['fields'])) {
      foreach ($layoutSettings['fields'] as $fieldName => $regionName) {
        if (isset($newFieldNames[$fieldName])) {
          $newFieldName = $newFieldNames[$fieldName];
          $layoutSettings['fields'][$newFieldName] = $regionName;
          unset($layoutSettings['fields'][$fieldName]);
          $settingsHaveChanged = TRUE;
        }
      }
    }
    return $settingsHaveChanged;
  }
} 
