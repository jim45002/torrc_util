# Torrc Utility
Torrc Utility is a simple Qt GUI for managing some options in Tor's configuration file. To learn about Tor go here https://www.torproject.org. This application was only tested on Ubuntu 16.04 LTS distro, with the default torrc file included with the Tor-Browser bundle. Before any changes are made to the torrc file, a copy is stored in the current directory. Torrc Utility is also used for graphically viewing the list of active Tor nodes. Double-click a name in the countries list to view its active nodes, and location on the GUI's map.


![Alt text](./resources/torrc_utility_gui.png?raw=true "Torrc Utility")


Torrc options supported are:

<ul>
  <li><b>ExcludeNodes</b>: List of identity fingerprints, country codes, and address patterns of nodes to avoid when building a circuit</li>
  <li><b>ExitNodes</b>: List of identity fingerprints, country codes, and address patterns of nodes to use as exits</li>
  <li><b>EntryNodes</b>: list of identity fingerprints, country codes, and address patterns of nodes to use for the first hop  </li>
 
 <li><b> HSLayer2Nodes</b>: A list of identity fingerprints, nicknames, country codes, and address patterns of nodes that are allowed to be used as the second hop in all client or service-side Onion Service circuits</li>

 <li><b> HSLayer3Nodes</b>: A list of identity fingerprints, nicknames, country codes, and address patterns of nodes that are allowed to be used as the third hop in all client and service-side Onion Service circuits</li>

<li><b>  ExcludeExitNodes</b>: A list of identity fingerprints, country codes, and address patterns of nodes to never use when picking an exit node---that is, a node that delivers traffic for you outside the Tor network</li>

<li><b>  ExcludeExitNodes</b>: A list of identity fingerprints, country codes, and address patterns of nodes to never use when picking an exit node---that is, a node that delivers traffic for you outside the Tor network</li>
 <li><b>EnforceDistinctSubnets</b>: If 1, Tor will not put two servers whose IP addresses are "too close" on the same circuit</li>

 <li><b>UseEntryGuards</b>: If this option is set to 1, we pick a few long-term entry servers, and try to stick with them</li>

 <li><b>NumEntryGuards</b>: If UseEntryGuards is set to 1, we will try to pick a total of NUM routers as long-term entries for our circuits</li>

 <li><b>StrictNodes</b>: If StrictNodes is set to 1, Tor will treat solely the ExcludeNodes option as a requirement to follow for all the circuits you generate, even if doing so will break functionality for you (StrictNodes applies to neither ExcludeExitNodes nor to ExitNodes, nor to MiddleNodes). If StrictNodes is set to 0, Tor will still try to avoid nodes in the ExcludeNodes list, but it will err on the side of avoiding unexpected errors. </li>

</ul>


Only 2-letter country abbreviations, IPv4 and IPv6 node addresses are supported for node identities. See https://2019.www.torproject.org/docs/tor-manual.html.en for complete descritpion of these options.


# Build Instructions
This source code was built using the cmake build tool. The instrutions below may also work on Debian. As with any software application, there will be bugs in the source code. Please send any issues you find with the application, or any questions you have, to the Bitmessage address BM-2cWzE6TNpMDuaA9ZcLubBfxderyTsnKQ8n or email hotmethods@protonmail.com with any questions. If you have problems with the build instruction please send the details to the previous mentioned email addresses.

To build torrc_util on Ubuntu Linux install you'll need Qt5 library, MarbleWigdet library, and Curl commmand-line utility:

<b>Qt5</b>: To install Qt5 go to https://www.qt.io/download and follow the download instructions. Extract the zipped file and when building the libraries that depend on Qt5, make sure cmake points to the downloaded Qt5 cmake libs, usually (Qt5.x.x/5.x.x/gcc_64/lib/cmake/the-qt-library) in the extracted download.

<b>MarbleWidget</b>: Marble can be downloaded at 'https://github.com/KDE/marble', you'll need to build the source code and install it. To build the source, extract the zipped file, cd into the directory created by the extracted zipped file, create a directory called build or whatever name you choose, cd into that directory, at the command-line type 'cmake-gui ../' or 'cmake ../' if there are any path issues cmake will report the errors. It's best to use cmake-gui to fix the path errors (note: Marble depends on Qt5, make sure the Marble CMakeLists.txt points to your downloaded Qt5), when cmake-gui returns errors you'll see a highlighted list, click the items in list to find the library paths that weren't found. The easiest way to do this is by using cmake-gui tool. After cmake generates the build files, type 'make' at the commandline to build the source code. Cmake-gui can be installed on Ubuntu at the command line using 'sudo apt install cmake-gui', use cmake-gui just as cmake is used. When marblewidget is finished building install it at the command-line using 'sudo make install' 

<b>Curl</b>: Curl comes with the default Ubuntu installation. It can be installed at the command line using 'sudo apt install curl'

<b>torrc_util</b>: Torrc_util uses the cmake build tool. The easist way to build it is by using the cmake-gui tool mentioned above. If Marble widget or Qt5 isn't picked up in your system paths, cmake-gui will allow you to set these paths in the gui. Curl should already be in the system path, you can verfy this by typing '/usr/bin/curl --version' at the command line and pressing enter.

<b>map_widget_factory</b> Map_widget_fatory is a library under the torrc_util project directory. Map_widget_factory is under the libs directory, it depends on MarbleWidget. Use the cmake, or cmake-gui tool to build it. After building the library go to the the build directory and do 'sudo make install' to install the library into your path where torrc_util can find it.  


If you find that these instruction are incomplete please feel free to contact the email or bitmessage address above.









