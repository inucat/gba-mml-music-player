/**
 * @file dmgsound.h
 * @brief GBA Sound register map & bit assignments
 * @author inucat
 * @note DMG refers to GameBoy whose codename is Dot Matrix Game.
 */
#ifndef _DMGSOUND_H_
#define _DMGSOUND_H_

#define IOREG   0x4000000


/// Master Switch & 1~4 Status (SOUNDCNT_X)
#define DMGMSW  (IOREG + 0x84)
#define DMGSTAT (IOREG + 0x84)
#define DMGMSW_ON   0x80
#define DMGSTAT_1   1
#define DMGSTAT_2   2
#define DMGSTAT_3   4
#define DMGSTAT_4   8


/// Ch. Switch & L/R Volume (SOUNDCNT_L)
#define DMGCNT (IOREG + 0x80)
#define DMGCNT_1L_ON    0x0100
#define DMGCNT_2L_ON    0x0200
#define DMGCNT_3L_ON    0x0400
#define DMGCNT_4L_ON    0x0800
#define DMGCNT_1R_ON    0x1000
#define DMGCNT_2R_ON    0x2000
#define DMGCNT_3R_ON    0x4000
#define DMGCNT_4R_ON    0x8000
#define DMGCNT_ALLON    0xFF00
#define DMGCNT_Lvol(v)  ((v) & 7)           // v = 0 ~ 7
#define DMGCNT_Rvol(v)  (((v) & 7) << 4)    // v = 0 ~ 7


/// DSound Ctrl & DMG Amp Ratio (SOUNDCNT_H)
#define DMGMVL  (IOREG + 0x82)
#define DMGMVL_25     0
#define DMGMVL_50     1
#define DMGMVL_100    2


/// S1 Sweep Ctrl (SOUND1CNT_L)
#define DMG1SWEEP (IOREG + 0x60)
#define SWEEP_OFF 0x0008
#define SWEEP_INC 0x00
#define SWEEP_DEC 0x08
#define SWEEP_shift(s)  ((s) & 7)           // 0 <= s <= 7
#define SWEEP_time(d)   (((d) & 7) << 4)    // 0 <= d <= 7


/// S3 Bank Ctrl (SOUND3CNT_L)
#define DMG3BANKSEL (IOREG + 0x70)
#define BANKSEL_2x32    0x00
#define BANKSEL_1x64    0x20
#define BANKSEL_PLAY0   0x00
#define BANKSEL_PLAY1   0x40
#define BANKSEL_ON      0x80


/// S3 Len & Volume Ctrl (SOUND3CNT_H)
#define DMG3VL (IOREG + 0x72)
#define DMG3VL_VOL100   0x2000
#define DMG3VL_VOL50    0x4000
#define DMG3VL_VOL25    0x6000
#define DMG3VL_VOL75    0x8000
#define DMG3VL_VOL0     0x0000
#define DMG3VL_len(l)   ((l) & 0xFF)    // 0 <= len <= 255

/// S3 Wave RAM
#define DMG3WRAM(offset) (IOREG + 0x90 + (offset))
#define MAKEINTWRAM(wave) \
    ( ((wave)<<24) | ((wave)>>24) | (((wave) & 0xFF00)<<8) | (((wave)>>8) & 0xFF00) )


/* Len, Duty, Env. Ctrl */

#define DMG1EDL (IOREG + 0x62)  //  SOUND1CNT_H
#define DMG2EDL (IOREG + 0x68)  //  SOUND2CNT_L
#define DMG4EDL (IOREG + 0x78)  //  SOUND4CNT_L

#define EDL_DUTY125 0x00
#define EDL_DUTY25  0x40
#define EDL_DUTY50  0x80
#define EDL_DUTY75  0xC0
#define EDL_ENVINC  0x0800
#define EDL_ENVDEC  0x0000
#define EDL_LENr(r) ((r) & 0x3F)    // 0 <= Length Param <= 63
#define EDL_ENVstep(e)  (((e) & 0x07) << 8) // 0 <= Step <= 7
#define EDL_ENVvol(v)   (((v) & 0x0F) <<12) // 0 <= Init Volume <= 15


/* Reset, Loop, Freq. or Noise Param. */

#define DMG1RLF (IOREG + 0x64)  //  SOUND1CNT_X
#define DMG2RLF (IOREG + 0x6C)  //  SOUND2CNT_H
#define DMG3RLF (IOREG + 0x74)  //  SOUND3CNT_X
#define DMG4RLN (IOREG + 0x7C)  //  SOUND4CNT_H

#define RLFN_TIMED 0x4000
#define RLFN_RESET 0x8000
#define RLN_7STAGE 0x0008
#define RLF_freqG(g) ((g) & 0x07FF)     // 0 <= Freq G <= 2047
#define RLF_freq(f)  ((0x800 - (0x20000 / (f))) & 0x07FF )   // 64 <= Freq F <= 131072
#define RLN_div(d)   ((d) & 7)          // 0 <= Divisor <= 7
#define RLN_pstep(p) (((p) & 15) << 4)  // 0 <= Prestep <= 15

#endif
