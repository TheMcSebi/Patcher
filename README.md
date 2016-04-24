# Patcher

This Patcher allows you to search for certain sequences of bytes inside binary files and replace them.
This being especially useful when working with constantly changing memory locations in binary files.

This tool offers various operation modes:

  - Starting it without parameters automatically enters console interface mode where patching information is prompted on command line. For more information read below.
  - Starting it with a file as first parameter (aka. dragging and dropping it onto the application) will read all relevant information from config file.
  - Providing Target-Hex, Replace-Hex and optionally a Filename will operate based on these given information.
  - Providing Prefix-Hex, Target-Hex, Replace-Hex and optionally a Filename will stick Prefix and Target together and search for only these occurrences.

> If no filename is given, the parameter will default to "input.txt".

> If the config file does not exist, an example config will be generated on first run in this mode.

### Usage

Command line interface:
```
 + + + + + McSebi's Bytepatcher + + + + +

 Modes
1 = <target> <replace> [file]
2 = <prefix> <target> <replace> [file]

Enter Mode: n
Enter Data: data
```
 - *n* being either 1 or 2
 - *data* depending on the choosen mode
```
Sample Mode 1:       
Enter Data: 9800000039C30F87B603000080FA5B0F 84D20200008D742600895C241C895C24 app.exe

Sample Mode 2:
Enter Data: 11000085C075DA8B44241C8B3D908140008D48 010FB6405A 01894C242084 app.exe
```

Using start parameters:
```
patcher.exe 9800000039C30F87B603000080FA5B0F 84D20200008D742600895C241C895C24 app.exe
or
patcher.exe 11000085C075DA8B44241C8B3D908140008D48 010FB6405A 01894C242084 data.bin
```
>If the filename is not given, *input.txt* will be opened.

The automatically generated config file *patcher.ini*:
```ini
; This is the configuration file for McSebi's Patcher
; Putting a prefix is not required.

[patcher]
prefix = 980039C30F8
target = 84D20200
replace = 895C241C
```

> The config file makes changing many files patching the same sequence easier

Files up to 4GB should work fine, even though larger files will propably take quite a while to process.

### Version
**1.1**
 - Improved console output
 - Skip confirmation when opened through console
 - Show confirmation when file is dropped on the executable
 - Safer handling of various strings
 - Less debugging output
 - Fixed lots of possible failiures in usage
 
**1.0**
 - Initial Release

### Dependencies for Building

* [inih] - Simple .INI file parser in C, good for embedded systems
* [mingw32] - Minimalist GNU for Windows
* [stdinc.c] - Personal STDINC file

   [inih]: <https://github.com/benhoyt/inih>
   [mingw32]: <http://www.mingw.org/>
   [stdinc.c]: <https://mcsebi.ru/patcher/stdinc.c>

License
----

MIT