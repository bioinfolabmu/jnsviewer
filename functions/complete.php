<?php
	if(!file_exists($_ssdjfile)) {
		die('Error: Your seuquence can\'t be folded by this algorithm with such settings. Please try other settings.');
	}
	$data = array();
	// input data format
	if (isset($file_ext)) {
		$data['format'] = $file_ext;
	} else {
		$data['format'] = 'ssdj';
	}	
	$data["gffError"] = '';
	$data["warn_gff"] = '';
	// test gff file
	if(isset($user["gffError"])){
 		$data["gffError"] = $user["gffError"];
 	} else if(!file_exists($user['data_dir'] . '/' . $gffFileName)){
		$data["warn_gff"] = true;
	}
	//generate svg and png files
	$_pngfile = $user['img_dir']."/".$filename.".png";
	$_svgfile = $user['img_dir']."/".$filename.".svg";
	// SSDJ to PNG
	$command="../exe/ssdjtopng ".$_ssdjfile.' '.$_pngfile;
	$result=shell_exec($command);		
	// SSDJ to SVG
	$command="../exe/ssdjtosvg ".$_ssdjfile.' '.$_svgfile;
	$result=shell_exec($command);	
	//zip the whole data folder
	$data['download'] = 'users/' . $user['id'] . '/whole_data.zip';
	$command = 'zip -r ' . $user['dir'] . '/whole_data.zip ' . $user['dir'];
	$result = shell_exec($command);
	echo json_encode($data);
?>