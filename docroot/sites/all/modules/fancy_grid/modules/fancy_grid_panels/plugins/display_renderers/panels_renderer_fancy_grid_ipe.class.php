<?php

/**
 * Alter the ipe renderer to add our own classes.
 */
class panels_renderer_fancy_grid_ipe extends panels_renderer_ipe {

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
    // @todo: quickfix for PHP notice of undefined index 'settings',
    // @Bene check if more fixes are needed
    if (isset($pane->style['settings'])) {
      $contexts['pane'] = $pane->style['settings'];
    }

    $pane->configuration['contexts'] = $contexts;

    $output = parent::render_pane($pane);

    return $output;
  }

  function ajax_save_form($break = NULL) {
    parent::ajax_save_form($break);

    // Fancy Grid initialisation
    if (empty($form_state['executed'])) {
      $this->commands[] = array(
        'command' => 'initFancy Grid',
        'key' => $this->clean_key,
      );
    }
  }

  /**
   * Create a command array to redraw a pane.
   */
  function command_update_pane($pid) {
    if (is_object($pid)) {
      $pane = $pid;
    }
    else {
      $pane = $this->display->content[$pid];
    }

    parent::command_update_pane($pid);

    $classes_array = _fancy_grid_generate_classes($pane->style['settings']);

    $this->commands[] = array(
      'command' => 'updateFancy GridElement',
      'selector' => "#panels-ipe-paneid-$pane->pid",
      'data' => array('cls' => implode(' ', $classes_array)),
    );
  }

  /**
   * Create a command array to add a new pane.
   */
  function command_add_pane($pid) {
    if (is_object($pid)) {
      $pane = $pid;
    }
    else {
      $pane = $this->display->content[$pid];
    }

    parent::command_add_pane($pid);

    $classes_array = _fancy_grid_generate_classes($pane->style['settings']);

    $this->commands[] = array(
      'command' => 'updateFancy GridElement',
      'selector' => "#panels-ipe-paneid-$pane->pid",
      'data' => array('cls' => implode(' ', $classes_array)),
    );
  }
}
