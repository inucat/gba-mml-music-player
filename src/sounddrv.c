#include "meta.h"

static hword freq_param[128] = {0};

static const unsigned char *Song[MAX_CHAN];

/// Internal step counter
static word tick = 0;

/// Playback states
static hword tempo_bpm = 120;                       // tempo (BPM)
static word next_tick[MAX_CHAN]    = {0};           // tick time when next note will be handled
static hword cmd_index[MAX_CHAN]   = {0};           // offset of each channel's byte array
static hword note_len[MAX_CHAN]    = {4,4,4,4};     // current note length
static hword loop_head[MAX_CHAN]    = {0};          // offset of start of repeat section
static hword loop_count[MAX_CHAN]   = {0};          // playback count for repeat section
static hword note_dot[MAX_CHAN]     = {0};          // indicates the note has dot(s)

struct playback_status_struct {
    hword next_tick;    // tick time when next note will be handled
    hword cmd_index;    // offset of each channel's byte array
    hword note_len;     // current note length
    hword loop_head;    // offset of start of repeat section
    hword loop_count;   // playback count for repeat section
    hword note_dot;     // indicates the note has dot(s)
} pb_status[MAX_CHAN];

void dmgload(int songid) {
    dmgstop();
    tick = 0;
    tempo_bpm = 120;
    for (int i=CH1; i<MAX_CHAN; i++) {
        Song[i] = get_song_track(songid, i);
        // next_tick[i] = cmd_index[i] = note_dot[i] = loop_head[i] = loop_count[i] = 0;
        pb_status[i].cmd_index   = 0;
        pb_status[i].next_tick   = 0;
        pb_status[i].note_dot    = 0;
        pb_status[i].loop_count  = 0;
        pb_status[i].loop_head   = 0;
        pb_status[i].note_len    = 4;
        // note_len[i] = 4;
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
    // Frequencies of MIDI note numbers -> Frequency Parameters (see RLF_freq macro)
    int freqbase[] = { 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544, 6645<<1, 7040<<1, 7459<<1, 7902<<1 };
    for (int i=127; i>=36; i--) {
        int f = freqbase[i%12] >> (10 - i/12);
        freq_param[i] = RLF_freq(f);
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

__inline short _dmg_handle_ctrl_cmd() {
    hword len_stack = 0, dot_tmp = 0, ch4_7stage = 0;
    // Ignore channel if TERM reached
    if (Song[ch][pb_status[ch].cmd_index] == TERM) {
        reg16(DMGCNT) &= ~((DMGCNT_1L_ON|DMGCNT_1R_ON) << ch);
        pb_status[ch].next_tick = 0x7FFFFFFF;
        continue;
    }

    //  Control commands are processed first
    while (Song[ch][pb_status[ch].cmd_index] > REST) {
        switch (Song[ch][pb_status[ch].cmd_index]) {
        case DRCG:
            pb_status[ch].note_len = Song[ch][++(pb_status[ch].cmd_index)];
            break;

        case TPDC:
            len_stack = pb_status[ch].note_len;
            pb_status[ch].note_len = Song[ch][++(pb_status[ch].cmd_index)];
            break;

        case DOTT:
            dot_tmp = Song[ch][++(pb_status[ch].cmd_index)];
            break;

        case RPHD:
            pb_status[ch].loop_head = pb_status[ch].cmd_index;
            pb_status[ch].loop_count = 0;
            break;

        case RPTL:
            pb_status[ch].loop_count++;                                //  Section played once
            hword repLim = Song[ch][++(pb_status[ch].cmd_index)];   //  Get repeat limit (0 means infty)
            if (repLim == 0 || pb_status[ch].loop_count < repLim)
                pb_status[ch].cmd_index = pb_status[ch].loop_head;
            break;

        case DRDT:
            pb_status[ch].note_dot = Song[ch][++(pb_status[ch].cmd_index)];
            break;

        case TMCG:
            tempo_bpm = Song[ch][++(pb_status[ch].cmd_index)] << 1;
            reg16(TM2COUNT) = (65536 - (16777216 * 120/ (INT_FREQ * tempo_bpm)));
            break;

        case SWPC:
            reg16(DMG1SWEEP)= Song[ch][++(pb_status[ch].cmd_index)];
            break;

        case NS7S:
            ch4_7stage = RLN_7STAGE;
            break;

        case VECG:
            switch (ch) {
            case CH1:
                reg16(DMG1EDL)&= ~EDL_ENVvol(15);
                reg16(DMG1EDL)|= EDL_ENVvol(Song[ch][++(pb_status[ch].cmd_index)]);
                break;
            case CH2:
                reg16(DMG2EDL)&= ~EDL_ENVvol(15);
                reg16(DMG2EDL)|= EDL_ENVvol(Song[ch][++(pb_status[ch].cmd_index)]);
                break;
            case CH4:
                reg16(DMG4EDL)&= ~EDL_ENVvol(15);
                reg16(DMG4EDL)|= EDL_ENVvol(Song[ch][++(pb_status[ch].cmd_index)]);
                break;
            }
            break;
        }
        pb_status[ch].cmd_index++;
    }
}

void dmgstep(void) {
    for (hword ch = 0; ch < MAX_CHAN; ch++) {
        //  When tick reaches the next note tick time
        if(tick >= pb_status[ch].next_tick) {
            if (Song[ch][pb_status[ch].cmd_index] == REST) {
                reg16(DMGCNT) &= ~((DMGCNT_1L_ON|DMGCNT_1R_ON) << ch);
            }
            else if (ch == CH4) {
                reg16(DMGCNT) |= (DMGCNT_4L_ON|DMGCNT_4R_ON);
                reg16(DMG4RLN) =
                    RLFN_RESET | ch4_7stage |
                    ((Song[ch][pb_status[ch].cmd_index] << 1) & 0xF0) |
                    (Song[ch][pb_status[ch].cmd_index] & 0b0111);
            }
            else {
                reg16(DMGCNT) |= (DMGCNT_1L_ON|DMGCNT_1R_ON) << ch;
                reg16(DMG1RLF + (ch<<3)) = RLFN_RESET | freq_param[ Song[ch][pb_status[ch].cmd_index] ];
            }

            hword gatetime_tick = FOURBEAT / pb_status[ch].note_len;

            if (dot_tmp == 1 || (len_stack == 0 && pb_status[ch].note_dot == 1))
                gatetime_tick += (gatetime_tick >> 1);
            else if (dot_tmp == 3 || (len_stack == 0 && pb_status[ch].note_dot == 3))
                gatetime_tick += (gatetime_tick >> 1) + (gatetime_tick >> 2);

            pb_status[ch].next_tick += gatetime_tick;

            if (len_stack)
                pb_status[ch].note_len = len_stack;

            pb_status[ch].cmd_index++;
        }   // END IF for handled note
    }   // END LOOP for each channel
    (tick)++;
}
