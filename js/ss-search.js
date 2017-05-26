//Search Sequence for a given Motif (Emily)
function motifSearch(sequence, displayRulers, result){
	//To Do: Add Wild Card and Ability to show more than one result
	var query= document.getElementById("query").value;	//Motif to Find
	//if( !query.match(/(G|A|U|C|\*){3,}/gi) ){
	//if(!query.match(/(G|A|U|C|T|N|\?){3,}/gi) ){
	if(!query.match(/(G|A|U|C|T|N|\?|\.|\*|\+\[|\]){3,}/gi)){
		//console.log(query);
		//Invalid Query
		document.getElementById("motif_text_results").innerHTML="<div style=\"color:red\">Invalid Query.</div>";
		//Disable Next and Previous Buttons
		document.getElementById("nextResult").disabled=true;
		document.getElementById("previousResult").disabled=true;
		return;
	}
	//query = query.replace(/\?/g, ".");	//Replace Single Wild Card Character with Wild Card for Regular Expressions
	var search=RegExp(query, "gi");	//Create Regular Expression to Find Motif
	//console.log(query);
	//Find all Matches (only shows first for now)
	result.matches = sequence.match(search);//store matched results
	//console.log(result.matches);	
	while (match = search.exec(sequence)){
		result.indexes.push(match.index);
   	}
   	//console.log(result.indexes.length);
   	//Check for Results	
   	if(result.indexes.length != 0){
	   	//Highlight Text Area (Only First Match for now)
	   	result.searchQueryLength = result.matches[0].length;
		//console.log(result);
		result.currentSearchResultDisplayed = 0;
	   	if(displayRulers){
	   		//Rulers On
			// function in ss-rulers.js
	   		document.getElementById("sequence").selectionStart = indexToText(result.indexes[0], true);
	   		document.getElementById("sequence").selectionEnd = indexToText(result.indexes[0]+result.searchQueryLength, false);
	   	} else{
	   		//Rulers Off
	   		document.getElementById("sequence").selectionStart = result.indexes[0];
	   		document.getElementById("sequence").selectionEnd = result.indexes[0]+result.searchQueryLength;
	   	}
		// functions in ss-client.js
		// select bases in dbn and structure graph
	   	signsSelected(result.indexes[0], (result.indexes[0]+result.searchQueryLength), sequence.length);
		elementsSelected(result.indexes[0], (result.indexes[0]+result.searchQueryLength), sequence.length);
		document.getElementById("motif_text_results").innerHTML	= "Showing Result 1/"+result.indexes.length+". ";
		//Enable Next and Previous Buttons
		document.getElementById("nextResult").disabled=false;
		document.getElementById("previousResult").disabled=false;
	    document.getElementById("sequence").focus();
		// add position to based selected field
		document.getElementById('selected_start').innerHTML = result.indexes[0] + 1;
		document.getElementById('selected_end').innerHTML = result.indexes[0] + result.searchQueryLength;
	} else{
		//No Results found
		result.searchQueryLength=0;
		result.currentSearchResultDisplayed=-1;
		document.getElementById("motif_text_results").innerHTML="No Results Found.";
		//Disable Next and Previous Buttons
		document.getElementById("nextResult").disabled=true;
		document.getElementById("previousResult").disabled=true;
	}
}

//Move to Next Search Result (Emily)
function nextSearchResult(len, displayRulers, result) {
	//Check that Previous Search Result is valid
	if(result.currentSearchResultDisplayed+1<result.indexes.length){
		//Valid
		result.currentSearchResultDisplayed++;
	} else{
		//Not Valid, return to Start
		result.currentSearchResultDisplayed = 0;
	}
	//Update Displays to Reflect New Result
	result.searchQueryLength = result.matches[result.currentSearchResultDisplayed].length;
	var indexSelectionEnd=(result.indexes[result.currentSearchResultDisplayed] + result.searchQueryLength);
	var indexSelectionStart=result.indexes[result.currentSearchResultDisplayed];	
	if (displayRulers) {
		//Rulers On
		// function in ss-rulers.js
		document.getElementById("sequence").selectionStart = indexToText(indexSelectionStart, true);
		document.getElementById("sequence").selectionEnd = indexToText(indexSelectionEnd,false);
	} else {
		//Rulers Off
		document.getElementById("sequence").selectionStart = result.indexes[result.currentSearchResultDisplayed];
		document.getElementById("sequence").selectionEnd = result.indexes[result.currentSearchResultDisplayed] + result.searchQueryLength;
	}
	signsSelected(indexSelectionStart, indexSelectionEnd, len);
	elementsSelected(indexSelectionStart, indexSelectionEnd, len);
	//Update Motif Search Result
	document.getElementById("motif_text_results").innerHTML	= "Showing Result "+(result.currentSearchResultDisplayed+1)+"/"+result.indexes.length+". ";
	document.getElementById("sequence").focus();
	// add position to based selected field
	document.getElementById('selected_start').innerHTML = result.indexes[result.currentSearchResultDisplayed] + 1;
	document.getElementById('selected_end').innerHTML = result.indexes[result.currentSearchResultDisplayed] + result.searchQueryLength;
}

//Move to Previous Search Result (Emily)
function previousSearchResult(len, displayRulers, result) {
	//Check that Previous Search Result is valid 
	if(result.currentSearchResultDisplayed-1>=0){
		//Valid
		result.currentSearchResultDisplayed--;
	} else{
		//Not Valid, return to End
		result.currentSearchResultDisplayed=result.indexes.length-1;
	}
	//Update Displays to Reflect New Result
	result.searchQueryLength = result.matches[result.currentSearchResultDisplayed].length;
	var indexSelectionEnd=(result.indexes[result.currentSearchResultDisplayed] + result.searchQueryLength);
	var indexSelectionStart=result.indexes[result.currentSearchResultDisplayed];	
	if (displayRulers) {
		//Rulers On
		// function in ss-rulers.js
		document.getElementById("sequence").selectionStart = indexToText(indexSelectionStart, true);
		document.getElementById("sequence").selectionEnd = indexToText(indexSelectionEnd,false);
	} else {
		//Rulers Off
		document.getElementById("sequence").selectionStart = result.indexes[result.currentSearchResultDisplayed];
		document.getElementById("sequence").selectionEnd = result.indexes[result.currentSearchResultDisplayed] + result.searchQueryLength;
	}
	signsSelected(indexSelectionStart, indexSelectionEnd, len);
	elementsSelected(indexSelectionStart, indexSelectionEnd, len);
	//Update Motif Search Result
	document.getElementById("motif_text_results").innerHTML	= "Showing Result "+(result.currentSearchResultDisplayed+1)+"/"+result.indexes.length+". ";
	document.getElementById("sequence").focus();
	// add position to based selected field
	document.getElementById('selected_start').innerHTML = result.indexes[result.currentSearchResultDisplayed] + 1;
	document.getElementById('selected_end').innerHTML = result.indexes[result.currentSearchResultDisplayed] + result.searchQueryLength;
}

//Reset Motif Search (Emily)
function resetMotifSearch(len){
	//Clear out query text field
	document.getElementById("query").value="";
	//Clear out Motif Results
	document.getElementById("motif_text_results").innerHTML="No resutls found";
	//Disable Next and Previous Buttons
	document.getElementById("nextResult").disabled=true;
	document.getElementById("previousResult").disabled=true;
}