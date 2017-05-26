<?php
	if (file_exists($_dbnfile)) {
		$_ssdjfile = $user['data_dir'] . '/' . $filename. ".ssdj";
		// remove existing ssdj file
		if (file_exists($_ssdjfile)) {
			unlink($_ssdjfile);
		}
		$command = '../exe/dbtoss ' . $_dbnfile . ' ' . $_ssdjfile;
		shell_exec($command);
		require_once 'complete.php';
	}
	else {
		die('Error: Your seuquence can\'t be folded by this algorithm with such settings. Please try other settings.');
	}
?>