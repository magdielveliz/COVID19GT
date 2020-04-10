<?php

function bean_type_export($bundle) {

  $bundles = field_info_bundles('bean');
  $bundle->info = $bundles[$bundle->type];

  $content = array();
  $content['bundle_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Content type',
    '#value' => content_type_export_dump_bean_bundle_as_code($bundle),
    '#resizable' => true,
  );
  $content['fields_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Fields',
    '#value' => content_type_export_dump_fields_as_code($bundle->type, 'bean'),
    '#resizable' => true,
  );
  $content['instances_export'] = array(
    '#type' => 'textarea',
    '#title' => 'Instances',
    '#value' => content_type_export_dump_instances_as_code($bundle->type, 'bean'),
    '#resizable' => true,
  );
  $content['uninstall'] = array(
    '#type' => 'textarea',
    '#title' => 'Uninstall code',
    '#value' => content_type_export_bean_bundle_uninstall_code($bundle),
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

function content_type_export_dump_bean_bundle_as_code($bundle) {

  $export = <<<PHP

\$plugin_info = array(
    'name' => '',
    'label' => '',
    'description' => '',
    'type' => '',
    'editable' => TRUE,
    'handler' => array(
      'info_file' => TRUE,
      'class' => 'BeanCustom',
      'parent' => 'bean',
      'file' => 'BeanCustom.class.php',
      'path' => drupal_get_path('module', 'bean_admin_ui') . '/plugins',
    ),
  );

  \$bundles = field_info_bundles('bean');
  if(empty(\$bundles['{$bundle->type}'])) {
    \$bean_type = new BeanCustom(\$plugin_info);
    \$bean_type->type = '{$bundle->type}';
    \$bean_type->setLabel('{$bundle->info['label']}');
    \$bean_type->setDescription('{$bundle->info['description']}');
    \$bean_type->save(TRUE);
    drupal_flush_all_caches();
  }

PHP;

  return $export;

}

function content_type_export_bean_bundle_uninstall_code($bundle) {
  $export = <<<PHP

  \$type = bean_load_plugin_class('{$bundle->type}');
  \$type->delete();

PHP;

  return $export;

}
