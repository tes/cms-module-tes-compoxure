<?php
/**
 * @file
 * compoxure_ctools_export_ui.inc
 *
 * Define this Export UI plugin
 */

$plugin = array(
  'schema' => 'compoxure',
  // As defined in hook_schema().
  'access' => 'administer compoxure',
  // Define a permission users must have to access these pages.
  'create access' => 'administer compoxure',
  'manage access' => 'manage compoxure',
  'delete access' => 'administer compoxure',

  'export' => array(
    'admin_title' => "admin_title",
  ),

  // Define the menu item.
  'menu' => array(
    'menu item' => 'compoxure',
    'menu title' => 'Compoxure',
    'menu description' => 'Create a compoxure that will be used as a pane.',
    'items' => array(
      'revision' => array(
        'path' => 'list/%ctools_export_ui/revision',
        'title' => 'Enable',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('compoxure_ctools_export_ui', 'revision', 4),
        'load arguments' => array('compoxure_ctools_export_ui', 6),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array(
          'compoxure_ctools_export_ui',
          'revision',
          4
        ),
        'type' => MENU_CALLBACK,
      ),
      'revertto' => array(
        'path' => 'list/%ctools_export_ui/revision/%/revertto',
        'title' => 'Revert to version',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('compoxure_ctools_export_ui', 'revertto', 4),
        'load arguments' => array('compoxure_ctools_export_ui', 6),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array(
          'compoxure_ctools_export_ui',
          'revertto',
          4
        ),
        'type' => MENU_CALLBACK,
      ),

      'view' => array(
        'path' => 'list/%ctools_export_ui/revision/%/view',
        'title' => 'Revert to version',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('compoxure_ctools_export_ui', 'view', 4),
        'load arguments' => array('compoxure_ctools_export_ui', 6),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array('compoxure_ctools_export_ui', 'view', 4),
        'type' => MENU_CALLBACK,
      ),
      'version-delete' => array(
        'path' => 'list/%ctools_export_ui/revision/%/delete',
        'title' => 'Delete revision',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array(
          'compoxure_ctools_export_ui',
          'version_delete',
          4
        ),
        'load arguments' => array('compoxure_ctools_export_ui', 6),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array(
          'compoxure_ctools_export_ui',
          'version_delete',
          4
        ),
        'type' => MENU_CALLBACK,
      ),
    ),
  ),

  'allowed operations' => array(
    'edit' => array('title' => t('Edit')),
    'enable' => array('title' => t('Enable'), 'ajax' => TRUE, 'token' => TRUE),
    'disable' => array(
      'title' => t('Disable'),
      'ajax' => TRUE,
      'token' => TRUE
    ),
    'revert' => array('title' => t('Revert')),
    'delete' => array('title' => t('Delete')),
    'clone' => array('title' => t('Clone')),
    'import' => array('title' => t('Import')),
    'export' => array('title' => t('Export')),
    'revision' => array('title' => t('Revisions')),
  ),

  // Define user interface texts.
  'title singular' => t('Compoxure'),
  'title plural' => t('Compoxures'),
  'title singular proper' => t('Compoxure'),
  'title plural proper' => t('Compoxures'),

  'handler' => 'compoxure_ctools_export_ui',
);

