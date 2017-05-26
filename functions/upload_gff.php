<?php
	if(isset($_POST['gff_f_c'])&& $_POST['gff_f_c']!=null) {
		//Text GFF
		// Redirect upload dir for every user.
		// Content of file.
		$_content =$_POST['gff_f_c'];
		// Save contents to file.
		$file = $user['data_dir'] . '/' . $gffFileName;
		file_put_contents($file, $_content);
		//chmod($file, 0777);	
	}
?>