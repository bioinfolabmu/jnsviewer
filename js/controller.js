app.controller('retriveCtrl', function($scope, $http, $location, $routeParams) {
	if ($routeParams.id) {
		$scope.id = $routeParams.id;
	}	
	$scope.error1 = false;
	$scope.error2 = false;	
	$scope.mySubmit = function() {
		sessionStorage.setItem('id', $scope.id);
		sessionStorage.setItem('code', $scope.code);
		var data = {id: $scope.id, code: $scope.code};
		$http({
			method: 'post', 
			url: 'functions/retrieve.php',
			headers: {'Content-Type': 'application/x-www-form-urlencoded'},
			data: $.param(data)
		}).success(function(data) {
			if (data == "fail") {
				// retrieve fail
				$scope.error1 = true;
			} else if (data == "running") {
				// program still running
				$scope.error2 = true;
			} else {
				// store response data in session
				sessionStorage.setItem('format', data.format);
				//console.log(data.format);
				sessionStorage.setItem('gffError', data.gffError);
				sessionStorage.setItem('warn_gff', data.warn_gff);
				sessionStorage.setItem('download', data.download);
				// redirect to complete page
				$location.path('/complete');
			}
		});
		$scope.id = '';
		$scope.code = '';
	}
})
.controller('ssCtrl', function($scope, $http, $routeParams) {
	// show gff info
	if(!sessionStorage.getItem('warn_gff')) {
		$scope.show_gff = true;
	}
	// color mode
	$scope.colorModes = [{name: "Nucleotide", value: 0}];
	if($scope.show_gff) {
		$scope.colorModes = [{name: "GFF", value: 1}, {name: "Nucleotide", value: 0}];
	}
	// defualt color mode selection
	$scope.selectedColorMode = $scope.colorModes[0];
	// user selected pos
	$scope.selected = {start: 0, end: 0};
	$scope.search_ini = function() {
		// initialize search results
		$scope.search_result = {};
		$scope.search_result.indexes = [];
		$scope.search_result.matches = [];
		$scope.search_result.searchQueryLength = 0;
		$scope.search_result.currentSearchResultDisplayed = -1;
	}
	$scope.search_ini();
	// data send to server
	var data;
	if(sessionStorage.getItem('id')) {
		//console.log(sessionStorage.getItem('id'));
		// user data
		data = $.param({id: sessionStorage.getItem('id')});
	}
	// get sturcture ssdj file and gff file from server
	$http({
		method: 'post',
		url: 'functions/ss-server.php',					
		headers: {'Content-Type': 'application/x-www-form-urlencoded'},
		data: data
	}).success(function(data) {
		//console.log(data);
		if(data !== '') {
			if (!sessionStorage.getItem('id')) {
				sessionStorage.setItem('id', data.id);
			}			
			$scope.ssdj = eval('(' + data.ssdj +')');
			// show seqeuence
			$scope.sequence = $scope.ssdj.sequence;
			$scope.gff = data.gff;
			$scope.dbn = $scope.ssdj.dot_bracket;
			// function in ss-client.js
			// show dbn
			ShowDotbrackets($scope.dbn);
			// show secondary structure			
			showSS($scope.ssdj, 'svgSS', $scope.gff, 'gffColorKey');
			
		}
	});
	// toggle ruler
	$scope.toggle_ruler = function(event) {
		// function in ss-client.js
		$scope.ruler = event.target.checked;
		IsChecked($scope.ruler, $scope.sequence);
		// remove highlighted content
		// function in ss-client.js
		remove_selected($scope.sequence.length, $scope.selected);
	};
	$scope.colorModeSwitch = function() {
		if($scope.selectedColorMode.value == 1) {			
			showSS($scope.ssdj, 'svgSS', $scope.gff, 'gffColorKey');
			if($scope.ssdj_sub) {
				showSS($scope.ssdj_sub, 'svgSS_sub', $scope.gff_sub, 'gffColorKey2');
			}
		} else {
			showSS($scope.ssdj, 'svgSS', 'none', 'gffColorKey');
			if($scope.ssdj_sub) {
				showSS($scope.ssdj_sub, 'svgSS_sub', 'none', 'gffColorKey2');
			}
			
		}
	};
	$scope.motifSearch = function() {
		$scope.search_ini();
		// remove highlighted content
		// function in ss-client.js
		remove_selected($scope.sequence.length, $scope.selected);
		// function in ss-search.js
		motifSearch($scope.sequence, $scope.ruler, $scope.search_result);
	};
	$scope.resetMotifSearch = function() {		
		$scope.search_ini();
		// remove highlighted content
		// function in ss-client.js
		remove_selected($scope.sequence.length, $scope.selected);
		// function in ss-search.js
		resetMotifSearch($scope.sequence.length);
	};
	$scope.nextSearchResult = function() {
		// function in ss-search.js
		nextSearchResult($scope.sequence.length, $scope.ruler, $scope.search_result);
	};
	$scope.previousSearchResult = function() {
		// function in ss-search.js
		previousSearchResult($scope.sequence.length, $scope.ruler, $scope.search_result);
	};
	$scope.textClick = function(event) {
		// function in ss-client.js
		textClick(event.target, $scope.ruler, $scope.selected);
	};
	$scope.showSelectedStruct = function() {
		var error = 'You haven\'t select any base.';
		// dirty code
		$scope.selected.start = $('#selected_start').html();
		$scope.selected.end = $('#selected_end').html();
		if($scope.selected.start != 0 && $scope.selected.end != 0) {
			// re-run layout algorithmm in the server to get new ssdj file.
			$http({
				method: 'post',
				url: 'functions/show_selected_struct.php',
				headers: {'Content-Type': 'application/x-www-form-urlencoded'},
				data: $.param({id: sessionStorage.getItem('id'), start: $scope.selected.start, end: $scope.selected.end})
			}).success(function(data) {
				if(angular.isString(data) && data != '') {
					alert(data);
				} else {
					$scope.show_sub = true;
					$scope.ssdj_sub = eval('(' + data.ssdj +')');
					$scope.gff_sub = data.gff;
					// simplified version of showing graph
					// function in ss-client.js
					if($scope.selectedColorMode.value == 1) {
						showSS($scope.ssdj_sub, 'svgSS_sub', $scope.gff_sub, 'gffColorKey2');
					} else {
						showSS($scope.ssdj_sub, 'svgSS_sub', 'none', 'gffColorKey2');
					}
					
				}
			});			
		} else {
			alert(error);
		}
	};
	$scope.resetSelect = function() {
		// remove highlighted content
		// function in ss-client.js
		remove_selected($scope.sequence.length, $scope.selected);
		$scope.show_sub = false;
	};
})
.controller('completeCtrl', function($scope, $http) {
	if(!sessionStorage.getItem('download')) {
		$('.container').remove();
	} else {		
		$scope.id = sessionStorage.getItem('id');		
		$scope.code = sessionStorage.getItem('code');
		$scope.download = sessionStorage.getItem('download');
		if(sessionStorage.getItem('gffError')) {
			$scope.gffError = sessionStorage.getItem('gffError');
			$scope.show_gffError = true;
		}
		if(sessionStorage.getItem('warn_gff')) {
			$scope.warn_gff =true;
		}
		//console.log(sessionStorage.getItem('format'));
		if(sessionStorage.getItem('format') == 'dbn') {
			$scope.dbn = true;
		} else if(sessionStorage.getItem('format') == 'ct'){
			$scope.ct = true;
		}			
	}				
})
.controller('fileCtrl', function($scope, $http, $routeParams) {
	$scope.type = $routeParams.type;
	// get file content from server
	$http({
		method: 'post', 
		url: 'functions/file.php',
		headers: {'Content-Type': 'application/x-www-form-urlencoded'},
		data: $.param({id: sessionStorage.getItem('id'), type: $scope.type})
	}).success(function(data) {
		if(data) {
			$scope.download = data.download;
			$scope.content = data.content;
		}
	});	
})
.controller('imgCtrl', function($scope, $http) {
	// get file content from server
	$http({
		method: 'post', 
		url: 'functions/img.php',
		headers: {'Content-Type': 'application/x-www-form-urlencoded'},
		data: $.param({id: sessionStorage.getItem('id')})
	}).success(function(data) {
		if(data) {
			$scope.png = data.png;
			$scope.svg = data.svg;
		}
	});	
})
.controller('importCtrl', function($scope, $http, $location) {
	$scope.waiting = false;
	$scope.show_error = false;
	$scope.show_form = true;
	$scope.file_seq = '';
	$scope.file_gff = '';
	// get upload file content
	function readFile(file, onLoadCallback){
		var reader = new FileReader();
		reader.onload = onLoadCallback;
		reader.readAsText(file);
	}
	$('#file').on('change', function(e) {
		if(this.files[0]) {
			readFile(this.files[0], function(e) {
				// use result in callback...
				$scope.file_seq = e.target.result;
				document.getElementById('sequence').value = $scope.file_seq;
			});
		}
	});
	$('#gffFile').on('change', function(e) {
		if(this.files[0]) {
			readFile(this.files[0], function(e) {
				// use result in callback...
				$scope.file_gff = e.target.result;
				document.getElementById('gff_sequence').value = $scope.file_gff;
			});
		}
	});
	$scope.mySubmit = function() {
		if (check_form()) {
			// form data checked
			// file type
			$scope.file_ext = 'fa';
			// function in impportcheck.js			
			if ($('#file').val()) {
				$scope.file_ext = get_file_ext($('#file').val());
			}
			// get user id and code
			$http({
				method: 'post',
				url: 'functions/ini.php',					
				headers: {'Content-Type': 'application/x-www-form-urlencoded'},
				data: $.param({request: 'ini'})
			}).success(function(data) {
				// store id and code in mainCtrl
				sessionStorage.setItem('id', data.id);
				sessionStorage.setItem('code', data.code);	
				$scope.id = data.id;
				$scope.code = data.code;
				// hide form
				$scope.show_form = false;
				$scope.waiting = true;
				// post form data to server
				$http({
					method: 'post', 
					url: 'functions/submit.php',
					headers: {'Content-Type': 'application/x-www-form-urlencoded'},
					data: $.param({id: data.id, code: data.code, file_ext: $scope.file_ext}) + '&' + $('#formInput').serialize()
				}).success(function(data2) {
					// results
					$scope.waiting = false;
					if (angular.isString(data2) && data2 != '') {
						// error msg
						$scope.error = data2;
						$scope.show_error = true;
					} else {
						// store response data in session
						sessionStorage.setItem('format', data2.format);
						sessionStorage.setItem('gffError', data2.gffError);
						sessionStorage.setItem('warn_gff', data2.warn_gff);
						sessionStorage.setItem('download', data2.download);
						// go to complete page
						$location.path('/complete');
					}					
				});	
			});
		}		
	}
})
.controller('navCtrl', function($scope, $location) {
	$scope.tab = [];
	$scope.tab[0] = 1;
	var num = 4;
	for(i = 1; i < num; i++) {
		$scope.tab[i] = 0;
	}
	$scope.isActiveTab = function(i) {
		// import page
		//console.log($location.path());
		if ($location.path().substr(0, 7) == '/import') {
			return false;
		}
		if($scope.tab[i] == 1) {
			return true;
		} else {
			return false;
		}
	};
	$scope.changeTab = function(j) {
		for(i = 0; i < num; i++) {
			$scope.tab[i] = 0;
		}
		$scope.tab[j] = 1;
	};
})
.controller('scrollCtrl', function ($scope, $location, $anchorScroll) {
    $scope.gotoId = function(id) {
      // set the location.hash to the id of
      // the element you wish to scroll to.
      $location.hash(id);
      // call $anchorScroll()
      $anchorScroll();
    };
 })
.controller('footerCtrl', function($scope) {
	$scope.year = new Date().getFullYear();
});