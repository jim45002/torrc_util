# torrc_utility
Qt GUI for some torrc options. Torrc options supported are:

-ExcludeNodes (list of identity fingerprints, country codes, and address patterns of nodes to avoid when building a circuit)

-ExitNodes (list of identity fingerprints, country codes, and address patterns of nodes to use as exits) 

-EntryNodes (list of  identity fingerprints, country codes, and address patterns of nodes to use for the first hop) 

-EnforceDistinctSubnets (If 1, Tor will not put two servers whose IP addresses are "too close" on the same circuit)

-UseEntryGuards (If this option is set to 1, we pick a few long-term entry servers, and try to stick with them)

-NumEntryGuards (If UseEntryGuards is set to 1, we will try to pick a total of NUM routers as long-term entries for our circuits)


Only 2-letter country abbreviations are supported for ExcludeNodes, ExitNodes, and EntryNodes. See https://2019.www.torproject.org/docs/tor-manual.html.en for complete descritpion of these options.






