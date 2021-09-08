/**
 * @file mml2dmg.c
 * @brief Convert MML to my DMG Sound Driver data
 * @author inucat(@inucat4)
 * @note fscanf() does not change the file cursor when it failed.
 */
#include <stdio.h>	    // printf, fscanf, ...
#include <stdlib.h>	    // malloc, EXIT_FAILURE
#include <string.h>	    // strcpy, strtok, strchr, ...
#include <ctype.h>      // isdigit
#include "../src/songdata.h"	// MUSICCOMMAND

// #define CMD_HUMANREADABLE       // Commands are printed as four-letter enum code
#define MAXLEN_FILENAME 256

// Stores commandline arguments
int nPitchShift = 0;
int nOctaveShift = 0;
int nArgIdxOfFilename = 0;
int nVelBitShift = 0;

// Global variables
char szSongID[256] = {0};

static void die(const char* szErrMes);
static void PutCommand(int cmd);
static void PutCommandWithValue(int cmd, unsigned char bValue);
static void SeekLengthOption(FILE* pFile, int cmd);
static int SeekDots(FILE* pFile, int cmd);

int SeekOption(FILE *, int);
void ParseCommandOption(int, char**);
void ReadMML(FILE*);

int main (int argc, char** argv) {
	setbuf(stdout, NULL);           // Strict output order
	ParseCommandOption(argc, argv);

    FILE *pFile = fopen(argv[nArgIdxOfFilename], "r");
	if (pFile == NULL)	{
		perror(argv[nArgIdxOfFilename]);
		exit(EXIT_FAILURE);
	}

	// Generates Song ID
	strcpy(szSongID, argv[nArgIdxOfFilename]);
	char *token = "!\"#$%&'()=~|-^\\@[`{;:]+*},./<>?";
	for (char *p = strpbrk(szSongID, token); p != NULL; p = strpbrk(p, token))
		*(p) = '_';

	printf("// %s, ID = %s\n", argv[nArgIdxOfFilename], szSongID);   // Prints song title

    ReadMML(pFile);     // Read and convert MML

    return 0;
}

static void die(const char *errmsg) {
    fprintf(stderr, "\n\n<!> %s\n", errmsg);
    exit(EXIT_FAILURE);
}

#ifdef CMD_HUMANREADABLE
static void PutCommand(int cmd)
{
    switch (cmd) {
        case TERM: printf("TERM");
            return;
        case REST: printf("REST, ");
            return;
        case TPDC: printf("TPDC, ");
            return;
        case DOTT: printf("DOTT, ");
            return;
        case RPHD: printf("RPHD, ");
            return;
        case RPTL: printf("RPTL, ");
            return;
        case TMCG: printf("TMCG, ");
            return;
        case DRCG: printf("DRCG, ");
            return;
        case DRDT: printf("DRDT, ");
            return;
        case PCSF: printf("PCSF, ");
            return;
        case VECG: printf("VECG, ");
            return;
        default: printf("%d, ", cmd);
            return;
    }
}

static void PutCommandWithValue(int cmd, unsigned char val)
{
    PutCommand(cmd);
    printf("%d, ", val);
}
#else
static void PutCommand(int cmd) { printf("%d, ", cmd); }
static void PutCommandWithValue(int cmd, unsigned char val) { printf("%d, %d, ", cmd, val); }
#endif

static void SeekLengthOption(FILE *pFile, int cmd) {
    int lopt;
    if(fscanf(pFile, "%d", &lopt) > 0)
        PutCommandWithValue(cmd, lopt);
}

/// Seeks any dots.
/// @return TRUE if any found, FALSE otherwise
static int SeekDots(FILE *pFile, int cmd) {
    int fDot=0;
    fpos_t *pos = (fpos_t *) malloc(sizeof(fpos_t *));
    fgetpos(pFile, pos);
    if (fgetc(pFile) == '.')
    {
        fDot = 0b01;

        fgetpos(pFile, pos);
        if (fgetc(pFile) == '.')    // judge double dots or not
            fDot = 0b11;
        else
            fsetpos(pFile, pos);

        PutCommandWithValue(cmd, fDot);
    }
    else fsetpos(pFile, pos);
    return fDot && 1;
}

/// Returns a command argument.
/// @param nDefault	Returned when error
int SeekOption(FILE *pFile, int nDefault) {
	fscanf(pFile, "%d", &nDefault);
	return nDefault;
}

