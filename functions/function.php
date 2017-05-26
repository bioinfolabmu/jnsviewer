<?php
	function initialiseUserDir($dir) {
		if (!is_dir($dir) ) {
			mkdir($dir);
		}
	}
	
	function generateRandomString($length){
		$avaliableCharacters='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
		$randString="";
		for($i = 0; $i < $length; $i++){
			$randString.=substr(str_shuffle($avaliableCharacters), 0,1);
		}
		return $randString;
	}
	
	function write_file($fileName, $returnString) {
		$fileHandle= fopen($fileName, 'w') or die ("Can't Open File! $fileName");
		fwrite($fileHandle, $returnString);
		fclose($fileHandle);
	}
	
	function validate_user($fileName, $code) {
		if(file_exists($fileName)) {
			$fd = fopen ($fileName, "r");
			$contents = fread ($fd,filesize ($fileName));
			fclose ($fd);
			$delimiter = "\n";
			$contents = explode($delimiter, $contents);
			$accessCode=$contents[0];
			if(strcmp(trim($accessCode),trim($code))==0) {
				return true;
			}
		}
		return false;
	}

	function getFileExtension($fileName){
		// The length of full filename.
		$_len = strlen($fileName);
		// The position of '.' in the right side of the full filename.
		$_pos = strripos($fileName, '.', 0 );
		// filename extension
		return substr($fileName, $_pos+1, $_len );
	}
	
	function find_pos($database, $id) {
		$path = $database . '/id_to_pos';
		$myfile = fopen($path, "r") or die("Internal error");
		$result = "not found";
		
		while($line = fgets($myfile)) {
			$tmp = explode("\t",$line);
			//find the name
			if($tmp[0] == $id) {
				$result = $tmp[1];
				break;
			}
		}
		fclose($myfile);
		return $result;
	}
	
	function getData($filename)	{
		// Import the datas from the data file.
		if(!file_exists($filename)) {
			return '';
		}
		$content = file_get_contents($filename);
		return $content;
	}
?>