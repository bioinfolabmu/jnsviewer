<?php
	if(isset($_POST['id']) && !empty($_POST['id'])) {
		require_once 'function.php';
		require_once '../config/ini.php';
		$user = array();
		$user['id'] = $_POST['id'];
		// names for user dir
		require_once 'user_info.php';
		$_ssdjfile = $user['data_dir'] . '/' . $filename. ".ssdj";
		$ssdj = getData($_ssdjfile);
		$gff = 'none';
		if(file_exists($user['data_dir'] . '/' . $gffFileName)) {
			$gff = getData($user['data_dir'] . '/' . $gffFileName);
		}
		$data = array('ssdj' => $ssdj, 'gff' => $gff);
		echo json_encode($data);
	}
?>
