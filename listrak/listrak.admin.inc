<?php

/**
 * @file
 * Admin settings forms for the listrak module.
 *
 * @author Tom Kirkpatrick (mrfelton), www.systemseed.com
 * @author port to D7 David Morrison
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
  $form['listrak_config']['listrak_form_list_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Listrak List ID'),
    '#default_value' => variable_get('listrak_form_list_id', '12345'),
    '#size' => 15,
    '#maxlength' => 20,
    '#description' => t("The internal Listrak ID of the list you'll be interacting with.")
  );
  $form['listrak_config']['listrak_form_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro HTML for the form'),
    '#default_value' => variable_get('listrak_form_html', '<p>Sign up for our email newsletters.</p>'),
    '#description' => t("This HTML will be output just before the form itself."),
    '#resizable' => TRUE,
  );
  $form['listrak_config']['listrak_form_success_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Thank you for signing up message'),
    '#default_value' => variable_get('listrak_form_success_message', '<p>Thanks for signing up, please fill out the next form.</p>'),
    '#description' => t("This HTML will be output just before the form itself."),
    '#resizable' => TRUE,
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

