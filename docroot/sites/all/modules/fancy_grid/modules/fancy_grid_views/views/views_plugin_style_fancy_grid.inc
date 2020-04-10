<?php
/**
 * @file
 * Contains the fancy_grid style plugin.
 */

/**
 * Fancy Grid style plugin to render rows one after another with fancy_grid classes.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_fancy_grid extends views_plugin_style {

  // Alter default options
  function option_definition() {
    $options = parent::option_definition();

    if ($this->uses_row_class()) {
      $options['default_row_class'] = array('default' => FALSE, 'bool' => TRUE);
      $options['row_class_special'] = array('default' => FALSE, 'bool' => TRUE);
    }

    $options['no_alignment'] = array('default' => 0);
    $options['is_container'] = array('default' => 0);
    $options['is_wrapper'] = array('default' => 0);
    $options['layouts'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $settings = isset($form_state['values']) ? $form_state['values'] : $this->options;
    $settings_form = _fancy_grid_element_config_form('view', $settings);

    $form = array_merge($form, $settings_form);
  }
}
