<?php
/**
 * @file
 * ddclover.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ddclover_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "webhook" && $api == "webhook") {
    return array("version" => "1");
  }
}