/// Parse command line option.
void ParseCommandOption(int argc, char** argv){
    // Show usage
    if (argc < 2) {
        char errmsg[MAXLEN_FILENAME] = {0};
        sprintf(errmsg, "Usage: %s\tMML_FILE [-oX -pX -vX]", argv[0]);
        die(errmsg);
    }

    int nOpVal = 0;
	for (int i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][1] == 'p') {	        // Pitch shift
                sscanf(&(argv[i][2]), "%d", &nOpVal);
				nPitchShift = nOpVal;
                    //atoi(argv[++i]);
			// } else if (argv[i][1] == 't') {	// Title
			// 	szArgSongTitle = argv[++i];
			} else if(argv[i][1] == 'o') {	// Octave shift
                sscanf(&(argv[i][2]), "%d", &nOpVal);
				nOctaveShift = nOpVal;
                    //atoi(argv[++i]);
			} else if(argv[i][1] == 'v') {	// Octave shift
                sscanf(&(argv[i][2]), "%c", &nOpVal);
                if (nOpVal == 'w')      // SMW
				    nVelBitShift = 4;
                else if (nOpVal == 'm') // Mabinogi
                    nVelBitShift = 0;
                else                    // default MIDI
                    nVelBitShift = 3;
			}
		} else {
			nArgIdxOfFilename = i;
            if (i >= argc) die("File name required.");
		}
	}
}

