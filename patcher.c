#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini/ini.h"
#include "ini/ini.c"
#include <math.h>
#include <unistd.h>
#include "stdinc.c"

// global & dev
unsigned short debug = 1;
unsigned short confirmation_interrupts = 0;

typedef struct
{
    const char* prefix;
    const char* target;
    const char* replace;
} configuration;

static int conf_handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    if (MATCH("patcher", "prefix")) {
        pconfig->prefix = strdup(value);
    } else if (MATCH("patcher", "target")) {
        pconfig->target = strdup(value);
    } else if (MATCH("patcher", "replace")) {
        pconfig->replace = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void conf_create(char* conf_name) {
  FILE* conftemp = NULL;

  conftemp = fopen(conf_name, "w");
  if(conftemp != NULL) {
    fputs("; This is the configuration file.\n", conftemp);
    fputs("; Setting a prefix is optional.\n\n", conftemp);
    fputs("[patcher]\n", conftemp);
    fputs("prefix = 980039C30F8\n", conftemp);
    fputs("target = 84D20200\n", conftemp);
    fputs("replace = 895C241C", conftemp);
    fclose(conftemp);

    printf("%s successfully created.\n", conf_name);
  } else {
    printf("Could not create %s\n", conf_name);
  }
}

void getOutputFile(char* out, char* in) {
  int inlen = strlen(in);
  if(strstr(in, ".") != NULL) {
    int k = 0;
    while(in[inlen-k] != '.') {
      k++;
    }
    strncpy(out, in, inlen-k);
    append(out, "_patched\0");
    append(out, &in[inlen-k]);
  } else {
    strcpy(out, in);
    append(out, "_patched");
  }
  if(strstr(in, "\\") != NULL) {
    // On windows always the full path is parsed when dropping a file onto an executable
    // If this is the case, let's not close the console windows right after finishing
    confirmation_interrupts = 1;
  }
}

//////////////////////////////////////////////////////
//
//    Main Function & Initialization
//
//////////////////////////////////////////////////////

int main(int argc, char** argv) {
  system("cls");
  printf(" + + + + + McSebi's Bytepatcher + + + + +\n\n");

  // Internal Variable Handling
  char* inputstring = NULL;
  int varcount = 0;
  char argprefix[] = "--\0";

  // ascii hex strings
  char pr[1024] = {0};
  char ta[1024] = {0};
  char re[1024] = {0};

  // file names and file input stuff
  char filestr[128] = {0};
  char input_file[128] = {0};
  char output_file[128] = {0};
  char config_file[128] = {0};
  strcpy(filestr, "input.txt\0");
  strcpy(config_file, "patcher.ini\0");

  // raw hex length
  int plen, tlen, rlen;

  // actual hex tables
  char prefix[1024] = {0};
  char target[1024] = {0};
  char replace[1024] = {0};

  unsigned short mode = 0;
  unsigned short guimode = 0;

  // file and hex stuff
  FILE* input = NULL;
  FILE* output = NULL;
  FILE* config = NULL;
  int filesize = 0;
  int targetsize = 0;
  int slen = 0;

  // Inner Loop Stuff
  int occurrences[4096] = {0};
  int percent = 0;
  int percentLogged = 0;

  // Loops
  int occ = 0;
  long int i = 0;
  int j;
  int r;

  //////////////////////////////////////////////////////
  //
  //    Engine Parameter Parsing
  //
  //////////////////////////////////////////////////////


  for(r = 0; argv[r] != NULL; r++) {

    char arg[128] = {0};
    if(memcmp(argv[r], argprefix, strlen(argprefix)) == 0) {
      // arg found lol
      strcpy(arg, &argv[r][strlen(argprefix)]);
      int tempr = r + 1; // Move all other following parameters one to the front in order to prevent mistakes when parsing the actual parameters
      while(argv[tempr] != NULL) {
        argv[tempr-1] = argv[tempr];
        tempr++;
      }
      argc--;

      if(memcmp(arg, "gui", 3) == 0) {
        guimode = 1;
        confirmation_interrupts = 0;
        printf("gui mode enabled\n", arg);
      }
    }
  }

  //////////////////////////////////////////////////////
  //
  //    Parsing all kinds of inputs into buffer
  //        First: Checking if start parameters are given
  //
  //////////////////////////////////////////////////////

  if(argc == 2) {
    // Mode 0: <file> & read from config file
    strcpy(filestr, argv[1]);

    // Do config parsing...
    configuration conf;
    if (ini_parse(config_file, conf_handler, &conf) < 0) {
        printf("Can't load '%s'\n", config_file);
        printf("Creating it...\n");
        conf_create(config_file);

        if (ini_parse(config_file, conf_handler, &conf) < 0) {
            printf("Unable to load '%s' on second attempt.\n", config_file);
            getchar();
            return 1;
        }
    }

    printf("Config loaded from '%s':\n prefix='%s'\n target='%s'\n replace='%s'\n\n", config_file, conf.prefix, conf.target, conf.replace);

    strcpy(pr, conf.prefix);
    strcpy(ta, conf.target);
    strcpy(re, conf.replace);

    /*
    Notice:
    const char* prefix;
    const char* target;
    const char* replace;
    */

  } else if(argc == 3) {
    // Mode 1: <target> <replace>
    strcpy(ta, argv[1]);
    strcpy(re, argv[2]);

  } else if(argc == 4) {
    // Detect if mode 1 or 2 by checking if argv[3] is a file

    char argtest[128] = {0};
    strcpy(argtest, argv[3]);
    FILE* argfile = NULL;
    argfile = fopen(argtest, "rb");
    if(argfile != NULL) {
      // Mode 1 <target> <replace> <file>
      strcpy(ta, argv[1]);
      strcpy(re, argv[2]);
      strcpy(filestr, argv[3]);

    } else {

      // Mode 2 <prefix> <target> <replace>
      strcpy(pr, argv[1]);
      strcpy(ta, argv[2]);
      strcpy(re, argv[3]);
    }
    fclose(argfile);
  } else if(argc == 5) {
      // Mode 2 <prefix> <target> <replace> <file>

      strcpy(pr, argv[1]);
      strcpy(ta, argv[2]);
      strcpy(re, argv[3]);
      strcpy(filestr, argv[4]);

  } else {
    //////////////////////////////////////////////////////
    //
    //    Parsing all kinds of inputs into buffer
    //        If no parameters are given: Parse input manually
    //
    //////////////////////////////////////////////////////
    confirmation_interrupts = 1;

    printf(" Modes\n  1 = <target> <replace> [file]\n  2 = <prefix> <target> <replace> [file]\n\n");
    printf("Enter Mode: ");
    scanf("%hu", &mode);
    getchar(); // clear buffer after dirty scanf

    printf("\nEnter Data: ");
    inputstring = getline();

    varcount = 1 + countChars(inputstring, ' ');

    if(debug == 1) {
      printf(" varcount = %d\n", varcount);
    }

    if(mode == 1) { // Mode 1: <target> <replace> [file]
      if(varcount == 2) { // If filename is not given
        sscanf(inputstring, "%s %s", ta, re);
      } else if(varcount == 3) { // If filename is given
        sscanf(inputstring, "%s %s %s", ta, re, filestr);
      } else {
        printf("Error: Unsupported Input Format for Mode 1\n");
      }
    } else if(mode == 2) { // Mode 2: <prefix> <target> <replace> [file]
      if(varcount == 3) { // If filename is not given
        sscanf(inputstring, "%s %s %s", pr, ta, re);
      } else if(varcount == 4) { // If filename is given
        sscanf(inputstring, "%s %s %s %s", pr, ta, re, filestr);
      } else {
        printf("Error: Unsupported Input Format for Mode 2\n");
      }
    } else {
      printf("Error: Unsupported Mode\n");
    }

    printf("\n Filename: %s\n", filestr);

    if(debug == 1) {
      printf(" Length of Filename = %d\n", strlen(filestr));
    }

    if(confirmation_interrupts == 1) {
      printf("\nEnter to continue...");
      getchar();
    }

  } // if(argc == n) end

  //////////////////////////////////////////////////////
  //
  //    Displaying all kinds of information and preparing next steps
  //
  //////////////////////////////////////////////////////

  plen = strlen(pr)/2;
  tlen = strlen(ta)/2;
  rlen = strlen(re)/2;

  strcpy(input_file, filestr);

  getOutputFile(output_file, input_file);

  printf("\n INPUT = %s\n OUTPUT = %s\n", input_file, output_file);

  if(tlen == rlen) {
    printf("\n Length of prefix hex: %d\n Length of section hex: %d\n", plen, tlen);
  } else {
    printf("Error: Target length does not match Replace length.\n");
    getchar();
    return 1;
  }

  // Convert ASCII strings to binary hex arrays
  hexStr(pr, prefix);
  hexStr(ta, target);
  hexStr(re, replace);

  //////////////////////////////////////////////////////
  //
  //    Start reading the files
  //
  //////////////////////////////////////////////////////

  slen = plen+tlen;

  printf("\n Prefix = '%s'\n Target = '%s'\n Replace = '%s'\n", prefix, target, replace);

  input = fopen(input_file, "rb"); // Use binary mode to prevent headache
  if(input != NULL) {
    filesize = fgetsize(input);
    printf("\nInput file opened successfully.\n");

    char* scanner = (char*)malloc(slen); // Initialize scanner array treating it as low level as possible
    memset(scanner, 0, slen);
    memcpy(scanner, prefix, plen);
    memcpy(&scanner[plen], target, tlen);

    printf("\n Sequence to search for = '");
    printstr(scanner, slen);
    printf("'");

    if(slen > 1 || slen == 0) {
      printf(" Total length = %d bytes\n", slen);
    } else {
      printf(" Total length = %d byte\n", slen);
    }

    char* buffer = (char*)malloc(slen); // Medieval like preparation of a char array
    memset(buffer, 0, slen);

    printf(" Filesize = %d bytes\n", filesize);

    if(confirmation_interrupts == 1) {
      printf("\nEnter to continue...");
      getchar();
    }

    output = fopen(output_file, "wb"); // Non-binary mode apparently does various magic to newlines
    if(output != NULL) {
      printf("Output opened successfully.\n");
      //////////////////////////////////////////////////////
      //
      //    Inner Loop Start
      //
      //////////////////////////////////////////////////////

      while(i < filesize - slen) { // INCREMENT I ON HOW MANY BYTES HAVE BEEN WRITTEN
        fseek(input, i, SEEK_SET);
        fread(buffer, sizeof(char), (unsigned)slen, input);

        if(memcmp(scanner, buffer, slen) == 0) {
          // if bytesequence is found write replace string to output
          fwrite(prefix, sizeof(char), plen, output);
          i = i + plen;
          occurrences[occ] = i;
          occ++;
          fwrite(replace, sizeof(char), rlen, output);
          i = i + rlen;

        } else {
          // if nothing found, write byte to output
          i++;
          fwrite(&buffer[0], sizeof(char), 1, output);
        }


        if(guimode == 1) { // Add support for GUI
          percent = floor(100.0*i/filesize);
          //printf("percent: %d\n", percent);
          if(percent > percentLogged) {
            percentLogged = percent;
            printf("%d\n", percent);
          }
        }
      }

      // After while condition has run out write the last n character to file
      // Currently this makes it impossible to replace a certain byte sequence at the very end of the file
      fseek(input, i, SEEK_SET);
      fread(buffer, sizeof(char), (unsigned)slen, input);
      fwrite(buffer, sizeof(char), slen, output);

      // Print out all information on where the string has been located at
      printf("\nA total of %d occurrences has been patched:\n", occ);
      for(j = 0; j < occ; j++) {
        int o = occurrences[j];
        printf("%d: Location: 0x%x dec(%d)\n", j+1, o, o);
      }

      targetsize = fgetsize(output);
      // Everything should be done. Closing files.
      fclose(input);
      fclose(output);

      if(filesize != targetsize) {
        printf("Output size does not match input size.");
        getchar();
        return 1;
      }

      printf("\nFinished.\n");
    } else {
      printf("Error: Failed to open output file for writing.\n");
    }

  } else {
    printf("Error: Failed to open input file for reading.\n");
  }

  // Wait if opened through shortcut (one of the intended useages)
  if(confirmation_interrupts == 1) {
    printf("\nEnter to continue...");
    getchar();
  }
  return 0;
}
