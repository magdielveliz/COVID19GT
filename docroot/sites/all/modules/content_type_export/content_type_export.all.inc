<?php

function content_type_export_all_entity_fields($form, &$form_state) {

  $default_values = variable_get('content_type_export_default_export_values', []);

  $entities_types = entity_get_info();

  // Loop entity types
  foreach($entities_types as $entity_type => $entities_type_details) {

    $default_value = isset($default_values[$entity_type]['value']) ? $default_values[$entity_type]['value'] : FALSE;

    $form[$entity_type] = array(
      '#type' => 'fieldset',
      '#title' => $entity_type,
      '#collapsible' => TRUE,
      '#collapsed' => !$default_value,
      '#tree' => TRUE,
    );

    $form[$entity_type]['include_in_export'] = array(
      '#title' => t('Include this entity type in export?'),
      '#type' => 'checkbox',
      '#default_value' => $default_value,
    );

    $form[$entity_type]['bundles'] = array(
      '#type' => 'container',
    );

    $form[$entity_type]['bundles']['label']['#markup'] = '<h3>' . t('Bundles') . '</h3>';

    // Loop bundles
    foreach($entities_type_details['bundles'] as $bundle_name => $bundle_details) {

      $bundle_default = isset($default_values[$entity_type]['bundles'][$bundle_name]) ? $default_values[$entity_type]['bundles'][$bundle_name] : FALSE;

      $form[$entity_type]['bundles'][$bundle_name] = array(
        '#type' => 'fieldset',
        '#title' => $bundle_name,
        '#collapsible' => TRUE,
        '#collapsed' => !$bundle_default,
        '#tree' => TRUE,
      );

      $form[$entity_type]['bundles'][$bundle_name]['include_in_export'] = array(
        '#title' => t('Include this bundle in export?'),
        '#type' => 'checkbox',
        '#default_value' => $bundle_default,
      );

      $form[$entity_type]['bundles'][$bundle_name]['field_defs'] = array(
        '#type' => 'fieldset',
        '#title' => t('Field definitions'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form[$entity_type]['bundles'][$bundle_name]['field_defs']['fields'] = array(
        '#type' => 'textarea',
        '#title' => 'Fields',
        '#value' => content_type_export_dump_fields_as_code($bundle_name, $entity_type),
        '#resizable' => TRUE,
      );

      $form[$entity_type]['bundles'][$bundle_name]['field_defs']['instances'] = array(
        '#type' => 'textarea',
        '#title' => 'Instances',
        '#value' => content_type_export_dump_instances_as_code($bundle_name, $entity_type),
        '#resizable' => TRUE,
      );

    }

  }

  $form['export_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to export to'),
    '#default_value' => DRUPAL_ROOT . '/profiles/taronga_zoo/inc/fields',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;

}

function content_type_export_all_entity_fields_submit($form, &$form_state) {

  $default_values = variable_get('content_type_export_default_export_values', []);

  form_state_values_clean($form_state);

  $values =& $form_state['values'];

  $export_path = rtrim($values['export_path'], '/');

  unset($values['export_path']);

  foreach($values as $entity_name => $entity_details) {

    $default_values[$entity_name]['value'] = $entity_details['include_in_export'];

    if($entity_details['include_in_export'] == 1) {

      // Check entity folder exists
      $entity_folder = $export_path . '/' . $entity_name;
      if(!file_exists($entity_folder)) {
        mkdir($entity_folder);
      }

      foreach($entity_details['bundles'] as $bundle_name => $bundle_details) {

        $default_values[$entity_name]['bundles'][$bundle_name] = $bundle_details['include_in_export'];

        if($bundle_details['include_in_export'] == 1) {
          $file = $entity_folder . '/' . $bundle_name . '.inc';
          $contents = '<?php' . PHP_EOL . PHP_EOL . $bundle_details['field_defs']['fields'] . PHP_EOL . PHP_EOL . $bundle_details['field_defs']['instances'];
          file_put_contents($file, $contents);

        }
      }

    }
  }

  variable_set('content_type_export_default_export_values', $default_values);

}