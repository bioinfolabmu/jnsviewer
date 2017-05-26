<?php
	$_dbnfile = $user['data_dir'] . '/' . $filename . '.dbn';
	$_ctfile = $user['data_dir'] . '/' . $filename . '.ct';
	$_ssdjfile = $user['data_dir'] . '/' . $filename. ".ssdj";
	// perform prediction according to different format
	switch ($file_ext) {
		case 'ct':
			$command="../exe/cttodbn ".escapeshellcmd($_ctfile.' '.$_dbnfile);
			// Exec the command.
			$result=shell_exec($command);
			//Redirect to 'dbtoss.php' for conversion to SSFormat
			require_once 'dbtoss.php';
			break;
		case 'dbn':	
			//Redirect to 'dbtoss.php' for conversion to SSFormat
			require_once 'dbtoss.php';
			break;
		case 'ssdj':
			//File is in SSDJ (JSON) format
			// extract DBN file
			$in = fopen($_ssdjfile, "r") or die('Internal error');
			$line = fgets($in);
			$a = explode(',', $line);
			$seq = trim(explode(':', $a[0])[1], '"');
			$dbn = trim(explode(':', $a[1])[1], '"');
			fclose($in);
			$out = fopen($_dbnfile, "w") or die('Internal error');
			fwrite($out, ">sequence\n");
			fwrite($out, $seq . "\n");
			fwrite($out, $dbn . "\n");
			fclose($out);
			//Redirect to Completion Page
			require_once 'complete.php';
			break;
		default:
			die('Error: Upload file format is not correct.');
    }
?>