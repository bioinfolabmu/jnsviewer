<?php
	if(isset($_POST['id']) &&  isset($_POST['type'])) {
		// information for user
		$user = array();
		$user['id'] = $_POST['id'];
		// names for user dir
		require_once 'user_info.php';
		require_once '../config/ini.php';
		$type = $_POST['type'];
		$_file = $user['data_dir'] . '/' . $filename . '.' . $type;
		if(file_exists($_file)) {
			$data = array();
			$data['content'] = file_get_contents($_file);
			$data['download'] = substr($_file, 3);
			echo json_encode($data);
		}
	}
?>