#include "meta.h"
#include "song_ttls.h"
#include "song_gymno1.h"

const uint8 **songs[] = {
    tracks_gymno1, tracks_ttls,
    0 // sentinel
};

const unsigned char *GetSongData(int index, int ch) { return songs[index][ch]; }
