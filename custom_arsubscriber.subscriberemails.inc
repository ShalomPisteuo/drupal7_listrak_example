<?php

/**
 * @file
 * Custom Subscriber Listrak Forms, Validation and Submit Logic
 */

/**
 * Implements hook_mail().
 *
 * Tells Drupal to send an email with specified content.
 */
function custom_subscriber_mail($key, &$message, $params) {
  
  switch($key) {
    // custom email for AR Subscribers
    case 'ar subscriber':
      // set the subject for this email
      $message['subject'] = t('We have a new AutoRemarketing subscriber!');
    
      // set the email body content
      $message['body'][] = t("Hello AR staff, \n\nWe have a new AR print subscriber!\n\nHere is their information:\n\nName: !firstname !lastname\nTitle: !title\nCompany Name: !companyname\nAddress: !address\nCity: !city\nState/Province: !stateprovince\nCountry: !country\nPostal Code: !postalcode\nPhone number: !phonenumber\nEmail: !email", array(
        '!firstname' => $params['firstname'],
        '!lastname' => $params['lastname'],
        '!title' => $params['title'],
        '!companyname' => $params['companyname'],
        '!address' => $params['address'],
        '!city' => $params['city'],
        '!stateprovince' => $params['stateprovince'],
        '!country' => $params['country'],
        '!postalcode' => $params['postalcode'],
        '!phonenumber' => $params['phonenumber'],
        '!email' => $params['email'],
      ));
    break;
    
    // custom email for BHPH Subscribers
    case 'bhph subscriber':
      // set the subject for this email
      $message['subject'] = t('We have a new BHPH subscriber!');
    
      // set the email body content
      $message['body'][] = t("Hello BHPH staff, \n\nWe have a new BHPH print subscriber!\n\nHere is their information:\n\nName: !firstname !lastname\nTitle: !title\nCompany Name: !companyname\nAddress: !address\nCity: !city\nState/Province: !stateprovince\nCountry: !country\nPostal Code: !postalcode\nPhone number: !phonenumber\nEmail: !email", array(
        '!firstname' => $params['firstname'],
        '!lastname' => $params['lastname'],
        '!title' => $params['title'],
        '!companyname' => $params['companyname'],
        '!address' => $params['address'],
        '!city' => $params['city'],
        '!stateprovince' => $params['stateprovince'],
        '!country' => $params['country'],
        '!postalcode' => $params['postalcode'],
        '!phonenumber' => $params['phonenumber'],
        '!email' => $params['email'],
      ));
    break;

    // custom email for SP Subscribers
    case 'sp subscriber':
      // set the subject for this email
      $message['subject'] = t('We have a new SP subscriber!');
    
      // set the email body content
      $message['body'][] = t("Hello SubPrime staff, \n\nWe have a new SP print subscriber!\n\nHere is their information:\n\nName: !firstname !lastname\nTitle: !title\nCompany Name: !companyname\nAddress: !address\nCity: !city\nState/Province: !stateprovince\nCountry: !country\nPostal Code: !postalcode\nPhone number: !phonenumber\nEmail: !email", array(
        '!firstname' => $params['firstname'],
        '!lastname' => $params['lastname'],
        '!title' => $params['title'],
        '!companyname' => $params['companyname'],
        '!address' => $params['address'],
        '!city' => $params['city'],
        '!stateprovince' => $params['stateprovince'],
        '!country' => $params['country'],
        '!postalcode' => $params['postalcode'],
        '!phonenumber' => $params['phonenumber'],
        '!email' => $params['email'],
      ));
    break;
  }
}
