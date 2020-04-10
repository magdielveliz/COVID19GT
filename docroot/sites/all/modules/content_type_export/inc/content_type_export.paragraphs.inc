<?php

function paragraphs_type_export_field($bundle) {

  $content = array();
  $content['bundle_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Content type',
    '#value' => content_type_export_dump_paragraphs_bundle_as_code($bundle),
    '#resizable' => true,
  );
  $content['fields_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Fields',
    '#value' => content_type_export_dump_fields_as_code($bundle->bundle, 'paragraphs_item'),
    '#resizable' => true,
  );
  $content['instances_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Instances',
    '#value' => content_type_export_dump_instances_as_code($bundle->bundle, 'paragraphs_item'),
    '#resizable' => true,
  );
  $content['uninstall'] = array(
    '#type' => 'textarea',
    '#title' => 'Uninstall code',
    '#value' => content_type_export_paragraphs_bundle_uninstall_code($bundle),
    '#resizable' => true,
  );
  $content['whole_install_file'] = array(
    '#type' => 'textarea',
    '#title' => 'The whole module.install',
    '#value' => content_type_export_install_file($content),
    '#resizable' => true,
    '#attributes' => array(
      'style' => 'height: 500px'
    ),
  );
  return $content;

}

/**
 * Export the basic properties needed to create a new taxonomy vocabulary in a module.
 */
function content_type_export_dump_paragraphs_bundle_as_code($bundle) {

  $export = "\$bundle = new stdClass();
\$bundle->name = '{$bundle->name}';
\$bundle->bundle = '{$bundle->bundle}';
\$bundle->locked = {$bundle->locked};

if(!paragraphs_bundle_load(\$bundle->bundle)) {
  paragraphs_bundle_save(\$bundle);
}";
  return $export;
}

function content_type_export_paragraphs_bundle_uninstall_code($bundle) {

  $export = <<<PHP
\$bundle = paragraphs_bundle_load('{$bundle->bundle}');
if(\$bundle) {
  paragraphs_bundle_delete(\bundle->vid);
}
PHP;
  return $export;
}