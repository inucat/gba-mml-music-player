#ifndef _SOUNDDRV_H_
#define _SOUNDDRV_H_

// Sound driver frequency == Resolution of a half note.
// This must be >=256 because the driver timer's reload count
// is determinted by the formula: 2^16 - (2^24 / FREQ).
// This value should not be too big or too small.
#define INT_FREQ    576

#define FOURBEAT    (INT_FREQ << 1)

// Load song data & reset playback states
// @param songidx Song index
void dmgload(int);

// Starts music
void dmgplay(void);

// Stop & rewind music
void dmgstop(void);

// Init
void dmginit(void);

// Updates sound driver
void dmgstep(void);

#endif