void ReadMML(FILE* pFile) {
    for (int iChannel=0; iChannel < CHMAX; iChannel++)
    {
        /// Error message buffer
        char errmsg[256] = {0};
        // ----- Each Channel Definition ----- //
        printf("const unsigned char %s_%d[] = {", szSongID, iChannel + 1);

        // ----- パラメータ初期値指定 ----- //
        int nOctave = 4,
            nOctaveAcci = 0,
            fLengthDot = 0;  // Set if 'l' has dot option, unset if not.

        // ----- Until channel terminal (,;) ----- //
        for (int nLetter = fgetc(pFile); nLetter != EOF; nLetter = fgetc(pFile))
        {
            //	Scans [a..g, n, r] with any Length, Dots, and Accidentals
            if (strchr("abcdefgrn", nLetter))
            {
                fpos_t *rescanPos = (fpos_t *) malloc(sizeof(fpos_t *));
                int nNoteNum = 0,
                    nNoteNumAcci = 0;

                // n と それ以外 で分岐
                if (nLetter == 'n') {
                    if( fscanf(pFile, "%d", &nNoteNum) <=0 )
                        die("Malformed `n` found!");
                } else {
                   if (nLetter == 'c') {
						// Ignore `ch`
						fgetpos(pFile, rescanPos);
						if (fgetc(pFile) == 'h')
							continue;
						fsetpos(pFile, rescanPos);
					}

                    //	Seeks accidentals (+, #, -)
					fgetpos(pFile, rescanPos);
					int nAcciChar = fgetc(pFile);
					if (strchr("+#", nAcciChar))
						nNoteNumAcci++;
					else if (nAcciChar == '-')
						nNoteNumAcci--;
					else
						fsetpos(pFile, rescanPos);

                    SeekLengthOption(pFile, TPDC); // Seeks [a-g] length option
                }

                SeekDots(pFile, DOTT);     // Seek dots

                // Convert [a..g] to Command/Note number
                if (nLetter == 'r') {
					PutCommand(REST);
                    continue;
                } else if (nLetter == 'n')
                    nNoteNum = nNoteNum + nPitchShift + 12 * (nOctaveShift);
                else
                {
                    int nNoteNumBase = 0;
                    switch (nLetter) {
                        case 'c':	nNoteNumBase = 0;	    break;
                        case 'd':	nNoteNumBase = 2;	    break;
                        case 'e':	nNoteNumBase = 4;	    break;
                        case 'f':	nNoteNumBase = 5;	    break;
                        case 'g':	nNoteNumBase = 7;	    break;
                        case 'a':	nNoteNumBase = 9;	    break;
                        case 'b':	nNoteNumBase = 11;      break;
                        default:    die("Unexpected Error!");
                    }
                    nNoteNum = nNoteNumBase + nNoteNumAcci + nPitchShift
                             + 12 * (nOctave + nOctaveAcci + 1 + nOctaveShift);
                }
                if (nNoteNum < 36)
                    die("Note number under 36!");
                else if (nNoteNum >= REST)
                    die("Note number over 127!");
                PutCommand(nNoteNum);
                nOctaveAcci=0;
            }
            else if (nLetter == 'N' && iChannel == CH4) {
                /// Noise parameter
                int clk_shr = fgetc(pFile); //~ 4bit
                int clk_div = fgetc(pFile); //~ 3bit
                if (strchr("0123456789ABCDEFabcdef", clk_shr) && strchr("0123456789ABCDEFabcdef", clk_div)) {
                    clk_shr -= 0x30;
                    clk_div -= 0x30;
                    if (clk_shr > 10) {
                        clk_shr = 9 + (clk_shr & 0x0F); /// 9 + lower 4bit --> [10 .. 15]
                    }
                    if (clk_div > 10) {
                        clk_div = 9 + (clk_div & 0x0F); /// 9 + lower 4bit --> [10 .. 15]
                    }
                    fpos_t *rescanPos = (fpos_t *) malloc(sizeof(fpos_t *));
                    fgetpos(pFile, rescanPos);
                    int outval = (clk_shr << 3) | (clk_div & 0x07);
                    int nAcciChar = fgetc(pFile);
                    if (strchr("+-", nAcciChar) || (clk_div & 0x08))
                        PutCommand(NS7S);
                    else
                        fsetpos(pFile, rescanPos);
                    SeekLengthOption(pFile, TPDC); // Seeks [a-g] length option
                    SeekDots(pFile, DOTT);     // Seek dots
                    PutCommand(outval);
                } else {
                    sprintf(errmsg, "'N' command has invalid parameter: S%c%c", clk_shr, clk_div);
                    die(errmsg);
                }
            }
            else if (nLetter == 'S' && iChannel == CH1) {
                /// Sweep parameter
                int swp_time = fgetc(pFile);
                int swp_shift = fgetc(pFile);
                if (isdigit(swp_time) && isdigit(swp_shift)) {
                    swp_time -= 0x30;   // Convert to actual numeric value
                    swp_shift -= 0x30;
                } else {
                    sprintf(errmsg, "'S' command has non-numeric parameter: S%c%c", swp_time, swp_shift);
                    die(errmsg);
                }
                if (!swp_time) {
                    // S0*: Sweep disable
                    PutCommandWithValue(SWPC, 0x08);
                } else if (swp_shift < 8 && swp_time < 8) {
                    byte val = swp_shift << 4 | swp_time;
                    fpos_t *rescanPos = (fpos_t *) malloc(sizeof(fpos_t *));
                    //	Seeks accidentals (+, -)
                    fgetpos(pFile, rescanPos);
                    int nAcciChar = fgetc(pFile);
                    if (nAcciChar == '-')
                        val |= 0x08;        //~ Sweep decliment
                    else if (nAcciChar == '+')
                        val &= ~0x08;
                    else
                        fsetpos(pFile, rescanPos);
                } else {
                    sprintf(errmsg, "'S' command parameters are out of range (0-7,0-7): S%d%d", swp_time, swp_shift);
                    die(errmsg);
                }
            }
            else if (nLetter == 'o') {
				nOctave = SeekOption(pFile, 0);
            }
            else if (nLetter == 'l') {
                SeekLengthOption(pFile, DRCG);
                if (SeekDots(pFile, DRDT)) // any dot found
                    fLengthDot = 1;
                else if (fLengthDot) {   // no dot found, but flag was set previously
                    fLengthDot = 0;
                    PutCommandWithValue(DRDT, 0);// disables DRDT enabled previously
                }
            }
			else if (nLetter == '_') nOctaveAcci = -1;
			else if (nLetter == '~') nOctaveAcci =  1;
			else if (nLetter == '<') nOctave--;
			else if (nLetter == '>') nOctave++;
			else if (nLetter == '[') PutCommand(RPHD);
            else if (nLetter == '/') PutCommand(IRED);
			else if (nLetter == ']') {
				int repeat_count_max = SeekOption(pFile, 1);
				PutCommandWithValue(RPTL, repeat_count_max);
            }
            else if (nLetter == 't') {
				int t = SeekOption(pFile, 0);
				if (!t) die("Tempo 0 found!");
				PutCommandWithValue(TMCG, t>>1);    //  tempo is saved as half a value.
			}
            else if (nLetter == 'v') {
                int v = SeekOption(pFile, 0);
                PutCommandWithValue(VECG, v>>nVelBitShift);
            }
            else if (nLetter == ';' || nLetter == ',')  break;
        }
		// ----- Definition of Each channel END ----- //
        PutCommand(TERM);
		printf("};\n");
    }
	fclose(pFile);

    // ----- Array for songs[][CHMAX] ----- //
	printf("{");
	for (int iChannel = 0; iChannel < CHMAX; iChannel++) {
        printf("%s_%d", szSongID, iChannel + 1);
        if (iChannel != CH4) printf(", ");
	}
	printf("},\n");
}
