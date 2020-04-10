<?php


namespace Drupal\views_content_ds\Collect;


class ViewsProcessor {

  /**
   * @var string[]
   */
  protected $entityTypesMap = array();

  /**
   * @var FieldCollectorInterface
   */
  protected $fieldCollector;

  /**
   * @param array $entityInfo
   *   The result of \entity_get_info()
   * @param FieldCollectorInterface $fieldCollector
   */
  function __construct(array $entityInfo, FieldCollectorInterface $fieldCollector) {
    $this->fieldCollector = $fieldCollector;
    foreach ($entityInfo as $entity_type => $info) {
      $this->addEntityType($entity_type, $info['entity keys']['id']);
    }
  }

  /**
   * @param string $typeName
   * @param string $primaryKeyName
   *   Name of the entity type's primary key.
   */
  private function addEntityType($typeName, $primaryKeyName) {
    $this->entityTypesMap["entity:$typeName.$primaryKeyName"] = $typeName;
  }

  /**
   * @param \view[] $allViews
   *   The result of views_get_all_views()
   */
  function processAllViews(array $allViews) {
    foreach ($allViews as $view) {
      $this->processViewsView($view);
    }
  }

  /**
   * @param \view $view
   */
  private function processViewsView($view) {

    if (!empty($view->disabled)) {
      return;
    }

    $view->init_display();

    /** @var \views_display $display */
    foreach ($view->display as $display) {

      $this->processViewsDisplay($display);
    }

    $view->destroy();
  }

  /**
   * @param \views_display $display
   */
  private function processViewsDisplay($display) {

    if (empty($display->handler->panel_pane_display)) {
      return;
    }
    if (empty($display->display_options['argument_input'])) {
      return;
    }
    if (count($display->display_options['argument_input']) > 1) {
      return;
    }

    $arg_options = reset($display->display_options['argument_input']);

    if (!isset($arg_options['context'])) {
      return;
    }

    $key = str_replace('-', '_', $arg_options['context']);
    if (isset($this->entityTypesMap[$key])) {
      $entityType = $this->entityTypesMap[$key];
    }
    elseif ($key === 'entity:group.entity_id') {
      $entityType = 'node';
    }
    else {
      return;
    }
    $this->fieldCollector->addViewsContentDisplay(
      $display->handler->view->name,
      $display->id,
      $entityType,
      t($display->handler->view->human_name),
      t($display->display_title)
    );
  }
} 
