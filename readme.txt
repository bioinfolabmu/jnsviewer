1. How to deploy the application

1.1 Installation of pre-requisite software

compiler and library:

# sudo apt-get install g++ gcc gfortran libgd-dev


1.2 Web code

Copy all the files in this folder to your desired location in the web server;
 
The direcotry 'users/' will hold all the users' result files, and you need to change the permisson to '753; (Here, assume that you are not in 'www-data' group)

Change the permission of 'exe/*' to '755';
