<?php
/**
 * @file
 * Compoxures
 *
 * Content type enable us to place the compoxure as pane
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Compoxure'),
  'description' => t('Compoxure.'),

  // 'single' => TRUE means has no subtypes.
  'single' => FALSE,
  // Constructor.
  'content_types' => array('Compoxure'),
  // Name of a function which will render the block.
  'render callback' => 'compoxure_content_type_render',
  // The default context.
  'defaults' => array(),
  // This explicitly declares the config form. Without this line, the func
  // would be ctools_plugin_example_no_context_content_type_edit_form.
  'edit form' => 'compoxure_content_type_edit_form',

  // Icon goes in the directory with the content type.
  'icon' => 'compoxure_icon.png',
  'category' => array(t('Compoxure'), -9),
  'content types' => 'compoxure_content_type_content_types',
  // For admin UI display.
  'admin title' => 'compoxure_content_type_render_admin_title',
  'admin info' => 'compoxure_content_type_render_admin_info',
);

/**
 * Return all Compoxures available as subtypes.
 */
function compoxure_content_type_content_types($plugin) {
  $types = &drupal_static('compoxure_subtypes', array());
  if (empty($types)) {
    // Include the ctools helper 'export'.
    ctools_include('export');
    $compoxures = ctools_export_crud_load_all('compoxure');
    foreach ($compoxures as $key => $compoxure) {
      $types[$key] = array(
        'title' => t('Compoxure: @compoxure_title (@machine_name)', array(
          '@compoxure_title' => t($compoxure->admin_title),
          '@machine_name' => $compoxure->name,
        )),
        'icon' => 'compoxure_icon.png',
        'description' => ($compoxure->title) ? $compoxure->title : NULL,
        'category' => t('Compoxure'),
        'name' => $compoxure->name,
      );
    }
  }

  // @todo A more persistent cache? Not even sure if drupal_static is useful.
  return $types;
}


/**
 * Edit form callback for the content type.
 */
function compoxure_content_type_edit_form($form, &$form_state) {
  // Get the form state i.e. configuration values to load the default values!
  $conf = $form_state['conf'];

  // Include the ctools helper 'export'.
  ctools_include('export');
  if (!isset($form_state['subtype']) || $form_state['subtype_name'] == 'compoxure') {
    // Loads all the 'compoxure' those are there in database.
    $options = ctools_export_crud_load_all('compoxure');

    // Create an option array.
    $compoxure_options = array();
    foreach ($options as $key => $value) {
      $title = ($value->title) ? $value->title : $value->admin_title;
      $compoxure_options[$key] = $title;
    }

    if (!count($compoxure_options)) {
      drupal_set_message(t('There are no "Compoxures" available.'), 'error');
    }

    // Form element : Select compoxure.
    $form['compoxure'] = array(
      '#type' => 'select',
      '#title' => t('Compoxures'),
      '#default_value' => $conf['compoxure'],
      '#options' => $compoxure_options,
      '#description' => t('Please select a compoxure.'),
    );
  }
  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function compoxure_content_type_edit_form_submit(&$form, &$form_state) {
  // Remove internal Form API values.
  form_state_values_clean($form_state);
  $form_state['conf'] = $form_state['values'];
}

/**
 * Render the compoxure content type.
 *
 * @param string $subtype
 *   Used to indicate the specific compoxure that should be used.
 * @param array $conf
 *   Ctools configuration array for this content type.
 * @param array $panel_args
 *   Arguments for the panel where this is being rendered.
 * @param array $context
 *   Argumnets for the context under which this is being rendered.
 */
function compoxure_content_type_render($subtype, $conf, $panel_args, &$context) {
  if (empty($conf['compoxure']) && $subtype != 'compoxure') {
    // Subtype passed in actually specifies the Compoxure we should be using.
    $conf['compoxure'] = $subtype;
  }
  // Get the compoxure.
  $compoxure = ctools_export_crud_load('compoxure', $conf['compoxure']);

  // If the compoxure is not found then return nothing.
  if (empty($compoxure)) {
    return '';
  }
  // If the compoxure is disabled then return nothing.
  elseif (isset($compoxure->disabled) && $compoxure->disabled) {
    return '';
  }

  // Get the revised data!
  $compoxure_revision = compoxure_load($conf['compoxure']);

  // If we don't have content to show and the viewing user does not have rights
  // to do operations on this compoxure then return nothing.
  if (empty($compoxure_revision->content)
    && !(user_access('manage compoxure') || user_access('administer compoxure'))
  ) {
    return '';
  }

  // Prepare array for theme!
  $variable['rid'] = $compoxure_revision->rid;
  $variable['name'] = $compoxure->name;

  $variable['content'] = check_markup($compoxure_revision->content, $compoxure_revision->content_format);
  $node = !empty($context->data) ? $context->data : FALSE;
  $token_conf = !empty($node) ? array('node' => $node) : array();

  if (module_exists('token') && !empty($variable['content'])) {
    $variable['content'] = token_replace($variable['content'], $token_conf);
  }

  $block = new stdClass();

  if (isset($conf['hide_title']) && $conf['hide_title'] == 1) {
    $block->title = '';
  }
  else {
    $block->title = check_plain(($compoxure_revision->rid) ? $compoxure_revision->title_revision : $compoxure->title);
  }

  if (module_exists('token') && !empty($block->title)) {
    $block->title = token_replace($block->title, $token_conf);
  }

  $block->content = theme('compoxure', $variable);

  return $block;
}

/**
 * Admin title callback for panel panes.
 */
function compoxure_content_type_render_admin_title($subtype, $conf, $contexts) {
  $title = 'Compoxure';
  if (!empty($conf)) {
    if (empty($conf['compoxure']) && $subtype != 'compoxure') {
      // Subtype passed in actually specifies the Compoxure we should be using.
      $conf['compoxure'] = $subtype;
    }
    // Get the compoxure.
    $compoxure = ctools_export_crud_load('compoxure', $conf['compoxure']);

    if ($compoxure) {
      $title = t('Compoxure: @compoxure_title (@machine_name)', array(
        '@compoxure_title' => t($compoxure->admin_title),
        '@machine_name' => $compoxure->name,
      ));
    }
    else {
      $title = t('Deleted/missing compoxure @name', array('@name' => $subtype));
    }
  }
  return $title;
}

/**
 * Admin info callback for panel panes.
 */
function compoxure_content_type_render_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {

    if (empty($conf['compoxure']) && $subtype != 'compoxure') {
      // Subtype passed in actually specifies the Compoxure we should be using.
      $conf['compoxure'] = $subtype;
    }
    // Get the compoxure.
    $compoxure = ctools_export_crud_load('compoxure', $conf['compoxure']);

    $node = !empty($context->data) ? $context->data : FALSE;
    $token_conf = !empty($node) ? array('node' => $node) : array();

    if ($compoxure) {
      $title = ($conf['override_title'] ? $conf['override_title_text'] : $compoxure->title);
      if (empty($block->title)) {
        $block->title = 'No title';
      }

      if (module_exists('token') && !empty($title)) {
        $title = token_replace($title, $token_conf);
      }

      $compoxure_content = compoxure_load($compoxure->name);
      $content = check_markup($compoxure_content->content, $compoxure_content->content_format);

      if (module_exists('token') && !empty($content)) {
        $content = token_replace($content, $token_conf);
      }
    }
    else {
      $title = t('Deleted/missing compoxure @name', array('@name' => $subtype));
      $content = '';
    }
    $block = new stdClass();
    $block->title = $title;
    $block->content = $content;
    return $block;
  }

  return FALSE;
}
