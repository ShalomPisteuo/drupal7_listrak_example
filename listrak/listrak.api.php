<?php

/*
 * @file
 * Hooks provided by the listrak module.
 *
 * @author Tom Kirkpatrick (mrfelton), www.systemseed.com
 * @author port to D7 David Morrison
 */

/**
 * Alter the options passed into the Listrak Client.
 *
 * @param array $options
 *   A multideminsional array passed into the Listrak Client.
 *   $options['options'] is the options passed into the SoapClient.
 */
function hook_listrak_client_options_alter(&$options) {
  $options['options']['soap_version'] = SOAP_1_1;
}

