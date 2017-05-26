<?php
	ini_set('error_reporting', E_ALL);
	ini_set('display_errors', 'On');
	
	if(!isset($_POST['request'])) {
		die();
	}
	
	require_once 'function.php';
	
	// information for user
	$user = array();
	
	// generate user id
	$user['id'] = generateRandomString(8)."_".date("mdy")."_".date("His");
	
	// names for user dir
	require_once 'user_info.php';
	
	// The dir of user
	initialiseUserDir($user['dir']);
	
	// The dir of data files.
	initialiseUserDir($user['data_dir']);
	
	// The dir of image files.
	initialiseUserDir($user['img_dir']);
	
	// Create Access File
	$code = generateRandomString(20);
	write_file($user['access'], $code);
	
	$data = array('id' => $user['id'], 'code' => $code);
	echo json_encode($data);
?>