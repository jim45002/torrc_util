# Torrc Utility
Torrc Utility is a simple Qt GUI for managing some options in Tor's configuration file. This application was only tested with the default torrc file included with the Tor-Browser bundle. Before any changes are made to the torrc file, a copy is stored in the current directory. Torrc Utility is also used for graphically viewing the list of active Tor nodes. You can double-click a name on the countries list to view its active nodes and location on the GUI's map. This source code was built using the cmake build tool. Build instructions will be added to this README soon. If you want to build the source and need step-by-step instructions, contact teamplayer21@tutanota.com for help with building the application.



![Alt text](torrc_utility_gui.png?raw=true "Torrc Utility")



Torrc options supported are:

	ExcludeNodes (list of identity fingerprints, country codes, and address patterns of nodes to avoid when building a circuit)

	ExitNodes (list of identity fingerprints, country codes, and address patterns of nodes to use as exits) 

	EntryNodes (list of  identity fingerprints, country codes, and address patterns of nodes to use for the first hop) 

	EnforceDistinctSubnets (If 1, Tor will not put two servers whose IP addresses are "too close" on the same circuit)

	UseEntryGuards (If this option is set to 1, we pick a few long-term entry servers, and try to stick with them)

	NumEntryGuards (If UseEntryGuards is set to 1, we will try to pick a total of NUM routers as long-term entries for our circuits)


Only 2-letter country abbreviations are supported for ExcludeNodes, ExitNodes, and EntryNodes. See https://2019.www.torproject.org/docs/tor-manual.html.en for complete descritpion of these options.






