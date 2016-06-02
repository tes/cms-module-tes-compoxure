<?php

/**
 * Implements hook_views_data().
 */
function compoxure_views_data() {
  $data['views']['compoxure'] = array(
    'title' => t('Compxure'),
    'group' => 'Compoxure',
    'help' => t('Area handler to add compoxure string to a view.'),
    'area' => array(
      'handler' => 'compoxure_views_handler_area_compoxure_handler',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_pre_render().
 */
function compoxure_views_pre_render(&$view) {
  $empty = (count($view->result) > 0) ? FALSE : TRUE;

  // Check header first.
  $compoxure = compoxure_views_get_title_override_compoxure($view->header, $empty);
  // Check empty next if result set is empty.
  if (empty($compoxure) && $empty) {
    $compoxure = compoxure_views_get_title_override_compoxure($view->empty, $empty);
  }
  // Finally check footer.
  if (empty($compoxure)) {
    $compoxure = compoxure_views_get_title_override_compoxure($view->footer, $empty);
  }

  if ($compoxure) {
    $compoxure_data = compoxure_load($compoxure);
    $view->build_info['title'] = isset($compoxure_data->title_revision) ? $compoxure_data->title_revision : $compoxure_data->title;
  }
}

/**
 * Helper function to get the title override compoxure from an array of
 * area handlers.
 *
 * @param array $handlers
 *   An array of area handlers found inside the view object.
 * @param bool $empty
 *   Whether or not the view results are empty. Defaults to FALSE.
 *
 * @return string
 *   The machine name of the compoxure set to override the view title.
 */
function compoxure_views_get_title_override_compoxure($handlers, $empty = FALSE) {
  if (empty($handlers) || !is_array($handlers) || !array_key_exists('compoxure', $handlers)) {
    return '';
  }

  $compoxure = '';

  // Loop through handlers and look for the first
  // compoxure with override view title enabled.
  foreach($handlers as $key => $data) {
    if ($empty && !$data->options['empty']) {
      continue;
    }

    if (strpos($key, 'compoxure') === 0) {
      if ($data->options['override_view_title']) {
        $compoxure = $data->options['compoxure'];
        break;
      }
    }
  }

  return $compoxure;
}