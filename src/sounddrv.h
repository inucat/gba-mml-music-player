#ifndef _SOUNDDRV_H_
#define _SOUNDDRV_H_

// 2分音符の分解能(?).
// タイマー初期値を 2^16 - (2^24 / FREQ) で求める都合上, FREQ >= 256 が必要.
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
