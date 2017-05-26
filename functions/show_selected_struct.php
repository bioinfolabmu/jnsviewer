<?php 
	ini_set('error_reporting', E_ALL);
	ini_set('display_errors', 'On');
	
	if(isset($_POST['id']) && isset($_POST['start']) && isset($_POST['end'])) {
		// information for user
		$user = array();
		$user['id'] = $_POST['id'];
		// names for user dir
		require_once 'user_info.php';
		require_once '../config/ini.php';
		require_once 'function.php';
		$_dbnfile = $user['data_dir'] . '/' . $filename . '.dbn';
		$gff_file = $user['data_dir'] . '/' . $gffFileName;
		$start = $_POST['start'] - 1;
		$end = $_POST['end'] - 1;
		$error = 'The selected region cannot present a secondary structure (some paired bases are broken).';
		if(file_exists($_dbnfile)) {
			$in = fopen($_dbnfile, "r") or die('Internal error');
			// new sub structure dbn file
			$dbn_new = $user['data_dir'] . '/' . $filename . '_sub.dbn';
			$out = fopen($dbn_new, "w") or die('Internal error');
			//id
			$line = fgets($in);
			fwrite($out, $line);
			//seq
			$seq = trim(fgets($in));
			//dbn
			$struct = trim(fgets($in));			
			//( and ) must be matched
			$left = 0;
			$right = 0;
			$arr = str_split($struct);
			for($i=$start; $i<=$end; $i++){
				if($arr[$i] == '(') {
					$left++;
				} else if($arr[$i] == ')') {
					$right++;
				}
				if($right > $left) {				
					die($error);
				}
			}	
			if($left != $right) {
				die($error);
			}
			$seq = substr($seq, $start, $end - $start + 1);
			fwrite($out, $seq."\n");
			$struct = substr($struct, $start, $end - $start + 1);
			fwrite($out, $struct."\n");
			fclose($in);
			fclose($out);
			// new gff file
			$gff_content = 'none';
			if(file_exists($gff_file)) {
				$in = fopen($gff_file, "r") or die('Internal error');
				$gff_new = $user['data_dir'] . '/new_' . $gffFileName;		
				$out = fopen($gff_new, "w") or die("Internal error");				
				$start++;
				$end++;
				while($line = fgets($in)) {
					$lines = explode("\t", $line);
					if (count($lines) < 9) {
						continue;
					}
					$my_start = $lines[3];
					$my_end = $lines[4];
					if($my_start >= $start && $my_end <= $end) {
						//feature within the range
						$lines[3] = $lines[3]-$start+1;
						$lines[4] = $lines[4]-$start+1;
						$content = $lines[0];
						for($i=1; $i<count($lines); $i++) {
							$content .= "\t$lines[$i]";
						}
						fwrite($out, $content);
					} else if($my_start < $start && $my_end >= $start && $my_end <= $end) {
						//part of feature within the range
						$lines[3] = 1;
						$lines[4] = $lines[4]-$start+1;
						$content = $lines[0];
						for($i=1; $i<count($lines); $i++) {
							$content .= "\t$lines[$i]";
						}
						fwrite($out, $content);
					} else if($my_end > $end && $my_start >= $start && $my_start <= $end) {
						//part of feature within the range
						$lines[3] = $lines[3]-$start+1;
						$lines[4] = $end-$start+1;
						$content = $lines[0];
						for($i=1; $i<count($lines); $i++) {
							$content .= "\t$lines[$i]";
						}
						fwrite($out, $content);
					} else if($my_start < $start && $my_end > $end) {
						$lines[3] = 1;
						$lines[4] = $end-$start+1;
						$content = $lines[0];
						for($i=1; $i<count($lines); $i++) {
							$content .= "\t$lines[$i]";
						}
						fwrite($out, $content);
					}
				}					
				fclose($in);
				fclose($out);
				$gff_content = getData($gff_new);
			}
			// dbn to ssdj
			$ssdjfile_new = $user['data_dir'] . '/' . $filename. "_sub.ssdj";
			$command = '../exe/dbtoss ' . $dbn_new . ' ' . $ssdjfile_new;
			shell_exec($command);
			$ssdj = getData($ssdjfile_new);
			$data = array('ssdj' => $ssdj, 'gff' => $gff_content);
			echo json_encode($data);
		}
	}	
?>