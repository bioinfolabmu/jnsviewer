<?php  	
	if($_POST["processing"] != 2) {
		//user input seq or upload a file
		//upload gff file
		require_once 'upload_gff.php';
		if (isset($_POST['f_c']) && !empty($_POST['f_c']) && isset($_POST['file_ext'])) {
			$seq = $_POST['f_c'];
			$file_ext = $_POST['file_ext'];
			$_content = $seq;
			$file = $user['data_dir'] . '/' . $filename . "." . $file_ext;
			file_put_contents($file, $_content);
			//chmod($file, 0777);
			require_once 'seqtodb.php';
		}
	}
?>