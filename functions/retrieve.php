<?php
	if(isset($_POST['id']) && isset($_POST['code'])) {
		require_once 'function.php';
		// information for user
		$user = array();
		$user['id'] = $_POST['id'];
		// names for user dir
		require_once 'user_info.php';		
		// access code
		$user['code'] = $_POST['code'];
		// test whether user id and access code match
		if(validate_user($user['access'], $user['code'])) {
			require_once '../config/ini.php';
			$_ssdjfile = $user['data_dir'] . '/' . $filename. ".ssdj";
			// program still running
			if(!file_exists($_ssdjfile)) {
				echo 'running';
			} else {
				require_once 'complete.php';
			}			
		} else {
			echo 'fail';
		}
	}
?>
