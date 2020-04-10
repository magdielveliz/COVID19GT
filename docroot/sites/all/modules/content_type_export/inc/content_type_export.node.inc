<?php
/**
 * @file
 * Functionality to export node bundles and attached fields to code.
 */

/**
 * Attempts to output PHP code which can re-create a content type.
 */
function content_type_export_node_task($bundle) {
  $content = array();
  $content['bundle_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Content type',
    '#value' => content_type_export_dump_bundle_as_code($bundle),
    '#resizable' => TRUE,
  );
  $content['fields_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Fields',
    '#value' => content_type_export_dump_fields_as_code($bundle->type),
    '#resizable' => TRUE,
  );
  $content['instances_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Instances',
    '#value' => content_type_export_dump_instances_as_code($bundle->type),
    '#resizable' => TRUE,
  );
  $content['uninstall'] = array(
    '#type' => 'textarea',
    '#title' => 'Uninstall code',
    '#value' => content_type_export_node_uninstall_code($bundle->type),
    '#resizable' => TRUE,
  );
  $content['whole_install_file'] = array(
    '#type' => 'textarea',
    '#title' => 'The whole module.install',
    '#value' => content_type_export_install_file($content),
    '#resizable' => TRUE,
    '#attributes' => array(
      'style' => 'height: 500px',
    ),
  );
  return $content;
}

/**
 * Export the basic properties needed to create a new content type in a module.
 */
function content_type_export_dump_bundle_as_code($bundle) {
  // Convert to an array for easy editing.
  $bundle = (array) $bundle;
  $export = '';

  // Unset default properties.
  unset($bundle['module']);
  unset($bundle['help']);
  unset($bundle['has_title']);
  unset($bundle['title_label']);
  unset($bundle['disabled']);
  unset($bundle['modified']);
  unset($bundle['orig_type']);
  unset($bundle['disabled_changed']);
  unset($bundle['locked']);

  // Output PHP code to create the bundle.
  $bundle_export = '$bundle = ' . content_type_export_var_export_reformat($bundle) . ';';
  $export .= <<<RAW
// {$bundle['name']} content type
$bundle_export

\$bundle = node_type_set_defaults(\$bundle);
node_type_save(\$bundle);
node_add_body_field(\$bundle);

RAW;

  return $export;
}

/**
 * Generate code for the uninstall hook.
 */
function content_type_export_node_uninstall_code($bundle) {
  $entity_type = 'node';
  $export = <<<PHP
  field_attach_delete_bundle('$entity_type', '$bundle');
  node_type_delete('$bundle');
PHP;
  return $export;
}
