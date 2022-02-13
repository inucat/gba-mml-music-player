#include "meta.h"

/// MIDI note num. --> Freq_G value (see RLF_freq Macro)
static hword FreqGs[128] = {0};

static const unsigned char *SongData[MAX_CHAN];

/// Internal step counter
static word wTick = 0;

/// Playback states
static hword hTempo = 120;                          // tempo (BPM)
static word wNextTick[MAX_CHAN]    = {0};           // tick time when next note will be handled
static hword hCmdIndex[MAX_CHAN]   = {0};           // offset of each channel's byte array
static hword hNoteLen[MAX_CHAN]    = {4,4,4,4};     // current note length
static hword hRepHead[MAX_CHAN]    = {0};           // offset of start of repeat section
static hword hRepCount[MAX_CHAN]   = {0};           // playback count for repeat section
static hword fLenDot[MAX_CHAN]     = {0};           // indicates the note has dot(s)

void dmgload(int songidx) {
    dmgstop();
    wTick = 0;
    hTempo = 120;
    for (int i=CH1; i<MAX_CHAN; i++) {
        SongData[i] = GetSongData(songidx, i);
        wNextTick[i] = hCmdIndex[i] = fLenDot[i] = hRepHead[i] = hRepCount[i] = 0;
        hNoteLen[i] = 4;
    }

    // Reset timer reload count which may be altered by tempo change
    reg16(TM2COUNT) = 65536 - 16777216 / INT_FREQ;
}

void dmgplay(void) {
    reg16(DMGMSW) = DMGMSW_ON;
    reg16(DMGCNT) |= DMGCNT_ALLON;
    reg16(TM2CTRL) = TM_START | TM_IRQEN;
}

void dmgstop(void) {
    reg16(TM2CTRL) &= ~TM_START;
    reg16(DMGCNT)  &= ~DMGCNT_ALLON;
}

void dmginit(void) {
    // C, C#, D, ..., B -> FREQ_G value (see RLF_freq macro)
    int freqbase[] = { 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544, 6645<<1, 7040<<1, 7459<<1, 7902<<1 };
    for (int i=127, f=0; i>=36; i--) {
        f = freqbase[i%12] >> (10 - i/12);
        FreqGs[i] = RLF_freq(f);
    }

    // master
    reg16(DMGMSW)= DMGMSW_ON;
    reg16(DMGMVL)= DMGMVL_100;
    reg16(DMGCNT)= DMGCNT_ALLON | DMGCNT_Lvol(7) | DMGCNT_Rvol(7);

    // channel 1
    reg16(DMG1SWEEP)= SWEEP_OFF;
    reg16(DMG1EDL)= EDL_DUTY50 | EDL_ENVDEC | EDL_ENVstep(7) | EDL_ENVvol(15);

    // channel 2
    reg16(DMG2EDL)= EDL_DUTY50 | EDL_ENVDEC | EDL_ENVstep(7) | EDL_ENVvol(15);

    // channel 4
    reg16(DMG4EDL)= EDL_ENVDEC | EDL_ENVstep(7) | EDL_ENVvol(15);

    // channel 3
    reg16(DMG3VL)= DMG3VL_VOL75;
    reg16(DMG3BANKSEL)= BANKSEL_2x32 | BANKSEL_PLAY1;
    reg32(DMG3WRAM(0))= MAKEINTWRAM(0x23456789);
    reg32(DMG3WRAM(4))= MAKEINTWRAM(0xABCDDCBA);
    reg32(DMG3WRAM(8))= MAKEINTWRAM(0x23456789);
    reg32(DMG3WRAM(12))= MAKEINTWRAM(0xABCDDCBA);
    reg16(DMG3BANKSEL)= BANKSEL_ON | BANKSEL_2x32 | BANKSEL_PLAY0;

    // timer interrupt
    reg16(IE)= IF_TM2;
    reg16(IME)= IME_ON;
}

void dmgstep(void)
{
    for (hword ch = 0; ch < MAX_CHAN; ch++)
    {
        //  When tick reaches the next note tick time
        if(wTick >= wNextTick[ch])
        {
            hword lenBackup = 0, tempDot = 0, ch4_7stage = 0;

            // Ignore channel if TERM reached
            if (SongData[ch][hCmdIndex[ch]] == TERM) {
                reg16(DMGCNT) &= ~((DMGCNT_1L_ON|DMGCNT_1R_ON) << ch);
                wNextTick[ch] = 0x7FFFFFFF;
                continue;
            }

            //  Control commands are processed first
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
                    reg16(TM2COUNT) = (65536 - (16777216 * 120/ (INT_FREQ * hTempo)));
                    break;

                case SWPC:
                    reg16(DMG1SWEEP)= SongData[ch][++hCmdIndex[ch]];
                    break;

                case NS7S:
                    ch4_7stage = RLN_7STAGE;
                    break;

                case VECG:
                    switch (ch)
                    {
                    case CH1:
                        reg16(DMG1EDL)&= ~EDL_ENVvol(15);
                        reg16(DMG1EDL)|= EDL_ENVvol(SongData[ch][++hCmdIndex[ch]]);
                        break;
                    case CH2:
                        reg16(DMG2EDL)&= ~EDL_ENVvol(15);
                        reg16(DMG2EDL)|= EDL_ENVvol(SongData[ch][++hCmdIndex[ch]]);
                        break;
                    case CH4:
                        reg16(DMG4EDL)&= ~EDL_ENVvol(15);
                        reg16(DMG4EDL)|= EDL_ENVvol(SongData[ch][++hCmdIndex[ch]]);
                        break;
                    }
                    break;
                }
                hCmdIndex[ch]++;
            }
            if (SongData[ch][hCmdIndex[ch]] == REST) {
                reg16(DMGCNT) &= ~((DMGCNT_1L_ON|DMGCNT_1R_ON) << ch);
            }
            else if (ch == CH4) {
                reg16(DMGCNT) |= (DMGCNT_4L_ON|DMGCNT_4R_ON);
                reg16(DMG4RLN) =
                    RLFN_RESET | ch4_7stage |
                    ((SongData[ch][hCmdIndex[ch]] << 1) & 0xF0) |
                    (SongData[ch][hCmdIndex[ch]] & 0b0111);
            }
            else {
                reg16(DMGCNT) |= (DMGCNT_1L_ON|DMGCNT_1R_ON) << ch;
                reg16(DMG1RLF + (ch<<3)) = RLFN_RESET | FreqGs[ SongData[ch][hCmdIndex[ch]] ];
            }

            hword gatetime = FOURBEAT / hNoteLen[ch];

            if (tempDot == 1 || (lenBackup == 0 && fLenDot[ch] == 1))
                gatetime += (gatetime >> 1);
            else if (tempDot == 3 || (lenBackup == 0 && fLenDot[ch] == 3))
                gatetime += (gatetime >> 1) + (gatetime >> 2);

            wNextTick[ch] += gatetime;

            if (lenBackup)
                hNoteLen[ch] = lenBackup;

            hCmdIndex[ch]++;
        }   // END IF for handled note
    }   // END LOOP for each channel
    (wTick)++;
}
