<?php
/**
 * @file
 * Provides a handler for compoxure fragments
 */

class compoxure_views_handler_area_compoxure_handler extends views_handler_area {
  public function option_definition() {
    $options = parent::option_definition();

    // Add defaults.
    $options['override_view_title'] = array('default' => 0);
    $options['compoxure'] = array('default' => '');
    $options['classes'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Include the ctools helper 'export'.
    ctools_include('export');
    // Load all the compoxures that are in the database.
    $options = ctools_export_crud_load_all('compoxure');
    // Create options array for form field.
    $compoxure_options = array();
    foreach ($options as $key => $value) {
      $title = ($value->title) ? $value->title : $value->admin_title;
      $compoxure_options[$key] = $title;
    }

    if (!count($compoxure_options)) {
      drupal_set_message(t('There are no compoxures available.'), 'error');
    }

    $form['override_view_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override view title'),
      '#description' => t('Check this box if you want the compoxure title to take the place of the view title in display. Note if you enable this setting on more than one compoxure displayed with this view, the first compoxure\'s title will be used.'),
      '#default_value' => $this->options['override_view_title']
    );

    $form['compoxure'] = array(
      '#type' => 'select',
      '#title' => t('Compoxure'),
      '#default_value' => $this->options['compoxure'],
      '#options' => $compoxure_options,
      '#description' => t('Please select a compoxure snippet.')
    );

    $form['classes'] = array(
      '#type' => 'textfield',
      '#title' => t('Additional classes'),
      '#description' => t('Classes to add to the compoxure wrapper. You may enter more than one, separated by spaces.'),
      '#default_value' => $this->options['classes']
    );

    return $form;
  }

  public function render($empty = FALSE) {
    $output = '';
    if (empty($this->options['compoxure']) || ($empty && !$this->options['empty'])) {
      return $output;
    }

    // Load compoxure and prepare output.
    $compoxure = compoxure_load($this->options['compoxure']);
    $classes = !empty($this->options['classes']) ? check_plain($this->options['classes']) : '';
    $vars = array();
    $vars['title'] = '';
    $vars['name'] = $compoxure->name;
    $vars['content'] = $compoxure->content;

    // Add title to variables array for theming only if it isn't being
    // used to override the view title.
    if (!$this->options['override_view_title']) {
      $vars['title'] = isset($compoxure->title_revision) ? $compoxure->title_revision : $compoxure->title;
    }

    $output .= theme('compoxure', $vars);

    return $output;
  }
}

