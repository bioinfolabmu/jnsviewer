<?php
    if(isset($_GET['file']) && !empty($_GET['file'])) {
    	$filename = $_GET['file'];
    	force_download( $filename );
    } else {
		header("Location: ../index.php");
	}

    function force_download( $file )
    {
        if( file_exists($file) )
        { // The file exists.
            // Clear the output buffer.
            ob_clean();

            header( 'Content-Type: application/octet-stream' );

            header( 'Content-Disposition:attachment;filename="'.$file.'"');//.txt
            header( 'Content-Length:'.filesize($file) );
            readfile( $file );
            // OK
        }
        else
        { // The file not exists.
            echo "Oops! File does not exist.";
        }
    } 
?>
