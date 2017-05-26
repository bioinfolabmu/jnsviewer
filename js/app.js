var app = angular.module('myApp', ['ngRoute']);
app.config(['$routeProvider', function($routeProvider) {
	$routeProvider	
	.when('/manual', {
		templateUrl: 'content/manual.html'
	})		
	.when('/retrieve', {
		templateUrl: 'content/retrieve.html'
	})
	.when('/retrieve/:id', {
		templateUrl: 'content/retrieve.html'
	})
	.when('/contact', {
		templateUrl: 'content/contact.html'
	})		
	.when('/home', {
		templateUrl: 'content/import-2.html'
	})		
	.when('/complete', {
		templateUrl: 'content/complete.html'
	})
	.when('/ss', {
		templateUrl: 'content/ss.html'
	})
	.when('/file/:type', {
		templateUrl: 'content/file.html'
	})
	.when('/img', {
		templateUrl: 'content/img.html'
	})
	.otherwise('/home');
}]);