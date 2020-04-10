<?php
require_once drupal_get_path('module', 'panels') . '/plugins/display_renderers/panels_renderer_standard.class.php';

/**
 * Alter the ipe renderer to add our own classes.
 */
class panels_renderer_fancy_grid extends panels_renderer_standard {

  function add_meta() {
    parent::add_meta();

    if (user_access('administer fancy_grid')) {
      ctools_add_js('fancy_grid', 'fancy_grid');
    }
  }

  function prepare_regions($region_pane_list, $settings) {
    parent::prepare_regions($region_pane_list, $settings);

    // Set region default to fancy_grid
    $this->prepared['regions'] = _fancy_grid_panels_prepare_regions($this->prepared['regions']);

    return $this->prepared['regions'];
  }

  function prepare_panes($panes) {
    parent::prepare_panes($panes);

    // Set region default to fancy_grid
    $this->prepared['panes'] = _fancy_grid_panels_prepare_panes($this->prepared['panes']);

    return $this->prepared['panes'];
  }

  /**
   * Render pane. Currently we only call the default ipe render function.
   * This may be altered later for our own wrapper.
   *
   * @param $pane
   */
  function render_pane(&$pane) {
    // Create fancy_grid context.
    $contexts = array();

    // Add panel style to context.
    if (isset($this->display->panel_settings['style_settings'][$pane->panel])) {
      $contexts['panel'] = $this->display->panel_settings['style_settings'][$pane->panel];
    }

    // Add pane style to context.
    $contexts['pane'] = $pane->style['settings'];

    $pane->configuration['contexts'] = $contexts;

    $output = parent::render_pane($pane);

    return $output;
  }
}
