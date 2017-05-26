<?php
	ini_set('error_reporting', E_ALL);
	ini_set('display_errors', 'On');
	
	if(!$_POST) {
		die();
	}
	require_once 'function.php';
	// information for user
	$user = array();
	$user['id'] = $_POST['id'];
	// names for user dir
	require_once 'user_info.php';
	// access code
	$user['code'] = $_POST['code'];
	// test whether user id and access code match
	if (validate_user($user['access'], $user['code'])) {
		// configuration file
		require_once '../config/ini.php';
		// parse form data
		require_once 'savetexttofile.php';
	}
?>