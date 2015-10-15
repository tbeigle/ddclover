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
  var $merchant_id = '';
  var $employee_id = '';
  var $uid = 0;
  var $access_token = '';
  var $url = '';
  var $authorized = FALSE;
  
  function __construct($token) {
    if (!empty($_GET['code']) && empty($token) && empty($this->access_token) && !$this->authorized) {
      $this->authorize($_GET['code']);
    }
    
    if (!empty($_GET['merchant_id']) || !empty($data['merchant_id'])) {
      $this->merchant_id = !empty($_GET['merchant_id']) ? $_GET['merchant_id'] : $data['merchant_id'];
    }
    
    if (!empty($_GET['employee_id']) || !empty($data['employee_id'])) {
      $this->employee_id = !empty($_GET['employee_id']) ? $_GET['employee_id'] : $data['employee_id'];
    }
  }
  
  /**
   * Populates the object with data from the merchant table.
   */
  public function load_merchant($merchant_id) {
    $query = 'SELECT * FROM {ddclover_merchant} WHERE `merchant_id` = :merchant_id ORDER BY `merchant_id` LIMIT 1';
    $row = db_query($query, array(':merchant_id' => $merchant_id))->fetchObject();
    
    $this->merchant_id = $merchant_id;
    $this->access_token = $row->token;
    $this->url = $row->url;
  }
  
  /**
   * Fetches an access token for a merchant.
   */
  public function authorize($code) {
    $url = 'https://www.clover.com/oauth/token?client_id=' . $this->client_id . '&client_secret=' . $this->client_secret . '&code=' . $code;
    $options = array(
      'method' => 'GET',
    );
    
    $response = drupal_http_request($url, $options);
    
    if (!empty($response->error)) {
      watchdog('ddclover', '<pre>!error</pre>', array('!error' => print_r($response->error, TRUE)), WATCHDOG_ERROR);
    }
    elseif (!empty($response->data)) {
      $decoded = json_decode($response->data);
      $this->access_token = $decoded->access_token;
      $this->authorized = TRUE;
    }
    else {
      watchdog('ddclover', 'Something went wrong trying to authorize merchant. Response:<pre>!response</pre>', array('!response' => print_r($response, TRUE)), WATCHDOG_ERROR);
    }
  }
  
  /**
   * Saves a merchant to access token relationship.
   */
  public function save_merchant() {
    $inserted = db_insert('ddclover_merchant')
    ->fields(array(
      'merchant_id' => $this->merchant_id,
      'token' => $this->access_token,
      'url' => $this->url,
    ))
    ->execute();
  }
  
  /**
   * Saves a merchant to access token relationship.
   */
  public function save_employee($data) {
    // Add the user account
    $values = array(
      'mail' => $data['mail'],
      'name' => $data['mail'],
      'pass' => $data['pass'],
      'roles' => array(
        4,
      ),
    );
    
    $u = entity_create('user', array());
    $u->name = $data['mail'];
    $u->status = 1;
    $u->roles = drupal_map_assoc(array(4) + array(DRUPAL_AUTHENTICATED_RID));
    $u->init = $data['mail'];
    $u->mail = $data['mail'];
    require_once DRUPAL_ROOT . '/includes/password.inc';
    $u->pass = user_hash_password($data['pass']);
    entity_save('user', $u);
    
    if (empty($u->uid)) {
      watchdog('ddclover', 'user create problem. <pre>!u</pre>', array('!u' => print_r($u, TRUE)), WATCHDOG_ERROR);
      return;
    }
    
    $this->uid = $u->uid;
    
    // Save the employee record
    $inserted = db_insert('ddclover_employee')
    ->fields(array(
      'employee_id' => $this->employee_id,
      'merchant_id' => $this->merchant_id,
      'uid' => $this->uid,
    ))
    ->execute();
  }
} 
