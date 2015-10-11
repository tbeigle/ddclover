<?php
/**
 * @file ddclover.inc
 */

/**
 * ddClover Class definition.
 */
class ddClover {
  var $client_id = DDCLOVER_CLIENT_ID;
  var $client_secret = DDCLOVER_CLIENT_SECRET;
  var $merchant_id;
  var $employee_id;
  var $access_token;
  
  function __construct($data = array()) {
    if (!empty($_GET['code'])) {
      $this->authorize($_GET['code']);
    }
    if (!empty($_GET['merchant_id']) || !empty($data['merchant_id'])) {
      $this->merchant_id = !empty($_GET['merchant_id']) ? $_GET['merchant_id'] : $data['merchant_id'];
    }
  }
  
  /**
   * Fetches an access token for a merchant.
   */
  public function authorize($code) {
    $url = 'https://www.clover.com/oauth/token';
    $options = array(
      'method' => 'GET',
      'data' => 'client_id=' . $this->client_id . '&client_secret=' . $this->client_secret . '&code=' . $code,
    );
    
    $response = drupal_http_request($url, $options);
    
    if (!empty($response->error)) {
      watchdog('ddclover', '<pre>!error</pre>', array('!error' => print_r($response->error, TRUE)), WATCHDOG_ERROR);
    }
    elseif (!empty($response->data)) {
      watchdog('ddclover', 'response data check: !data', array('!data' => $response->data));
    }
    else {
      watchdog('ddclover', 'Something went wrong trying to authorize merchant. Response:<pre>!response</pre>', array('!response' => print_r($response, TRUE)), WATCHDOG_ERROR);
    }
  }
} 
