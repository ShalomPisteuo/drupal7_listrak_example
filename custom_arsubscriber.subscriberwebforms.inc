<?php

/**
 * @file
 * Custom Subscriber Listrak Forms, Validation and Submit Logic
 */

/**
 * Creates a Custom AR Form.
 */
function custom_subscriber_ar_form($form,&$form_submit) {
	$form = array();
  
  $form['firstname'] = array(
		'#title' => t('Firstname'),
		'#type' => 'textfield',
		'#required' => TRUE,
  );
  $form['lastname'] = array(
    '#title' => t('Lastname'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['companyname'] = array(
    '#title' => t('Company name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['address'] = array(
    '#title' => t('Address'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['stateprovince'] = array(
    '#title' => t('State'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['country'] = array(
    '#title' => t('Country'),
    '#type' => 'textfield',
		'#default_value' => t('USA'),
		'#description' => t('This magazine is only available for delivery in the USA.'),
    '#required' => TRUE,
  );
  $form['postalcode'] = array(
    '#title' => t('Zip Code'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['phonenumber'] = array(
    '#title' => t('Phone number'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#title' => t('Email address'),
    '#type' => 'textfield',
    '#required' => TRUE,
		'#description' => t('Must be a valid email address'),
  );
  
  // Declaring our radios for subscription options
  $active = array(0 => t('Print only'), 1 => t('Digital only'), 2 => t('Print and Digital'));
  $form['subscribeoption'] = array(
		'#title' => t('Subscription options'),
	  '#type' => 'radios',
		'#multiple' => FALSE,
		'#default_value' => 2,
		'#options' => $active,
		'#description' => t('Note: Digital Edition subscribers will also receive our Auto Remarketing Today e-newsletter.'),
  );
  
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );
  
	return $form;
}

/**
 * Implements function_form_validate.
 *
 * Validates the AR subscriber form entries.
 */

function custom_subscriber_ar_form_validate($form, &$form_state) {

  $email = $form_state['values']['email'];
	$country = $form_state['values']['country'];

  // Check to see if user entered a properly formated email address
  if(valid_email_address($email) == 0) {
		form_set_error('email', t('Not a valid email address'));
  }
	
	// Check to make sure the user is located in the USA
	if($country != 'USA') {
		form_set_error('country', t('To subscribe to this magazine you must be located in the USA.'));
	}
}

/**
 * Implements drupal_form_submit.
 *
 * Decides what happens when the AR subscriber form submits.
 */
function custom_subscriber_ar_form_submit($form, &$form_state) {
	$subscribeOption = $form_state['values']['subscribeoption'];
	// get form submission data and pass them to listrak function
	$params = $form_state['values'];
	// automotive newsletter lists id
	$lid = variable_get('listrak_form_list_id');
	// AR Today newsletter list code
	$newsletter = 269965;
	// Email subscriber notification recepients
	$alerts = 'dmorrison@sacommunications.com, ejachym@cherokeepublishing.com, lmcgraw@sacherokee.com';
  
  switch ($subscribeOption) {
		// Print only
		case 0:
			// send a drupal mail with subscribers info to administrator
			$message = drupal_mail('custom_subscriber', 'ar subscriber', $alerts, language_default(), $params);
			
			// check to make sure the form submitted properly
			if(!empty($message['result'])) {
				drupal_set_message(t('You have been subscribed!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to send subscriber email notification.', WATCHDOG_WARNING);
			}
			break;
		
		// Digital only
		case 1:
			// connect to listrak and submit subscriber info
			$message = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
			
			// check to make sure the form submitted properly
			$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
			if(!empty($message) && in_array($message, $success)) {
				drupal_set_message(t('You have successfully subscribed to the Digital Edition!'), 'status');
			} else {
				//drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				$test = $message;
				drupal_set_message(t($test), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
			}
			break;
		
		// Print and Digital
		case 2:
			// send a drupal mail with subscribers info to administrator
			$message_mail = drupal_mail('custom_subscriber', 'ar subscriber', $alerts, language_default(), $params);
			// connect to listrak and submit subscriber info
			$message_listrak = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
			
			// check to make sure the form submitted properly
			$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
			if(!empty($message_listrak) && in_array($message_listrak, $success) && !empty($message_mail['result'])) {
				drupal_set_message(t('You have successfully subscribed to the Digital Edition and Print Edition!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
			}
			break;
  }
}

/**
 * Creates a Custom ARC Form.
 */
function custom_subscriber_arc_form($form,&$form_submit) {
  
  $form['firstname'] = array(
		'#title' => t('Firstname'),
		'#type' => 'textfield',
		'#required' => TRUE,
  );
  $form['lastname'] = array(
    '#title' => t('Lastname'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['companyname'] = array(
    '#title' => t('Company name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['address'] = array(
    '#title' => t('Address'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['stateprovince'] = array(
    '#title' => t('Province/State'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['country'] = array(
    '#title' => t('Country'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['postalcode'] = array(
    '#title' => t('Postal Code'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['phonenumber'] = array(
    '#title' => t('Phone number'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#title' => t('Email address'),
    '#type' => 'textfield',
    '#required' => TRUE,
		'#description' => t('Must be a valid email address'),
  );
  
  // Declaring our radios for subscription options
  $active = array(1 => t('Digital only'));
  $form['subscribeoption'] = array(
		'#title' => t('Subscription options'),
	  '#type' => 'radios',
		'#multiple' => FALSE,
		'#default_value' => 1,
		'#options' => $active,
		'#description' => t('Note: Digital Edition subscribers will also receive our Auto Remarketing Canada e-newsletter.'),
  );
  
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );
  
	return $form;
}

/**
 * Implements function_form_validate.
 *
 * Validates the ARC subscriber form entries.
 */

function custom_subscriber_arc_form_validate($form, &$form_state) {

  $email = $form_state['values']['email'];

  // Check to see if user entered a properly formated email address
  if(valid_email_address($email) == 0) {
		form_set_error('email', t('Not a valid email address'));
  }
}

/**
 * Implements drupal_form_submit.
 *
 * Decides what happens when the AR subscriber form submits.
 */
function custom_subscriber_arc_form_submit($form, &$form_state) {
	// get form submission data and pass them to listrak function
	$params = $form_state['values'];
	// automotive newsletter lists id
	$lid = variable_get('listrak_form_list_id');
	// AR Today newsletter list code
	$newsletter = 269966;

	// connect to listrak and submit subscriber info
	$message = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
	
	// check to make sure the form submitted properly
	$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
	if(!empty($message) && in_array($message, $success)) {
		drupal_set_message(t('You have successfully subscribed to the Digital Edition!'), 'status');
	} else {
		drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
		// log form submission errors
		watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
	}
}

/**
 * Creates a Custom BHPH Form.
 */
function custom_subscriber_bhph_form($form,&$form_submit) {
  
  $form['firstname'] = array(
		'#title' => t('Firstname'),
		'#type' => 'textfield',
		'#required' => TRUE,
  );
  $form['lastname'] = array(
    '#title' => t('Lastname'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['companyname'] = array(
    '#title' => t('Company name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['address'] = array(
    '#title' => t('Address'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['stateprovince'] = array(
    '#title' => t('State'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['country'] = array(
    '#title' => t('Country'),
    '#type' => 'textfield',
		'#default_value' => t('USA'),
		'#description' => t('This magazine is only available for delivery in the USA.'),
    '#required' => TRUE,
  );
  $form['postalcode'] = array(
    '#title' => t('Zip Code'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['phonenumber'] = array(
    '#title' => t('Phone number'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#title' => t('Email address'),
    '#type' => 'textfield',
    '#required' => TRUE,
		'#description' => t('Must be a valid email address'),
  );
  
  // Declaring our radios for subscription options
  $active = array(0 => t('Print only'), 1 => t('Digital only'), 2 => t('Print and Digital'));
  $form['subscribeoption'] = array(
		'#title' => t('Subscription options'),
	  '#type' => 'radios',
		'#multiple' => FALSE,
		'#default_value' => 2,
		'#options' => $active,
		'#description' => t('Note: Digital Edition subscribers will also receive our BHPH Report e-newsletter.'),
  );
  
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );
  
	return $form;
}

/**
 * Implements function_form_validate.
 *
 * Validates the BHPH subscriber form entries.
 */
function custom_subscriber_bhph_form_validate($form, &$form_state) {

  $email = $form_state['values']['email'];
	$country = $form_state['values']['country'];
  
	// Check to see if user entered a properly formated email address
  if(valid_email_address($email) == 0) {
		form_set_error('email', t('Not a valid email address'));
  }
	
	// Check to make sure the user is located in the USA
	if($country != 'USA') {
		form_set_error('country', t('To subscribe to this magazine you must be located in the USA.'));
	}
}

/**
 * Implements drupal_form_submit.
 *
 * Decides what happens when the AR subscriber form submits.
 */
function custom_subscriber_bhph_form_submit($form, &$form_state) {
	$subscribeOption = $form_state['values']['subscribeoption'];
	// get form submission data and pass them to listrak function
	$params = $form_state['values'];
	// automotive newsletter lists id
	$lid = variable_get('listrak_form_list_id');
	// AR Today newsletter list code
	$newsletter = 2113468;
	// Email subscriber notification recepients
	$alerts = 'dmorrison@sacommunications.com, ejachym@cherokeepublishing.com, lmcgraw@sacherokee.com';
  
  switch ($subscribeOption) {
		// Print only
		case 0:
			// send a drupal mail with subscribers info to administrator
			$message = drupal_mail('custom_subscriber', 'bhph subscriber', $alerts, language_default(), $params);
			
			// check to make sure the form submitted properly
			if(!empty($message['result'])) {
				drupal_set_message(t('You have been subscribed!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to send subscriber email notification.', WATCHDOG_WARNING);
			}
			break;
		
		// Digital only
		case 1:
			// connect to listrak and submit subscriber info
			$message = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
			
			// check to make sure the form submitted properly
			$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
			if(!empty($message) && in_array($message, $success)) {
				drupal_set_message(t('You have successfully subscribed to the Digital Edition!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
			}
			break;
		
		// Print and Digital
		case 2:
			// send a drupal mail with subscribers info to administrator
			$message_mail = drupal_mail('custom_subscriber', 'bhph subscriber', $alerts, language_default(), $params);
			// connect to listrak and submit subscriber info
			$message_listrak = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
			
			// check to make sure the form submitted properly
			$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
			if(!empty($message_listrak) && in_array($message_listrak, $success) && !empty($message_mail['result'])) {
				drupal_set_message(t('You have successfully subscribed to the Digital Edition and Print Edition!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
			}
			break;
  }
}

/**
 * Creates a Custom SP Form.
 */
function custom_subscriber_sp_form($form,&$form_submit) {
  
  $form['firstname'] = array(
		'#title' => t('Firstname'),
		'#type' => 'textfield',
		'#required' => TRUE,
  );
  $form['lastname'] = array(
    '#title' => t('Lastname'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['companyname'] = array(
    '#title' => t('Company name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['address'] = array(
    '#title' => t('Address'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['stateprovince'] = array(
    '#title' => t('State'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['country'] = array(
    '#title' => t('Country'),
    '#type' => 'textfield',
    '#default_value' => t('USA'),
    '#description' => t('This magazine is only available for delivery in the USA.'),
    '#required' => TRUE,
  );
  $form['postalcode'] = array(
    '#title' => t('Zip Code'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['phonenumber'] = array(
    '#title' => t('Phone number'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#title' => t('Email address'),
    '#type' => 'textfield',
    '#required' => TRUE,
		'#description' => t('Must be a valid email address'),
  );
  
  // Declaring our radios for subscription options
  $active = array(0 => t('Print only'), 1 => t('Digital only'), 2 => t('Print and Digital'));
  $form['subscribeoption'] = array(
		'#title' => t('Subscription options'),
	  '#type' => 'radios',
		'#multiple' => FALSE,
		'#default_value' => 2,
		'#options' => $active,
		'#description' => t('Note: Digital Edition subscribers will also receive our SubPrime Auto Finance News e-newsletter.'),
  );
  
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Implements function_form_validate.
 *
 * Validates the SP subscriber form entries.
 */

function custom_subscriber_sp_form_validate($form, &$form_state) {

  $email = $form_state['values']['email'];
	$country = $form_state['values']['country'];

  // Check to see if user entered a properly formated email address
  if(valid_email_address($email) == 0) {
		form_set_error('email', t('Not a valid email address'));
  }
	
	// Check to make sure the user is located in the USA
	if($country != 'USA') {
		form_set_error('country', t('To subscribe to this magazine you must be located in the USA.'));
	}
}

/**
 * Implements drupal_form_submit.
 *
 * Decides what happens when the AR subscriber form submits.
 */
function custom_subscriber_sp_form_submit($form, &$form_state) {
	$subscribeOption = $form_state['values']['subscribeoption'];
	// get form submission data and pass them to listrak function
	$params = $form_state['values'];
	// automotive newsletter lists id
	$lid = variable_get('listrak_form_list_id');
	// AR Today newsletter list code
	$newsletter = 269969;
	// Email subscriber notification recepients
	$alerts = 'dmorrison@sacommunications.com, ejachym@cherokeepublishing.com, lmcgraw@sacherokee.com';
  
  switch ($subscribeOption) {
		// Print only
		case 0:
			// send a drupal mail with subscribers info to administrator
			$message = drupal_mail('custom_subscriber', 'sp subscriber', $alerts, language_default(), $params);
			
			// check to make sure the form submitted properly
			if(!empty($message['result'])) {
				drupal_set_message(t('You have been subscribed!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to send subscriber email notification.', WATCHDOG_WARNING);
			}
			break;
		
		// Digital only
		case 1:
			// connect to listrak and submit subscriber info
			$message = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
			
			// check to make sure the form submitted properly
			$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
			if(!empty($message) && in_array($message, $success)) {
				drupal_set_message(t('You have successfully subscribed to the Digital Edition!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
			}
			break;
		
		// Print and Digital
		case 2:
			// send a drupal mail with subscribers info to administrator
			$message_mail = drupal_mail('custom_subscriber', 'sp subscriber', $alerts, language_default(), $params);
			// connect to listrak and submit subscriber info
			$message_listrak = custom_arsubscriber_listrak_setcontact($lid, $override=FALSE, $params, $newsletter);
			
			// check to make sure the form submitted properly
			$success = array('SuccessContactSubscribed', 'ContactAlreadySubscribedProfileUpdated');
			if(!empty($message_listrak) && in_array($message_listrak, $success) && !empty($message_mail['result'])) {
				drupal_set_message(t('You have successfully subscribed to the Digital Edition and Print Edition!'), 'status');
			} else {
				drupal_set_message(t('There was a problem trying to subscribe you.'), 'error');
				// log form submission errors
				watchdog('custom_subscriber', 'Failed to properly add subscriber to listrak.', WATCHDOG_WARNING);
			}
			break;
  }
}

/**
 * Creates a GENERAL newsletter subscriber form
 */
function custom_subscriber_manage_form() {
  
	$lid = variable_get('listrak_form_list_id');
	$email = (isset($_GET['email'])) ? $_GET['email'] : '';
	
	// get profile data from listrak
	$profile = custom_arsubscriber_listrak_getcontact($lid, $email);
	
	if($profile != 'none provided'){

    if($profile == 'not found'){
      drupal_set_message('We couldn\'t find that profile in our system. Please subscribe below.');
    }
    
    $form['description'] = array(
      '#value' => variable_get('listrak_form_success_message', '<p>Thank you for subscribing to the AutoRemarketing.com email newsletters.  We use the following information to bring our readers the most relevant &amp; up to date information available:</p>'),
    );
    $form['contactinfo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contact Information'),
    );
    $form['contactinfo']['old_email'] = array(
      '#type' => 'hidden',
      '#value' => $email, 
    );
    $form['contactinfo']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email Address'),
      '#default_value' => $email, 
      '#required' => TRUE,
      '#size' => 25,
    );
    $form['subscribe_group'] = array(
      '#type' => 'fieldset',
      '#title' => t('Subscription options:'),
    );
    $form['subscribe_group']['subscribe'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Newsletter options'),
      '#options' => array(
				'269965' => t('AutoRemarketing Today'), 
				'269968' => t('Used Car Manager Weekly'), 
				'545602' => t('Repossession &amp; Recovery'),
				'269967' => t('Remarketer &amp; Consignor'),
				'269969' => t('SubPrime News Update'),
				'269966' => t('AutoRemarketing Canada'),
				'269970' => t('AutoRemarketing Conference Updates'),
				'1480474' => t('AR Canada Conference Updates'),
				'269971' => t('Special Offers from our advertising partners'),
			),
      '#default_value' => $profile,
      '#required' => TRUE,
    );
    $form['subscribe_group']['submit_one'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['subscribe_group']['submit_two'] = array(
      '#type' => 'submit',
      '#value' => t('Unsubscribe me from Everything'),
    );
    $form['subscribe_group']['submit_two']['#attributes'] = array(
			'style' => 'margin-left: 20px'
		);
  
	} else {
    $form['description'] = array(
      '#value' => variable_get('listrak_form_success_message', '<p>Please enter your email into the form below to Manage your subscription:</p>'),
    );
    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email Address'),
      '#default_value' => $email, 
      '#required' => TRUE,
      '#size' => 25,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Manage my Subscription!'),
    );
  }
  
  return $form;
}


function custom_subscriber_manage_form_submit($form, &$form_state) {
	$update_type = $form_state['values']['op'];
	$email = $form_state['values']['email'];

	// if no email is provided, collect email and reload the page accessing variable from $_GET
	if ($update_type == 'Manage my Subscription!') {
		$form_state['redirect'] = array('subscriptions/manage', array('query' => array('email' => $email)), 302);
	} else {
		$message = custom_arsubscriber_listrak_manage($form_state);
	}

	// set proper messaging for unsubscribers
	if ($update_type == 'Unsubscribe me from Everything') {
		$form_state['redirect'] = array('subscriptions/manage');
		if ($message == 'SuccessContactUnSubscribed' || $message == 'ContactAlreadyUnSubscribed') {
			drupal_set_message('You have been successfully unsubscribed.');
		} elseif ($message == 'ContactNotSubscribed') {
			drupal_set_message('That email address is not subscribed.  Please try again using a different email address.');
		} else {
			drupal_set_message('We\'re sorry, but you could not be unsubscribed due to a system error.');
			watchdog('custom_subscriber', 'Failed to properly unsubscribe a subscriber.', WATCHDOG_WARNING);
		}
	}
	
	// set proper messaging for list management needs
	if ($update_type == 'Save') {
		if ($message == 'SuccessContactSubscribed') {
			drupal_set_message('You have been subscribed!');
		} elseif ($message == 'ContactAlreadySubscribedProfileUpdated') {
			drupal_set_message('Your profile has been updated.');
		} else {
			drupal_set_message('We\'re sorry, but you could not be unsubscribed due to a system error.');
			watchdog('custom_subscriber', 'Failed to properly update a subscribers information.', WATCHDOG_WARNING);
		}
	}
}

