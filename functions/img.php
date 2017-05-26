<?php
	if(isset($_POST['id'])) {
		// information for user
		$user = array();
		$user['id'] = $_POST['id'];
		// names for user dir
		require_once 'user_info.php';
		require_once '../config/ini.php';
		$_png = $user['img_dir'] . '/' . $filename . '.png';
		$_svg = $user['img_dir'] . '/' . $filename . '.svg';
		if(file_exists($_png) && file_exists($_svg)) {
			$data['png'] = substr($_png, 3);
			$data['svg'] = substr($_svg, 3);
			echo json_encode($data);
		}
	}
?>