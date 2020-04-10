<?php
/**
 * @file
 * Admin configuration page for fancy_grid.
 *
 */

/**
 * Form for the Fancy Grid configuration page.
 * @param $form
 * @param $form_state
 * @return array
 */
function fancy_grid_admin($form, &$form_state) {
  $form = array();

  $layouts = _fancy_grid_get_layouts();
  $grid_width = _fancy_grid_get_grid_width();
  $grid_max_width = _fancy_grid_get_grid_max_width();
  $compile = _fancy_grid_get_compile();
  $compile_webservice = _fancy_grid_get_compile_webservice();
  $breakpoint_group = _fancy_grid_get_breakpoint_group();
  $breakpoint_groups = breakpoints_breakpoint_group_load_all();

  $form_values = array();
  if (isset($form_state['values']['layouts']))
    $form_values = $form_state['values']['layouts'];


  // Auto compiling
  $form['compile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Should Fancy Grid compile the css automatically after you save this form?'),
  );

  $options = array(1 => t('Compile via webservice'), 0 => t('I will take care about that myself'));
  $form['compile']['compile'] = array(
    '#type' => 'radios',
    '#default_value' => $compile,
    '#options' => $options,
  );
  $form['compile']['compile_webservice'] = array(
    '#type' => 'textfield',
    '#title' => t('Url to the SassMeister instance for css compilation.'),
    '#description' => t('You can set up your own using <a href="https://www.heroku.com">Heroku</a>. Just clone the code from the <a href="https://github.com/jedfoster/SassMeister">SassMeister repository</a>.'),
    '#default_value' => $compile_webservice,
    '#states' => array(
      'disabled' => array(
        ':input[name="compile"]' => array('value' => 0),
      ),
    ),
  );
  $form['compile']['force_recompile'] = array(
    '#type' =>'checkbox',
    '#title' => t('Recompile the css once when this form was saved successfully.'),
    '#default_value' => isset($form_values['force_recompile']) ? $form_values['force_recompile'] : FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="compile"]' => array('value' => 0),
      ),
    ),
  );

  // How to
  $form['compile']['howto'] = array(
    '#type' => 'fieldset',
    '#title' => t('How to implement Fancy Grid in my own sass environment?'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['compile']['howto']['implementation'] = array(
    '#markup' => '
    <ol>
      <li>
        Require Singularity.gs and Breakpoint in your config.rb file.
<pre>
require \'singularitygs\'
require \'breakpoint\'
</pre>
      </li>
      <li>
        Add Fancy Grid directory as import path to your config.rb file. These may fit your setup:
<pre>
# Fancy Grid imports.
' . implode("\n", _fancy_grid_sass_import_paths()) . '
</pre>
      </li>
      <li>
        Import Singularity.gs and Fancy Grid files in your sass files:
<pre>
@import \'singularitygs\';
@import \'fancy_grid\'; // Imports the Fancy Grid configuration
@import \'fancy_grid.config\'; // Imports the Fancy Grid mixins and default configs
// You can alter all singularitygs configurations between these imports.
@import \'fancy_grid.utilities\'; // Generates the actual grid classes
</pre>
      </li>
    </ol>
    ',
  );

  $form['grid_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Grid width'),
    '#default_value' => $grid_width,
    '#description' => t('The relative width of the grid. Can be any value. For Example use "90%"" for responsive layouts, "980px" for fixed layouts.'),
    '#required' => TRUE,
  );

  $form['grid_max_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum grid width'),
    '#default_value' => $grid_max_width,
    '#description' => t('The maximum width of the grid. For Example use "980px" for responsive layouts, "none" for fixed layouts.'),
    '#required' => TRUE,
  );

  $map_select_options = function($value) { return $value->name; };
  $options = array_map($map_select_options, $breakpoint_groups);

  $form['breakpoint_group'] = array(
    '#type' => 'select',
    '#title' => t('Breakpoint group'),
    '#default_value' => $breakpoint_group,
    '#options' => $options,
    '#description' => t('Select a Breakpoint group which will be used to generate the grid.'),
    '#required' => TRUE,
  );

  $form['from'] = array(
    '#type' => 'item',
    '#title' => t('Fancy Grid layout configuration'),
    '#description' => t('These settings will be used to generate the grid and the Fancy Grid forms.'),
  );

  // Breakpoints configuration.
  $form['layouts'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<div id="layouts-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  if (!empty($breakpoint_group)) {
    $group_settings = breakpoints_breakpoint_group_load($breakpoint_group);
    foreach ($group_settings->breakpoints as $breakpoint_key) {
      $breakpoint = breakpoints_breakpoint_load_by_fullkey($breakpoint_key);

      if (array_key_exists($breakpoint_key, $layouts)) {
        $layout = $layouts[$breakpoint_key];
      }
      else {
        $layout = array(
          'breakpoint' => _fancy_grid_layout_default_breakpoint_config($breakpoint),
          'identifier' => _fancy_grid_layout_default_identifier($breakpoint),
          'columns' => 12,
          'gutters' => '2%',
          'config' => '*|*'
        );
      }

      $form['layouts'][$breakpoint_key] = array(
        '#type' => 'fieldset',
        '#title' => $breakpoint->name,
      );

      $form['layouts'][$breakpoint_key]['title'] = array(
        '#markup' => '<h2>' . $breakpoint->name . ' (' . $breakpoint_key . ')</h2>'
      );
      $form['layouts'][$breakpoint_key]['details'] = array(
        '#markup' => 'Media query for this breakpoint: <code>@media ' . $breakpoint->breakpoint . '</code>',
      );

      $default_value = isset($form_values[$breakpoint_key]['breakpoint']) ? $form_values[$breakpoint_key]['breakpoint'] : $layout['breakpoint'];
      $form['layouts'][$breakpoint_key]['breakpoint'] = array(
        '#type' => 'textfield',
        '#title' => t('Breakpoint Sass module configuration'),
        '#default_value' => $default_value,
        '#description' => t('The <a href="@breakpoint">Breakpoint Sass library</a> configuration for this breakpoint. Keep in mind that Breakpoint can <strong>not</strong> handle configurations containing only a "max-width" definition. Also Singularitygs <strong>does not</strong> support <a href="@or">lists of media queries</a>.', array('@breakpoint' => 'https://github.com/at-import/breakpoint/wiki/Basic-Media-Queries', '@or' => 'https://github.com/at-import/breakpoint/wiki/Advanced-Media-Queries#or-queries')),
        '#field_prefix' => '<code>@include breakpoint(',
        '#field_suffix' => ') {}</code>',
        '#attributes' => array(
          'style' => 'min-width: 200px; width: 60%; max-width: 400px;'
        ),
        '#required' => TRUE,
      );

      $default_value = isset($form_values[$breakpoint_key]['identifier']) ? $form_values[$breakpoint_key]['identifier'] : $layout['identifier'];
      $form['layouts'][$breakpoint_key]['identifier'] = array(
        '#type' => 'machine_name',
        '#title' => t('Identifier'),
        '#default_value' => $default_value,
        '#description' => t('Will be used as part of the generated css classes.'),
        '#required' => TRUE,
        '#machine_name' => array(
          'exists' => 'fancy_grid_check_identifier',
          'replace_pattern' => '[^a-z0-9_-]+',
        ),
      );

      $default_value = isset($form_values[$breakpoint_key]['columns']) ? $form_values[$breakpoint_key]['columns'] : $layout['columns'];
      $form['layouts'][$breakpoint_key]['columns'] = array(
        '#type' => 'numberfield',
        '#title' => t('Columns'),
        '#default_value' => $default_value,
        '#description' => t('Define the number of columns for this layout.'),
        '#min' => 1,
        '#required' => TRUE,
      );

      $default_value = isset($form_values[$breakpoint_key]['gutters']) ? $form_values[$breakpoint_key]['gutters'] : $layout['gutters'];
      $form['layouts'][$breakpoint_key]['gutters'] = array(
        '#type' => 'textfield',
        '#title' => t('Gutters'),
        '#default_value' => $default_value,
        '#description' => t('Define the gutter for this layout.'),
        '#required' => TRUE,
      );

      $default_value = isset($form_values[$breakpoint_key]['config']) ? $form_values[$breakpoint_key]['config'] : $layout['config'];
      $form['layouts'][$breakpoint_key]['config'] = array(
        '#type' => 'textarea',
        '#title' => t('Class config'),
        '#default_value' => $default_value,
        '#required' => TRUE,
        '#description' => t('Available columns and element ranges for this layout. Format: columns|ranges. Supports * and comma separated values.'),
        '#required' => TRUE,
        // @todo support isolated and floated grids.
      );
    }
  }

  $form['actions']['#type'] = 'actions';

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}


