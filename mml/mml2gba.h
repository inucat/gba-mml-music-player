#pragma once

#define MAIN_N_FNAMEMAXLEN 256

#define MAIN_STR_USAGE \
"Usage: mml2gba filename [options]" "\n" \
"Available options: " "\n" \
"\t-oN" "\n" \
"\t-pN" "\n" \
"\t-vN" "\n" \
"\t   --verbose" "\n" \

#define FALSE 0
#define TRUE 1

enum _ERROR_CODE {
    ERR_SUCCESS,
    ERR_UNEXPECTED,
    ERR_WRONG_USAGE
};
