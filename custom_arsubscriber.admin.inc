<?php

/**
 * @file
 * Admin settings forms for the listrak module.
 * 
 */

/**
 * Form callback; The admin settings form for the listrak module.
 */
function listrak_api_settings_form() {
  $form = array();

  $form['listrak_config'] = array(
    '#type' => 'fieldset',
  );
  // Field for the Security ID (listid)
  $form['listrak_config']['listrak_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Your Listrak username.'),
    '#default_value' => variable_get('listrak_user', NULL),
    '#required' => TRUE,
  );
  // Field for the Security ID (listid)
  $form['listrak_config']['listrak_pass'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Your Listrak password.'),
    '#default_value' => variable_get('listrak_pass', NULL),
    '#required' => TRUE,
  );
  $form['listrak_config']['listrak_proxy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proxy Support'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('If the API requests need to pass through a proxy server please fill in the information below.'),
  );
  $form['listrak_config']['listrak_proxy']['listrak_proxy_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#default_value' => variable_get('listrak_proxy_login', NULL),
  );
  $form['listrak_config']['listrak_proxy']['listrak_proxy_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('listrak_proxy_pass', NULL),
    '#description' => t('Warning: the password is stored as plain text in the database.'),
  );
  $form['listrak_config']['listrak_proxy']['listrak_proxy_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('listrak_proxy_host', NULL),
  );
  $form['listrak_config']['listrak_proxy']['listrak_proxy_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('listrak_proxy_port', NULL),
  );

  return system_settings_form($form);
}

