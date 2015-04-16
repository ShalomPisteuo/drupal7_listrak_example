<?php

/**
 * @file
 * An listrak Soap Client class for interfacing with the listrak SOAP Interface.
 *
 * @author Tom Kirkpatrick (mrfelton), www.systemseed.com
 * @author port to D7 David Morrison
 */

/**
 * The listrak Client Class enables access to the SOAP API.
 *
 * The listrak SOAP API requires a token to be passed at each call. The token
 * expires after about an hour of time and then needs to be refreshed. This
 * class takes care of automating the token usage. Only when a token fails to
 * be refreshed is the token error thrown.
 */
class ListrakClient extends SoapClient {

  /**
   * The SoapClient controller object.
   */
  protected $controller = NULL;

  /**
   * Initialize the listrak Soap Client.
   *
   * @param string $endpoint
   *   The endpoint we are connecting to.
   * @param array $options
   *   - siteid: The site id for the listrak site being accessed.
   *   - secid: The secure id for the site being accessed.
   *   - token: An listrak Token. (optional)
   *   - options: The SoapClient options.
   *     @link http://php.net/SoapClient
   */
  public function __construct($endpoint, array $options = array()) {
    $this->user = $options['user'];
    $this->pass = $options['pass'];

    $auth = array(
      'UserName' => $this->user,
      'Password' => $this->pass,
    );
    $authvalues = new SoapVar($auth, SOAP_ENC_OBJECT);
    $headers[] = new SoapHeader("http://webservices.listrak.com/v31/", 'WSUser', $auth);

    // Initialize a SoapClient object to make the api calls.
    $this->controller = new SoapClient($endpoint, $options['options']);
    $this->controller->__setSoapHeaders($headers);
  }

  /**
   * Make a call to the listrak SOAP API. The name is the operation being preformed
   * on the interface.
   *
   * The required token is automatically added to the API call. If the token is
   * stale it will be refreshed and the call tried again. If a refresh fails an
   * error will be thrown.
   *
   * @link https://secure.accesslistrak.com/listrakcfwsv2/wsca.asmx
   * @link https://secure.accesslistrak.com/listrakcfwsv2/wscea.asmx
   *
   * @param $operation
   *   The operation being called on the API.
   * @param $arguments
   *   Arguments to pass on to the API.
   *
   * @todo Switch from returning FALSE to throwing Exceptions on error.
   */
  public function __call($operation, array $arguments) {
    $params = (isset($arguments[0]) && is_array($arguments[0])) ? $arguments[0] : array();

    // We have a listid, make a call to the api.
    if ($this->user != FALSE && $this->pass != FALSE) {
      try {
        $response = $this->controller->{$operation}($params);
        //dsm($this->controller->__getLastRequest());
        return $response;
      }
      catch (SoapFault $e) {
        //dsm($this->controller->__getLastRequest());
        watchdog('listrak', 'The listrak API reported the following error: %error', array('%error' => $e->getMessage()), WATCHDOG_ERROR);
        return FALSE;
      }
    }

    // Refreshing the token has already failed.
    return FALSE;
  }
  
}

