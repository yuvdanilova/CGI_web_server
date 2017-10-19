# CGI WEB SERVER

This is a simple UNIX web server in C++ implemented by CS HSE student Yulia Danilova as part of Fall 2015 project work.

## functionality

* Serves multiple clients at the same time
* Handles static content
* Handles dynamic content
* Uses both GET and POST methods

The server supports multiple connections creating a new process for every incoming request. It parses the HTTP/1.1 request, checks if the file (or the folder) does exist on the server side, and after that forms a response depending on the file type.

Files of _html, css, javascript_ types and _bmp, jpeg, png_ images ar supported. __Static files__ of other types are returned as plain text.

If __executable file__ (a CGI script) is requested, dynamic content is generated on the server and sent to the client.

If the server gets __empty__ or __folder requests__, it searches for a file "index.*" (preferrably index.htm or index.html) in this folder.

## how to use it

To run this server you have to work on *UNIX operating system* and have *CMake* installed of *version 3.3* or later.

To check your current version of CMake type
```
cmake --version
```
in terminal. If cmake is not installed, you will get a "not found" error.

You can get the latest release on https://cmake.org/download/

Here is an example how to install CMake 3.4.1 on Ubuntu:
```
sudo apt-get install build-essential
wget https://cmake.org/files/v3.4/cmake-3.4.1.tar.gz
tar xzf cmake-3.4.1.tar.gz

//For security reason, we move this folder under “/opt” location.

sudo mv cmake-3.4.1 /opt/
./configure
make

//If make finishes without errors:

sudo make install
export PATH=/usr/local/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

## How to compile the server
First, clone the repository located at __repo__ into the folder called __directory__ on the local machine using the following command
```
git clone <repo> <directory>
```

Then type
```
cmake <directory>
```
where __directory__ is your web server source folder to create a Makefile.

Change your current working directory to __directory__ and run make
```
cd <directory>
make
```
Then you will get an executable file of, _for example_, name "cgi".

## how to start the server
The server gets __zero__, __one__ or __two__ arguments of two types:

* "path=PATH", where PATH is a full path to a directory where you store content. 

_For example, "path=/home/user1/cgi/examples"_

* "port=PORT", where PORT is a number of port you would like to use (might be unavailable, then you will get an error)

_For example, "port=8800"_

If you do not provide a source folder, it will be set to your current working directory.

If you do not provide a port, it will be set to a random available port.

After you started a server, you will get a message "Socket created" and information about your server's port and source directory.

_For example,_
```
$ ./cgi path=/home/user1/cgi/examples
Socket created
PORT=33225 DIRECTORY=/home/user1/cgi/examples
```

## how to use the server
To check the work of your server you can use any browser with, if needed, special plug-ins for generating GET or POST requests (e.x. Postman for Google Chrome).

Type into an address line of your browser something like

http://localhost:PORT/your_request?arg1&arg2

where PORT is the number of a port that you established your server on.
For further information search for "GET and POST requests".

Every time the server receives a request from a client you will see a message like that:
```
METHOD: POST URL: /home/user1/cgi/examples/prog1 QUERY: name=Michael
```
And the result of the action will be sent back to the client's browser.

To stop the server press __"Ctrl+\"__ combination on the keyboard.
