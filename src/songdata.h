#ifndef _MUSIC_H_
#define _MUSIC_H_
#include "main.h"

typedef enum tagMusicCommands {
    REST = 128,//-128

    DRCG,// Duration Change
    TPDC,// Temporary Duration Change
    DOTT,// Dotted note
    RPHD,// Repeat Section Head
    RPTL,// Repeat Section Tail
    TMCG,// Tempo Change
    INIT,// Initialization Section Signal
    INED,// Init. Section End
    PCSF,// Pitch Shift
    DRDT,// Duration Dotted (ex. "l4.")
    VECG,// Velocity Change
    IRED,// Intro End

    TERM = 255// Terminate Signal
} MUSICCOMMAND;

enum sound_channels {
    CH1,
    CH2,
    CH3,
    CH4,
    CHMAX
};

const unsigned char *GetSongData(int index, int ch);

#endif
