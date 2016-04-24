How to use:

This patcher can be used in the following ways:
 
 # Mode		# Parameters										# Description
 Mode A:    patcher.exe											Starts the patcher in console interface mode
 Mode B:	patcher.exe <File>									Loads patching information from ini file
 Mode C:	patcher.exe <Target> <Replace> [file]				Replaces the target hex with the replace hex bytes
 Mode D:	patcher.exe <Prefix> <Target> <Replace> [file]		Searches for Prefix+Target string and replaces Target with Replace
 
[parameter] means that the parameter is optional. [file] will always default to input.txt if not given.
If the config file does not exist, an example config will be generated on first run in Mode B.