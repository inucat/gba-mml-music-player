#include "mml2gba.h"
#include <stdio.h>	    // printf, fscanf, ...
#include <stdlib.h>	    // malloc, EXIT_FAILURE
#include <string.h>	    // strcpy, strtok, strchr, ...
#include <ctype.h>      // isdigit
#include "../src/songdata.h"	// MUSICCOMMAND

// #define CMD_HUMANREADABLE       // Commands are printed as four-letter enum code

/// Parse command line option.
static int ParseCommandOption(int argc, char** argv){
    int op_param = 0;
    int pit_val = 0;
    int oct_val = 0;
    int vel_val = 0;
    int fname_idx = 0;
    // Show usage if arguments are not enough
    if (argc < 2) {
        // char errmsg[MAIN_N_FNAMEMAXLEN] = {0};
        // sprintf(errmsg, "Usage: %s\tMML_FILE [-oX -pX -vX]", argv[0]);
        // die(errmsg);
        fprintf(stderr, MAIN_STR_USAGE);
        return ERR_WRONG_USAGE;
    }

	for (int i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][1] == 'p') {	        // Pitch shift
                sscanf(&(argv[i][2]), "%d", &op_param);
				pit_val = op_param;
                    //atoi(argv[++i]);
			// } else if (argv[i][1] == 't') {	// Title
			// 	szArgSongTitle = argv[++i];
			} else if(argv[i][1] == 'o') {	// Octave shift
                sscanf(&(argv[i][2]), "%d", &op_param);
				oct_val = op_param;
                    //atoi(argv[++i]);
			} else if(argv[i][1] == 'v') {	// Octave shift
                sscanf(&(argv[i][2]), "%c", &op_param);
                if (op_param == 'w')      // SMW
				    vel_val = 4;
                else if (op_param == 'm') // Mabinogi
                    vel_val = 0;
                else                    // default MIDI
                    vel_val = 3;
			}
		} else {
			fname_idx = i;
            if (i >= argc) die("File name required.");
		}
	}
}

int main (int argc, char** argv) {
	// setbuf(stdout, NULL);           // Strict output order
	if (ParseCommandOption(argc, argv))
        return FALSE;

    FILE *pfile = fopen(argv[fname_idx], "r");
	if (pfile == NULL)	{
		// perror(argv[fname_idx]);
		// exit(EXIT_FAILURE);
        return 1;
	}

	// Generates Song ID
	strcpy(szSongID, argv[fname_idx]);
	char *token = "!\"#$%&'()=~|-^\\@[`{;:]+*},./<>?";
	for (char *p = strpbrk(szSongID, token); p != NULL; p = strpbrk(p, token))
		*(p) = '_';

	printf("// %s, ID = %s\n", argv[fname_idx], szSongID);   // Prints song title

    ParseMML(pfile);     // Read and convert MML

    return 0;
}
