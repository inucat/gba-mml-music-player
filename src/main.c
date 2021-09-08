#include "meta.h"

void irq_handler(void)
{
    reg16(IME) = IME_OFF;   // Stop IRQ to prevent multiple IRQ
    short flag = reg16(IF);	// Get IRQ flag
    if (flag & IF_TM2) {
        dmgstep();
    }
    reg16(IF) = flag;       // Acknowledge IRQ
    reg16(IME) = IME_ON;    // Re-enable IRQ
}

int main(void)
{
    reg32(IRQ_VECTOR) = (int) irq_handler;  // Register IRQ handler
    BgInit();
    BgMakeFrame();

    dmginit();
    dmgload(0);
    static short songid_max;
    for (songid_max=0; GetSongData(songid_max, 0); songid_max++);
    songid_max--;

    reg16(TM3COUNT)=0;
    reg16(TM3CTRL)= TM_PRESC1024 | TM_START;

    static unsigned short songid;    // Vertical or Horizontal Cursor position, and current songid
    while(1) {
        KeyStateUpdate();

        if (KeyTyped(KEY_SEL)) dmgstop();
        if (KeyTyped(KEY_STA)) {dmgload(songid); dmgplay();}
        if (KeyTyped(KEY_DL)) {songid = (songid ? songid - 1 : songid_max);}
        if (KeyTyped(KEY_DR)) {songid = (songid == songid_max ? 0 : songid + 1);}

        while(reg16(LCDSTAT) ^ LCDSTAT_VBMASK);
        /// Within VBLANK. Update screen
        PutStr(BGN_MENULIST, BGL_MENU_VALUE_TX-1, 4, (reg16(TM3COUNT) & 0x1000) ? "\xe0     \xe1" : "       ");
        PrintShort(BGN_MENULIST, BGL_MENU_VALUE_TX, 4, songid);
        while(reg16(LCDSTAT) & LCDSTAT_VBMASK);
    }

    return 0;
}
