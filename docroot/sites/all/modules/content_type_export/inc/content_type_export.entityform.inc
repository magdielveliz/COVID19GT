<?php
/**
 * @file
 * Functionality to export entity form types and attached fields to code.
 */

/**
 * Attempts to output PHP code which can re-create a entityform_type.
 */
function content_type_export_entityform($entityform_type) {

  $content = array();
  $content['bundle_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Entityform type',
    '#value' => content_type_export_dump_entityform_type_as_code($entityform_type),
    '#resizable' => TRUE,
  );
  $content['fields_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Fields',
    '#value' => content_type_export_dump_fields_as_code($entityform_type->type, 'entityform'),
    '#resizable' => TRUE,
  );
  $content['instances_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Instances',
    '#value' => content_type_export_dump_instances_as_code($entityform_type->type, 'entityform'),
    '#resizable' => TRUE,
  );
  $content['uninstall'] = array(
    '#type' => 'textarea',
    '#title' => 'Uninstall code',
    '#value' => content_type_export_entity_form_uninstall_code($entityform_type->type),
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
 * Export the basic properties needed to create a new entityform_type.
 */
function content_type_export_dump_entityform_type_as_code($entityform_type) {

  $entityform_type = (array) $entityform_type;

  // Remove private properties.
  foreach ($entityform_type as $k => $v) {
    if (ord($k) === 0) {
      unset($entityform_type[$k]);
    }
  }

  $export = '$entityform_type = entity_create(\'entityform_type\', ' . content_type_export_var_export_reformat((array) $entityform_type) . ');
  if (!entityform_type_load(\'' . $entityform_type['type'] . '\')) {
    entityform_type_save($entityform_type);
  }';
  return $export;
}

/**
 * Generate code for the uninstall hook.
 */
function content_type_export_entity_form_uninstall_code($type) {
  $entity_type = 'entityform';
  $instances = field_info_instances($entity_type, $type);
  $fields = '\'' . implode('\', \'', array_keys($instances)) . '\'';

  $export = <<<PHP
  \$fields = array($fields);
  foreach (\$fields as \$field) {
    field_delete_instance(\$field);
  }
  \$type = entityform_type_load('$type');
  entityform_type_delete(\$type);
PHP;

  return $export;
}