/**
 * Ensure unique layout identifier
 */
function fancy_grid_check_identifier($identifier) {
  $layouts = _fancy_grid_get_layouts();
  foreach ($layouts as $layout) {
    if ($layout['identifier'] == $identifier) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Implements hook_submit().
 */
function fancy_grid_admin_submit($form, &$form_state) {

  // Set config variables
  variable_set('fancy_grid_grid_width', $form_state['values']['grid_width']);
  variable_set('fancy_grid_grid_max_width', $form_state['values']['grid_max_width']);
  variable_set('fancy_grid_compile', $form_state['values']['compile']);
  variable_set('fancy_grid_compile_webservice', $form_state['values']['compile_webservice']);
  variable_set('fancy_grid_breakpoint_group', $form_state['values']['breakpoint_group']);

  unset($form_state['values']['layouts']['layouts__active_tab']);

  // Store the layouts in the configuration array.
  $layouts = array();
  foreach ($form_state['values']['layouts'] as $breakpoint_key => $layout) {
    if (! is_array($layout))
      continue;
    $layouts[$breakpoint_key] = array(
      'breakpoint' => $layout['breakpoint'],
      'identifier' => $layout['identifier'],
      'columns' => $layout['columns'],
      'gutters' => $layout['gutters'],
      'config' => $layout['config'],
    );
  }

  variable_set('fancy_grid_layouts', $layouts);

  $force_compile = FALSE;
  if ($form_state['values']['force_recompile']) {
    $force_compile = TRUE;
  }

  _fancy_grid_write_config_file($force_compile);
}

/**
 * Returns media field wrapper for ajax request
 */
function fancy_grid_admin_ajax($form, $form_state) {
  return $form['layouts'];
}

/**
 * Proposes a breakpoint sass module configuration based on a given Drupal breakpoint.
 * @param $breakpoint
 * @return string breakpoint module configuration proposal
 */
function _fancy_grid_layout_default_breakpoint_config($breakpoint) {
  $breakpoint_config = array();
  $query = $breakpoint->breakpoint;

  // Reduce media query lists to one item
  if ($list = explode(',', $query)) {
    $query = reset($list);
  }

  // Prepare query for regex
  $query = strtolower(str_replace(' ', '', $query));

  // Look for a media type.
  $pattern = "/(not|only|)(all|aural|braille|handheld|print|projection|screen|tty|tv|embossed)/";

  preg_match($pattern, $query, $match);

  if ($match) {
    $query = str_replace(array_shift($match), '', $query);
    $breakpoint_config[] = implode(' ', $match);
  }

  // Look for feature pairs
  $features = array('width', 'height', 'device-width', 'device-height', 'aspect-ratio', 'device-aspect-ratio', 'color', 'color-index', 'monochrome', 'resolution');
  foreach ($features as $feature) {
    $pattern = '/\(min-' . $feature . ':([^)]+)+\).*\(max-' . $feature . ':([^)]+)+\)/';
    preg_match($pattern, $query, $match);

    if ($match) {
      $query = str_replace(array_shift($match), '', $query);
      $breakpoint_config[] = '(' . $feature . ' ' . implode(' ', $match) . ')';
    }
  }

  // Look for single features
  $features = array('width', 'height', 'device-width', 'device-height', 'aspect-ratio', 'device-aspect-ratio', 'color', 'color-index', 'monochrome', 'resolution', 'scan', 'grid', 'orientation');
  foreach ($features as $feature) {
    $pattern = '/\(((?:min-|max-|)' . $feature . '):([^)]+)\)/';
    preg_match($pattern, $query, $match);

    if ($match) {
      $query = str_replace(array_shift($match), '', $query);
      $breakpoint_config[] = '(' . implode(' ', $match) . ')';
    }
  }
  return implode(' ', $breakpoint_config);
}

/**
 * Proposes an identifier for a unconfigured fancy grid layout
 * @param $breakpoint
 * @return string identifier
 */
function _fancy_grid_layout_default_identifier($breakpoint) {
  $machine_name = $breakpoint->machine_name;
  $last_dot = strrpos($machine_name, '.');
  $name = substr($machine_name, $last_dot + 1);
  $identifier = str_replace('_', '-', $name);
  return $identifier;
}