<?php
/**
 * @file
 * ddclover.webhook.inc
 */

/**
 * Implements hook_webhook_default_config().
 */
function ddclover_webhook_default_config() {
  $export = array();

  $webhook = new stdClass();
  $webhook->disabled = FALSE; /* Edit this to true to make a default webhook disabled initially */
  $webhook->api_version = 1;
  $webhook->whid = '8';
  $webhook->title = 'DD Clover Listener';
  $webhook->machine_name = 'ddclovercloverlistener';
  $webhook->description = 'This is a simple web hook for listening to POST requests from Clover.';
  $webhook->unserializer = 'json';
  $webhook->processor = 'cloverlog';
  $webhook->config = '';
  $webhook->enabled = TRUE;
  $export['ddclovercloverlistener'] = $webhook;

  return $export;
}
