#include "sounddrv.h"
#include "dmgsound.h"
#include "songdata.h"
#include "main.h"

// #define CHMAX 4

/// Note Num. --> Freq_G value
static hword FreqGs[128] = {0};

/// Pointer to CHANNELS of A SONG.
static const unsigned char* SongData[CHMAX];

/// Internal step counter
static word wTick = 0;

/// Playback states
static hword hTempo = 120;
static word wNextTick[CHMAX]    = {0};
static hword hCmdIndex[CHMAX]   = {0};
static hword hNoteLen[CHMAX]    = {4,4,4,4};
static hword hRepHead[CHMAX]    = {0};
static hword hRepCount[CHMAX]   = {0};
static hword fLenDot[CHMAX]     = {0};

/// Load song data & Initialize playback states
/// @param songidx Song index
void dmgload(int songidx) {
    dmgstop();
    wTick = 0;
    hTempo = 120;
    for (int i=CH1; i<CHMAX; i++) {
        SongData[i] = GetSongData(songidx, i);
        wNextTick[i] = hCmdIndex[i] = fLenDot[i] = hRepHead[i] = hRepCount[i] = 0;
        hNoteLen[i] = 4;
    }
    reg16(TM2CNT_L) = 65536 - 16777216 / INT_FREQ;
}

void dmgplay(void) {
    reg16(DMGMSW) = DMGMSW_ON;
    reg16(DMGCNT) |= DMGCNT_ALLON;
    reg16(TM2CNT_H) = TM_START | TM_IRQEN;
}

void dmgstop(void) {
    reg16(TM2CNT_H) &= ~TM_START;
    reg16(DMGCNT) &= ~DMGCNT_ALLON;
}

void dmginit(void) {
    int freqbase[] = { 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544, 6645<<1, 7040<<1, 7459<<1, 7902<<1 };
    for (int i=127, f=0; i>=36; i--) {
        f = freqbase[i%12] >> (10 - i/12);
        FreqGs[i] = RLF_freq(f);
    }
    reg16(DMGMSW)= DMGMSW_ON;
    reg16(DMGMVL)= DMGMVL_100;
    reg16(DMGCNT)= DMGCNT_ALLON | DMGCNT_Lvol(7) | DMGCNT_Rvol(7);
    reg16(DMG1SWEEP)= SWEEP_OFF;
    reg16(DMG1EDL)= EDL_DUTY50 | EDL_ENVDEC | EDL_ENVstep(7) | EDL_ENVvol(15);
    reg16(DMG2EDL)= EDL_DUTY50 | EDL_ENVDEC | EDL_ENVstep(7) | EDL_ENVvol(15);
    reg16(DMG4EDL)= EDL_ENVDEC | EDL_ENVstep(7) | EDL_ENVvol(15);
    reg16(DMG3VL)= DMG3VL_VOL75;
    reg16(DMG3BANKSEL)= BANKSEL_2x32 | BANKSEL_PLAY1;
    reg32(DMG3WRAM(0))= MAKEINTWRAM(0x23456789);
    reg32(DMG3WRAM(4))= MAKEINTWRAM(0xABCDDCBA);
    reg32(DMG3WRAM(8))= MAKEINTWRAM(0x23456789);
    reg32(DMG3WRAM(12))= MAKEINTWRAM(0xABCDDCBA);
    reg16(DMG3BANKSEL)= BANKSEL_ON | BANKSEL_2x32 | BANKSEL_PLAY0;
    reg16(IE)= IF_TM2;
    reg16(IME)= IME_ON;
}

void dmgstep(void)
{
    for (hword ch = 0; ch < CHMAX; ch++)
    {
        //  When tick reaches the next note timing
        if(wTick >= wNextTick[ch])
        {
            hword lenBackup = 0, tempDot = 0;
            if (SongData[ch][hCmdIndex[ch]] == TERM) {
                reg16(DMGCNT) &= ~(0x1100 << ch);
                wNextTick[ch] = 0x7FFFFFFF;
                continue;
            }
            //  Commands are processed first
            while (SongData[ch][hCmdIndex[ch]] > REST)
            {
                switch (SongData[ch][hCmdIndex[ch]])
                {
                case DRCG: hNoteLen[ch] = SongData[ch][++hCmdIndex[ch]];
                    break;

                case TPDC:
                    lenBackup = hNoteLen[ch];
                    hNoteLen[ch] = SongData[ch][++hCmdIndex[ch]];
                    break;

                case DOTT: tempDot = SongData[ch][++hCmdIndex[ch]];
                    break;

                case RPHD:
                    hRepHead[ch] = hCmdIndex[ch];
                    hRepCount[ch] = 0;
                    break;

                case RPTL:
                    hRepCount[ch]++;                                //  Section played once
                    hword repLim = SongData[ch][++hCmdIndex[ch]];   //  Get repeat limit (0 means infty)
                    if (repLim == 0 || hRepCount[ch] < repLim)
                        hCmdIndex[ch] = hRepHead[ch];
                    break;

                case DRDT: fLenDot[ch] = SongData[ch][++hCmdIndex[ch]];
                    break;

                case TMCG:
                    hTempo = SongData[ch][++hCmdIndex[ch]] << 1;
                    reg16(TM2CNT_L) = (65536 - (16777216 * 120/ (INT_FREQ * hTempo)));
                    break;
                }
                hCmdIndex[ch]++;
            }
            if (SongData[ch][hCmdIndex[ch]] == REST) {
                reg16(DMGCNT) &= ~(0x1100 << ch);
            }
            else {
                reg16(DMGCNT) |= 0x1100 << ch;
                reg16(DMG1RLF + (ch<<3)) = RLFN_RESET | FreqGs[ SongData[ch][hCmdIndex[ch]] ];
            }

            hword steptime = FOURBEAT / hNoteLen[ch];

            if (tempDot == 1 || (lenBackup == 0 && fLenDot[ch] == 1))
                steptime += (steptime >> 1);
            else if (tempDot == 3 || (lenBackup == 0 && fLenDot[ch] == 3))
                steptime += (steptime >> 1) + (steptime >> 2);

            wNextTick[ch] += steptime;

            if (lenBackup)
                hNoteLen[ch] = lenBackup;

            hCmdIndex[ch]++;
        }
    }
    (wTick)++;
}
