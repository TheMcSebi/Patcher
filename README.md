# Patcher

This Patcher allows you to replace certain sequences of bytes inside binary files.

This tool offers various operation modes:

  - Starting it without parameters automatically enters console interface mode where patching information is prompted on command line. For more information read below.
  - Starting it with a file as first parameter (aka. dragging and dropping it onto the application) will read all relevant information from config file.
  - Providing Target-Hex, Replace-Hex and optionally a Filename will operate based on these given information.
  - Providing Prefix-Hex, Target-Hex, Replace-Hex and optionally a Filename will stick Prefix and Target together and search for only these occurrences.

> If no filename is given, the parameter will default to "input.txt".

> If the config file does not exist, an example config will be generated on first run in this mode.

### Usage

Sample for mode A:
```
patcher.exe

 + + + + + McSebi's Bytepatcher + + + + +

 Modes
1 = <target> <replace> [file]
2 = <prefix> <target> <replace> [file]

Enter Mode: 
```
Options: 1 or 2
```
Enter Data: 
```
Options: Depende on the mode

Mode 1 could be used like
```
9800000039C30F87B603000080FA5B0F 84D20200008D742600895C241C895C24 app.exe
```

An example for mode 2 would be
```
11000085C075DA8B44241C8B3D908140008D48 010FB6405A 01894C242084 app.exe
```
### Version
**1.0.0.0**
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