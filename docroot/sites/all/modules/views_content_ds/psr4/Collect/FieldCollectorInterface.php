<?php


namespace Drupal\views_content_ds\Collect;


interface FieldCollectorInterface {

  /**
   * @param string $viewId
   * @param string $displayId
   * @param string $entityType
   * @param string $localizedViewName
   * @param string $localizedDisplayName
   */
  function addViewsContentDisplay($viewId, $displayId, $entityType, $localizedViewName, $localizedDisplayName);
} 
