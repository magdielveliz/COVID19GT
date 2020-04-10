<?php
/**
 * @file
 * Views template functions for fancy_grid.
 *
 */

/**
 * Display the fancy_grid view of rows
 */
function template_preprocess_views_view_fancy_grid(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $style = $view->style_plugin;
  $options = $style->options;
  $layouts = _fancy_grid_get_layouts();
  $pane_config = $view->display_handler->get_option('pane_conf');

  // Run views unformatted theme function.
  template_preprocess_views_view_unformatted($vars);

  // Init column counter. This is used to detect linebreaks.
  $c_count = array();
  foreach ($layouts as $machine_name => $layout) {
    $c_count[$machine_name] = 1;
  }

  // Add Fancy Grid classes.
  foreach ($rows as $id => $row) {
    $tmp_options = $options;

    // Overwrite options by pane config.
    if (isset($pane_config['fancy_grid_overwrite'])) {
      $tmp_options = array_merge($tmp_options, $pane_config['fancy_grid_overwrite']);
    }

    // Set correct column per layout.
    foreach ($layouts as $machine_name => $layout) {
      $tmp_options['layouts'][$machine_name]['column'] = $c_count[$machine_name];

      // Force new line at column 1.
      if ($c_count[$machine_name] == 1) {
        $tmp_options['layouts'][$machine_name]['linebreak'] = 1;
      }
      else {
        $tmp_options['layouts'][$machine_name]['linebreak'] = 0;
      }
    }

    // Set fancy_grid context.
    $tmp_options['contexts'] = $pane_config['contexts'];

    // Set classes
    $fancy_grid_classes = _fancy_grid_generate_classes($tmp_options);

    if (!isset($vars['classes'][$id])) {
      $vars['classes'][$id] = $fancy_grid_classes;
    }
    else {
      $vars['classes'][$id] = array_merge($fancy_grid_classes, $vars['classes'][$id]);
    }

    // Calculate element column per layout for next row.
    foreach ($layouts as $machine_name => $layout) {
      $last_column = $layouts[$machine_name]['columns'];

      // Last column based on contexts.
      if (isset($pane_config['contexts'])) {
        $current_context = FALSE;
        foreach ($pane_config['contexts'] as $context) {
          if (isset($context['layouts'][$machine_name]['range'])) {
            if ($context['layouts'][$machine_name]['range'] < $current_context
                || !$current_context) {
              $current_context = $context['layouts'][$machine_name]['range'];
            }
          }
        }
        if ($current_context) {
          $last_column = $current_context;
        }
      }

      $range = $tmp_options['layouts'][$machine_name]['range'];
      $c_count[$machine_name] += $range;

      // Calculate new line.
      if ($c_count[$machine_name] + $range - 1  > $last_column) {
        $c_count[$machine_name] = 1;
      }
    }

    // Flatten the classes to a string for each row for the template file.
    $vars['classes_array'][$id] = (isset($vars['classes'][$id])) ? implode(' ', $vars['classes'][$id]) : '';
  }
}
