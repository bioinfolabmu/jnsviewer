//General IsChecked Function for all Checkboxes (Emily)
function IsChecked(flag, sequence) {
	var display = document.getElementById("sequence");
	// get the characters in a row
	var numberOfCharactersInRow = 100;	//100
	if (!flag) {
		//Remove Rulers
		display.value = sequence;
	} else {
		//Add Rulers		
		display.value = addRulers(sequence, sequence.length, numberOfCharactersInRow);
	}
}

//Add New Lines (Emily)
function addRows(s, numberOfCharactersInRow) {

	var tempSeq = "";

	//Add a new line character every 100 characters (<br> is used as a temporary character and is replaced later)

	for (var i = 0; i < s.length; i += numberOfCharactersInRow) {

		tempSeq += s.substr(i, numberOfCharactersInRow) + '<br>';

	}

	return tempSeq;

}

//Adds Rulers (Emily)

function addRulers(inSeq, seqlen, numberOfCharactersInRow) {

	

	var numLineSpaces = (seqlen.toString().length>=4?seqlen.toString().length:4);

	

	//Number of spaces for line numbers

	var newSeq = "";



	//Temporary storage for new displaySeq (Rows are added by the AddRows Method)

	var splitSequence = addRows(inSeq, numberOfCharactersInRow).split('<br>');

	//Break displaySeq up by rows

	var rowNumber = 0;

	var numOfPaddingSpaces = 0;

	//Get the value for the first row number

	var spaceFlag = false;

	//false if padding spaces are found, true if they have passed

	var characterFlag = false;

	//false if nucleotides are found, true if other characters are found.



	//Add Top Ruler, 10-100 in increments of 10.

	spaces_9 = "         ";

	rulerLeadingSpaces = "";

	if (numLineSpaces - 2 > 0) {

		for (var n = 0; n < (numLineSpaces) - 2; n++) {

			rulerLeadingSpaces += " ";

		}

	}



	if (sequence.end < numberOfCharactersInRow)

		rulerLeadingSpaces += " ";



	//Spaces between column numbers

	newSeq +=  rulerLeadingSpaces + " " + spaces_9 + "10" + spaces_9 + "20" + spaces_9 + "30" + spaces_9 + "40" + spaces_9 + "50" + spaces_9 + "60" + spaces_9 + "70" + spaces_9 + "80" + spaces_9 + "90" + spaces_9 + "100" + '\n';



	for (var i = 0; i < splitSequence.length - 1; i++) {

		//Number of spaces that go before lines, depends on len of row #

		var tempLineSpace = "";

		//Temporary count for determing tempLineSpaces

		var count = 0;

		//Num characters in block- blocks are 10 characters long

		var blockLength = 0;

		//Temporary Storage for value retrived from splitSequence

		var tempSplitSeq = "";



		for (var j = 0; j < splitSequence[i].length; j++) {

			tempSplitSeq += splitSequence[i].charAt(j);

			if (spaceFlag == false) {

				if (splitSequence[i].charAt(j) == '<') {

					characterFlag = true;

				} else {

					blockLength++;

					if (blockLength == 10) {

						tempSplitSeq += " ";

						blockLength = 0;

					}

				}

				spaceFlag = true;



			} else {

				//Padding spaces are no longer being processed

				if (characterFlag == true) {

					//Currently in HTML tag.

					if (splitSequence[i].charAt(j) == '>')

						//Closing tag found.

						characterFlag = false;

				} else {

					//Not Currently in HTML Tag

					if (splitSequence[i].charAt(j) == '<') {

						//HTML Tag has started

						characterFlag = true;

					} else {

						blockLength++;

						if (blockLength == 10) {

							tempSplitSeq += " ";

							blockLength = 0;

						}

					}

				}

			}

		}

		//Add linespaces, the line number and then the sequence for that row.

		//Adds more spaces if the line number is shorter

		lineSeps = "";

		if (numLineSpaces - 1 > 0) {

			for (var n = 0; n < ((numLineSpaces - 1) - rowNumber.toString().length - 1); n++) {

				lineSeps += " ";



			}

		}

		newSeq += lineSeps + rowNumber + " " + tempSplitSeq.substr(0, tempSplitSeq.length) + '\n';

		rowNumber++;



	}

	//Remove Rogue Character at end

	newSeq=newSeq.substring(0,newSeq.length-1);

	

	//Replace all <br> with new line character

	return newSeq.replace("<br>",'\n');

}


//Convert text selection indices to indexes valid for SS Viewer and dot bracket notation (Emily)

function textToIndex(pos){

	var firstRowLength=114; //Header row length not including new line.

	var rowLabelLength=4;	//Lenght of row labels including new line.

	var numberOfCharactersPerGroup=11;	//Total number of characters in a group including space.

	var totalNumberOfCharactersPerLine=114;	//Total number of characters in a line including row label, spaces, and new Line.

	var offset=0;	//Total offset needed to convert text selection index to a valid index.

	

	//Amount for length of Header Row (Row 0) and first row label

	offset+=firstRowLength+rowLabelLength;

	

	//Position selected is in the header or first row labels

	if(pos<offset){

		return 0;

	}

	

	//Past the first Line (Rows beyond 1)

	if(pos>=(firstRowLength+rowLabelLength+totalNumberOfCharactersPerLine)){

		//Calculate number of Row Labels needed (including new line)

		offset+=(Math.floor((pos/(totalNumberOfCharactersPerLine)))-1)*rowLabelLength

	}

	

	//Past First Group in Row 1

	if(pos>=(numberOfCharactersPerGroup+firstRowLength+rowLabelLength)){

		//Calculate number of spaces needed

		offset+=Math.floor((pos-offset)/numberOfCharactersPerGroup);

	}

	//Return Index (Position - Offset)

	return pos-offset;



}

//Convert Indexes valid for SS Viewer and Dot Bracket notation to vaid text selection indices (Emily)
function indexToText(pos, isStart){
	//console.log("---"+pos);
	var firstRowLength=114;  //Header row length not including new line.
	var rowLabelLength=4;	 //Length of row labels including new line.
	var numberOfNucleotidesPerGroup=10;	//Total number of nucleotides in a group not including spaces
	var totalNucleotidesPerLine=100;	//Total number of nucleotides per line not including spaces, row labels or new lines.
	var offset=0;	//Total offset needed to convert a index to a vaid text selection index.
	//Amount for length of Header Row (Row 0) and first row label
	offset+=firstRowLength+rowLabelLength;
	//Calculate number of Row Labels needed (including new line)	
	if((pos)%(totalNucleotidesPerLine)==0){
		if(isStart){
			//Starting Posistion
			offset+=Math.floor(((pos)/(totalNucleotidesPerLine)))*rowLabelLength;
		}
		else{
			//Ending Posistion
			console.log("End of Line");
			offset+=Math.floor(((pos)/(totalNucleotidesPerLine))-1)*rowLabelLength;
		}
	}else{
		offset+=Math.floor(((pos)/(totalNucleotidesPerLine)))*rowLabelLength;
	}
	//Past First Group in Row 1
	if(pos>=numberOfNucleotidesPerGroup){
		//Calculate number of spaces needed		
		offset+=Math.floor(pos/numberOfNucleotidesPerGroup);
	}	
	//console.log(">>>"+pos+offset);
	//Return Index (Position + Offset)
	return pos+offset;
}