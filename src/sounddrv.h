#ifndef _SOUNDDRV_H_
#define _SOUNDDRV_H_

/// Sound driver frequency == Resolution of a half note.
/// This must be at least 256 because the driver timer's reload count
/// is set by the formula: 2^16 - (2^24 / FREQ).
/// The slower the tempo gets, the smaller the reload count gets, and
/// it can underflows if this constant is too small.
#define INT_FREQ    576

#define FOURBEAT    (INT_FREQ << 1)

/// Load song data & Initialize playback states
/// @param songidx Song index
void dmgload(int);

void dmgplay(void);
void dmgstop(void);
void dmginit(void);
void dmgstep(void);

#endif
