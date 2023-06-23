#include "meta.h"

void irq_handler(void)
{
        reg16(IME) = IME_OFF; // Stop IRQ to prevent multiple IRQ
        short flag = reg16(IF); // Get IRQ flag
        if (flag & IF_TM2) {
                dmgstep();
        }
        reg16(IF) = flag; // Acknowledge IRQ
        reg16(IME) = IME_ON; // Re-enable IRQ
}

int main(void)
{
        reg32(IRQ_VECTOR) = (int)irq_handler; // Register IRQ handler

        // BG & music
        BgInit();
        BgMakeFrame();
        dmginit();
        dmgload(0);

        static unsigned short songid; // Current songid
        // Determines max. song id
        static short max_songid;
        for (max_songid = 0; GetSongData(max_songid, 0); max_songid++)
                ;
        max_songid--;

        // Timer for blink cursor
        reg16(TM3COUNT) = 0;
        reg16(TM3CTRL) = TM_PRESC1024 | TM_START;

        while (1) {
                KeyStateUpdate();

                if (KeyTyped(KEY_SEL)) {
                        dmgstop();
                }
                if (KeyTyped(KEY_STA)) {
                        dmgload(songid);
                        dmgplay();
                }
                if (KeyTyped(KEY_DL)) {
                        songid = (songid > 0 ? songid - 1 : max_songid);
                }
                if (KeyTyped(KEY_DR)) {
                        songid = (songid == max_songid ? 0 : songid + 1);
                }

                // Update screen within VBLANK.
                while (reg16(LCDSTAT) ^ LCDSTAT_VBMASK)
                        ;
                PutStr(BGN_MENULIST, BGL_MENU_VALUE_TX - 1, 4,
                       (reg16(TM3COUNT) & 0x1000) ? "\xe0     \xe1" :
                                                    "       ");
                PrintShort(BGN_MENULIST, BGL_MENU_VALUE_TX, 4, songid);
                while (reg16(LCDSTAT) & LCDSTAT_VBMASK)
                        ;
        }

        return 0;
}
