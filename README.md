# Patcher

This Patcher allows you to search for certain sequences of bytes inside binary files and replace them.
This being especially useful when working with constantly changing memory locations in binary files.

This tool offers various operation modes:

  - Simply double clicking will enter console interface mode where patching information is prompted from command line. For more information read below.
  - Starting it with only a file as first parameter (aka. dragging and dropping it onto the application) will read all relevant information from config and apply it to the file.
  - Information can also be supplied using start parameters, where the prefix parameter is optional.

> If the config file does not exist, an example config will be generated when needed.

### Usage

Command line interface:
```
 # Byte Patcher >_

 Manual Data Input
  Option 1: <file> <target> <replace>
  Option 2: <file> <prefix> <target> <replace>

 Enter Data:
```
 - Data is to be given seperated by spaces
```
Sample for option 1:
Enter Data: app.exe 9800000039C30F87B603000080FA5B0F 84D20200008D742600895C241C895C24

Sample for option 2:
Enter Data: app.exe 11000085C075DA8B44241C8B3D908140008D48 010FB6405A 01894C242084
```

Using start parameters in console, batch or shortcut:
```
patcher.exe app.exe 9800000039C30F87B603000080FA5B0F 84D20200008D742600895C241C895C24
or
patcher.exe data.bin 11000085C075DA8B44241C8B3D908140008D48 010FB6405A 01894C242084
```

The automatically generated config file *patcher.ini*:
```ini
; This is the configuration file for McSebi's Patcher
; Putting a prefix is not required.

[patcher]
prefix = 980039C30F8
target = 84D20200
replace = 895C241C
```

> The config file makes patching many files with the same sequence easier

Files up to 4GB should work fine, even though larger files will propably take quite a while to process.

### Version
**1.2**
 - Moved File argument to the front making it required, not having to set it was useful for development testing only anyways
 - Implemented GUI support
 - Fixed few older bugs

**1.1**
 - Improved console output
 - Skip confirmation when opened through console
 - Show confirmation when file is dropped on the executable
 - Safer handling of various strings
 - Less debugging output
 - Fixed lots of possible failiures in usage

**1.0**
 - Initial Release

### Dependencies for Running
* none

### Dependencies for Building
patcher.exe:
* [inih] - Simple .INI file parser in C, good for embedded systems
* [mingw32] - Minimalist GNU for Windows
* [stdinc.c] - Personal STDINC file

patcher_gui.exe:
* [AutoIt3] - A free BASIC-like scripting language designed for automating the Windows GUI and general scripting. 

   [inih]: <https://github.com/benhoyt/inih>
   [mingw32]: <http://www.mingw.org/>
   [stdinc.c]: <https://mcsebi.ru/patcher/stdinc.c>
   [stdinc.c]: <https://www.autoitscript.com/site/autoit/>

License
----

MIT