<?php


namespace Drupal\views_content_ds\Collect;


class FieldCollector implements FieldCollectorInterface {

  /**
   * @var array[]
   *   Fields configuration by entity type and field name. E.g.
   *
   *   $this->fields['node']['field_body'] = array(
   *
   *   );
   */
  private $fields = array();

  /**
   * @param string $viewId
   * @param string $displayId
   * @param string $entityType
   * @param string $localizedViewName
   * @param string $localizedDisplayName
   */
  function addViewsContentDisplay($viewId, $displayId, $entityType, $localizedViewName, $localizedDisplayName) {
    $fieldName = 'views_content_ds__' . $viewId . '__' . $displayId;
    $fieldLabel = t($localizedViewName);
    $fieldLabel .= ': ' . t($localizedDisplayName);
    $fieldLabel .= ' (' . t('Views content DS') . ')';
    # $admin_path = "admin/structure/views/view/$viewId/edit/$displayId";
    $this->fields[$entityType][$fieldName] = array(
      'title' => $fieldLabel,
      'field_type' => DS_FIELD_TYPE_FUNCTION,
      'function' => '_views_content_ds_field_print',
      // These two settings have no meaning for ds api,
      // we just need them in the callback.
      '_view_id' => $viewId,
      '_display_id' => $displayId,
      // We could restrict to specific bundles and view modes, if we wanted.
      'ui_limit' => array('*|*'),
    );
  }

  /**
   * Get the collected result.
   *
   * @return array[]
   *   The collected fields information.
   */
  function getCollectedFields() {
    return $this->fields;
  }
}
