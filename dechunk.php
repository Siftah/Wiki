#!/usr/bin/php
<?php
/**
 * http://wiki.siftah.com/Split_An_XML_File_Into_Chunks
 *
 */

class xmlChunk {
	/**
	 *
	 */
	function xmlChunk() {
	}

/*
 * $basefilename // the base file name for the chunks
 * $xmlfile // the xml file name to be processed
 * $xmldatadelimiter // core data delimiter
 * $xmlitemdelimiter // record delimiter
 * $chunksize = 2000; // number of records in each chunk file
 * $dir // path to where splits will be stored
*/

	/**
	 *
	 */
	function doChunk( $basefilename, $xmlfile, $xmldatadelimiter, $xmlitemdelimiter, $chunksize=2000, $dir= "/var/www/public_html") {
		//initialize vars
		$begin=time(); // script start time
		$start = time(); // last gate time
		$interval=time(); // current gate time
		$minutes=1; // intervals for gates
		$filenum = 1; // start chunk file number at 1
		$recordnum = 1; // start at record 1
		$xmlstring =."\n";
		$xmlstring.="<$xmldatadelimiter>\n";
		// xmlchunk file header
		//dirs and files
		$exportfile = "$dir"."/splits/$basefilename-$filenum.xml";
		//start processing
		echo "Processing (".$dir."/$xmlfile)\n";
		$handle = @fopen($dir."/$xmlfile", "r");
		if ($handle) {
			while (!feof($handle)) {
				$buffer = fgets($handle, 4096);
				// if item delimiter reached
				// increment record number iterator
				if (ereg($xmlitemdelimiter, $buffer)==true) {
					$recordnum++;
				}
				//write line to chunk file
				error_log("$buffer", 3, $exportfile);
				// if chunk limit reached then start to
				// close the file with well formed xml
				if ($recordnum>$chunksize) {
					// post feed end tag
					error_log("", 3, $exportfile);
					// and increment file number to start new log file chunk
					//reset record counter number for new chunk file
					$recordnum=0;
					$filenum++;
					//update export file name
					$exportfile = "$dir"."/splits/$basefilename-$filenum.xml";
					//echo status report to STDOUT
					echo"Segment $filenum. Record ".($chunksize*$filenum).".\n";
					// write new chunk xml file header
					error_log($xmlstring, 3, $exportfile);
				}
				//put in a catch so that script doesn't run riot and
				//will die after X number of cycles
				if ($filenum>5000) {
					die();
				}
				if (($interval-$start)>60) {
					$minutes++;
					echo $minutes." Minutes so far.\n";
					$start=time();
				} else {
					$interval = time();
				}
			}
			fclose($handle);
		} else {
			echo"Unable to open file! (".$dir."$xmlfile\")\n";
		}
		$procend = time();
		echo "\n####\n";
		echo "Split Complete (".floor((($procend-$begin)/60))." Minutes)\n";
	}


}


if ($argv[1] != "") {
	$fileToParse = $argv[1];
	$directoryToUse = dirname($fileToParse);
	$basefilename = basename($fileToParse, ".xml");
	$filename = basename($fileToParse);
	mkdir("$directoryToUse/splits");

	if ($argv[4] != "") {
		$recordLimit = $argv[4];
	} else {
		$recordLimit = 2000;
	}

	echo "Creating ".$basefilename." Splits
";
	$chunk = new xmlChunk();
	$chunk -> doChunk( $basefilename, $filename, $argv[2], $argv[3], $recordLimit, $directoryToUse);
	unset($chunk);
} else {
	echo "Usage: ./dechunk.php /home/you/hugeBigXMLFile.xml [RootTag] [ItemTag] [RecordCount]\r\n";
}
?>
