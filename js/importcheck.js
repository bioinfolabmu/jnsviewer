function check_form() {
	var form = document.getElementById("formInput");
	var main_choice = document.getElementsByName("processing");	
	if(main_choice[1].checked) {
		//check gff file extention
		var gff = document.getElementById("gffFile").value;
		if(gff!=="" && !check_gff(gff)) {
			alert("GFF File format is not correct. Please check your file again.");
			return false;
		}
		var sequence = document.getElementById("sequence").value;
		var filename = document.getElementById("file").value;
		var type = '';
		if(filename) {
			if(!check_filename(filename)) {
				return false;
			} else {
				type = get_file_ext(filename);
			}
		}
		if(sequence != "") {
			if(!check_sequence(sequence, type)) {
				return false;
			}
		} else {
			alert("Please enter your sequence or upload a file.");
			return false;
		}
	}	
	return true;
}

function isRNA(s) {
	var seq;
	seq=s.toUpperCase();
	// RNA sequence may consist of {A/U/G/C/T/N}
	var patrn=/[^AUGCTN]/;
	if ( !patrn.exec(seq) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

function check_sequence(seq, type) {
	if(type == 'dbn') {
		// identifier line
		if(seq[0] == '>') {
			// split line
			var lines = seq.split("\n");
			if(lines.length < 2) {
				alert("Incorrect sequence format. Please make sure to import a sequence.");
				return false;     
			} else {
				seq = lines[1];
			}			
		}
		//Check Sequence Length
		if(seq.length > 700) {
			var msg = 'Your input sequence is too long to have accurate RNA secondary structure prediction (Nature Reviews Genetics, 2011, 12:641-655). If you still want to perform prediction, press OK; otherwise press Cancel.';
			var flag = confirm(msg);
			if(!flag) {		
				return false;
			}
		}
		//Not Valid Sequence
		if(seq && !isRNA(seq)){
			alert("Sequence is not a valid RNA or DNA sequence. Please import a RNA or DNA sequence only.");
			return false;
		}
	}
	//Imported Empty Sequence
	if (seq == "") {
		alert("Empty sequence. Please make sure to import a sequence.");
		return false;     
	}
	return true;
}

function get_file_ext(filename) {
	return filename.split('.').pop();
}

function check_filename(filename) {
	var ext = get_file_ext(filename);
	if (ext == 'ssdj' || ext == 'dbn' || ext == 'ct') {
		return true;
	}
	else {
		alert("Sequence file format is not correct. Please check your file again.");
		return false;
	}
}

function check_gff(filename) {
	var ex_gff;
	ex_gff= /\.gff$/i;
	var ex_gtf;
	ex_gtf = /\.gtf$/i;
	var ex_gff3;
	ex_gff3 = /\.gff3$/i;
	if (ex_gff.exec(filename)
			|| ex_gtf.exec(filename) 
			|| ex_gff3.exec(filename))
	{
		return true;
	}
	else
	{		
		return false;
	}	
}