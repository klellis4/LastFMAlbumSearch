# MusicAlbumSearch
GUI application that uses a Last.FM API key to search and display albums and tracks by artist.


### Java Application

This project is designed to run on either Debian Buster Linux or MacOS. It
requires jdk13.

Building and running the sample is done with Ant.
This example depends on the following frameworks:
1. Ant
   see: http://ant.apache.org/
2. Json for Java as implemented (reference) by Doug Crockford.
   See: https://github.com/stleary/JSON-java
3. Gluon's JavaFX for Java13 on Linux.
   See: https://gluonhq.com/products/javafx/

To build and run the example, you will need to have Ant installed on
your system, with the antlib target extensions

#### Run the server with the command:
`ant server -DhostId=localhost -DregPort=1099`

#### Run the client with the command:
`ant client -DhostId=localhost -DregPort=1099 -Dlastfmkey=MyLastFMKey`

Note: For running on a raspberry pi, the hostId's in each of the above commands would be replaced with 
the RPi's hostname/IP. The clients would be run on Linux, and the server would be run on the RPi.

#### To clean the project directory:
ant clean

#### Java Server Running on Raspberry Pi:

![Image of Server](https://github.com/klellis4/MusicAlbumSearch/blob/master/server_java.png)


#### Java Client Running:

![Image of Client](https://github.com/klellis4/MusicAlbumSearch/blob/master/client_java.png)


### C++ Application

This program is has been tested on Linux and MacOS (build.xml only works on
Linux).

To execute Ant using the build.xml in this directory, you will need to
copy the file: antlibs.jar from the lib directory to your home directory:
cp lib/antlibs.jar ~
or
cp lib/antlibs.jar $HOME/
Note that ~ (tilde) is a shortcut for $HOME
then extract the antlibs.jar file:
pushd ~
jar xvf antlibs.jar
pushd -0
The pushd commands manipulate a stack of directories for switching your
bash's current directory. The first pushd pushes home onto the stack and
switches the current directory to home. The second pushd takes you
back to whatever directory you were in before the first.

To compile the GUI and to do last.fm searches, you must also provide the Curlpp
library, which is a C++ wrapper for cURL. See:  http://www.curlpp.org/
This example depends on the library curlpp to download a file or make
a URL request. This should be used in problem solutions to search
last.fm for album and track information. Download the library from: 
  http://www.curlpp.org/
Extract the archive and from a terminal change to the directory created.
Execute the following commands looking for errors. Note the following steps
require that libcurl first be installed, if its not already present.
   cmake ./
   make
   sudo make install
   sudo ldconfig
To see the make options available do: make help

#### To run the server:
`ant server -Dport=1099`

#### To run the client(s):
`ant client -Dhost=localhost -Dport=1099 -DuserId=Your.Name -Dlastfmkey=Your.fm.key`

Note: For running the server on a RaspberryPi, the host in the above commands would be replaced with the RPi's
hostname/IP.

To clean the project (remove the executable files) execute:
ant clean


#### C++ Server Running on a RaspberryPi:

![Image of C++ Server](https://github.com/klellis4/MusicAlbumSearch/blob/master/server_cpp.png)


#### C++ Client Running:

![Image of C++ Client](https://github.com/klellis4/MusicAlbumSearch/blob/master/client_cpp.png)

