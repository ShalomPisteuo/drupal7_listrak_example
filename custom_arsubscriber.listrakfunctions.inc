<?php

/**
 * @file
 * Custom Subscriber Listrak Subscribe, Unsubscribe and Update Functions
 */

/**
 * Subscribe A Contact to a specified newsletter.
 */
function custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $data, $newsletter) {
  $client = listrak_request();
  
  // Setting form variables.
  $firstName = $data['firstname'];
  $lastName = $data['lastname'];
  $title = $data['title'];
  $companyName = $data['companyname'];
  $address = $data['address'];
  $city = $data['city'];
  $stateprovince = $data['stateprovince'];
  $country = $data['country'];
  $postalcode = $data['postalcode'];
  $phoneNumber = $data['phonenumber'];
  $email = $data['email'];
  
	$params = array(
    'WSContact' => array(
      'EmailAddress' => $email,
      'ListID' => $lid,
      'ContactProfileAttribute' => array(
        array(
          'AttributeID' => 269954,
          'Value' => $firstName
        ),
        array(
          'AttributeID' => 269955,
          'Value' => $lastName
        ),
        array(
          'AttributeID' => 269956,
          'Value' => $address
        ),
        array(
          'AttributeID' => 269958,
          'Value' => $city
        ),
        array(
          'AttributeID' => 269959,
          'Value' => $stateprovince
        ),
        array(
          'AttributeID' => 269960,
          'Value' => $postalcode
        ),
        array(
          'AttributeID' => 269961,
          'Value' => $companyName
        ),
        array(
          'AttributeID' => 269962,
          'Value' => $title
        ),
        array(
          'AttributeID' => 269963,
          'Value' => $phoneNumber
        ),
        array(
          'AttributeID' => 338797,
          'Value' => $country
        ),
        // Subscribe to AR Today if no other newsletter is specified
        array(
          'AttributeID' => '',
          'Value' => 1
        ),
      )
    ), 
  	'ProfileUpdateType' => 'Overwrite',
  	'ExternalEventIDs' => "",
  	'OverrideUnsubscribe' => TRUE
	);
  
  switch ($newsletter) {
    // AR Canada Newsletter
    case '2373388':
      $params['WSContact']['ContactProfileAttribute'][10]['AttributeID'] = '2373388';
      break;
    
    // BHPH Newsletter
    case '2113468':
      $params['WSContact']['ContactProfileAttribute'][10]['AttributeID'] = '2113468';
      break;
    
    // SPNU Newsletter
    case '269969':
      $params['WSContact']['ContactProfileAttribute'][10]['AttributeID'] = '269969';
      break;
    
    // default to AR Today if no other newsletter is specified
    default:
      $params['WSContact']['ContactProfileAttribute'][10]['AttributeID'] = '269965';
  }
  
  // add the subscriber to the listrak list
  $response = $client->SetContact($params);
  //print $response;
  //drupal_set_message(t('test'), 'error');
  return $response->SetContactResult;
}

/**
 * Get newsletter subscriber info for an email address.
 */
function custom_arsubscriber_listrak_getcontact($lid, $email) {
  $client = listrak_request();
  
  if ($email != '') {
    // setting search parameters
    $params = array(
      'ListID' => $lid,
      'ContactEmailAddress' => $email
    );
    
    // gathering data from listrak
    $profile = $client->getContact($params);
    
    // gather existing subscription information
    if (!empty($profile->WSContact)) {
      // loop through the attributes array, and figure out what they are
      $attr_array = array();
      foreach($profile->WSContact->ContactProfileAttribute as $attrib){
        $attr_array[] = get_object_vars($attrib);
      }
      
      // @TODO find a better way to do this
      $usable_array = array();
      foreach($attr_array as $attribute){
        $usable_array[$attribute['AttributeID']] = $attribute['Value'];
      }
      
      unset($attr_array);
      
      // get all the subscription options
      $important_newsletter = array('269965','2373388','2373388','269967','269968','269969','269970','269971','545602','1480474');      
      
      $options_checked = array();
      foreach($important_newsletter as $key){
        if($usable_array[$key]) {
          $options_checked[] = $key;
        }
      }
      
      return $options_checked;
      
    } else {
    
      // if listrak does not know the provided email address
      return 'not found';
    }
  
  } else {
    
    // if user does not provide email address
    return 'none provided';
  }
}

/**
 * ???
 */
function custom_arsubscriber_listrak_manage($form_state) {
	$update_type = $form_state['values']['op'];
  $listID = variable_get('listrak_form_list_id');
  $email = $form_state['values']['email'];
  
  switch ($update_type) {
    
    // subscriber requests complete removal from everything
    case 'Unsubscribe me from Everything':
      $client = listrak_request();
      $params = array(
        'ListID' => $listID,
        'ContactEmailAddress' => $email,
      );
    
      $result = $client->UnsubscribeContact($params);   
      return $result->UnsubscribeContactResult;
    
    // subscriber requests to update their existing information
    case 'Save':
      $client = listrak_request();
      
      $params = array(
        'WSContact' => array(
          'EmailAddress' => $email,
          'ListID' => $listID,
          'ContactProfileAttribute' => array(
            // AutoRemarketing Today
            array(
              'AttributeID' => '269965',
              'Value' => $form_state['values']['subscribe'][269965],
            ),
            // Used Car Manager Weekly
            array(
              'AttributeID' => '269968',
              'Value' => $form_state['values']['subscribe'][269968],
            ),
            // Repossession & Recovery
            array(
              'AttributeID' => '545602',
              'Value' => $form_state['values']['subscribe'][545602],
            ),
            // Remarketer & Consignor
            array(
              'AttributeID' => '269967',
              'Value' => $form_state['values']['subscribe'][269967],
            ),
            // SubPrime News Update
            array(
              'AttributeID' => '269969',
              'Value' => $form_state['values']['subscribe'][269969],
            ),
            // AutoRemarketing Canada
            array(
              'AttributeID' => '2373388',
              'Value' => $form_state['values']['subscribe'][2373388],
            ),
            // AutoRemarketing Conference Updates
            array(
              'AttributeID' => '269970',
              'Value' => $form_state['values']['subscribe'][269970],
            ),
            // AR Canada Conference Updates
            array(
              'AttributeID' => '1480474',
              'Value' => $form_state['values']['subscribe'][1480474],
            ),
            // Special Offers from our advertising partners
            array(
              'AttributeID' => '269971',
              'Value' => $form_state['values']['subscribe'][269971],
            )
          )
        ),
        'ProfileUpdateType' => 'Overwrite',
        'ExternalEventIDs' => "",
        'OverrideUnsubscribe' => TRUE
      );
      
      $result = $client->SetContact($params);
      return $result->SetContactResult;
  }
    
}
