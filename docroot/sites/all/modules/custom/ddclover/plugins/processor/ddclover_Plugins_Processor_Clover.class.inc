<?php

/**
 * @file
 * Plugin class for handling post data from Clover.
 */

/**
 * Webhook Processor Clover logger class.
 *
 * Records Clover actions in watchdog log.
 */
class ddclover_Plugins_Processor_Clover implements Webhook_Plugins_Processor_Interface {

  /**
   * Configuration form.
   */
  public function config_form() {
    // No configuration needed.
    return array();
  }

  /**
   * Processes data.
   */
  public function process(stdClass $data) {
    $vars = array(
      '!cloverdata' => print_r($data, TRUE),
    );
    
    $message = 'Data received from clover:<pre>!cloverdata</pre>';
    return watchdog($message, $vars, WATCHDOG_INFO);
  }
}
