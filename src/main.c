#include "meta.h"

#define SONGINDEX_TO_PLAY 0   // <--- Change here for other songs

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
    reg32(IRQ_VECTOR) = (int) irq_handler;  // Register IR handler
    BgInit();
    PutStr(0, 3, 1, STAB_MUSICPLAYER);
    // PutStr(0, 2, 9-3, "\xfd \xba\xda\xca \xb9\xde\xb0\xd1\xce\xde\xb0\xb2\xb1\xc4\xde\xca\xde\xdd\xbd");
    // PutStr(0, 2, 10-3, "  \xc6\xce\xdd\xba\xde\xc9\xbb\xdd\xcc\xdf\xd9\xc3\xde\xbd");
    // PutStr(0, 2, 12, "\1\2 Game Boy Advance \1\2");
    // PutStr(0, 2, 13, "\1\2 \xfa\xfb\x17\3\4\5\6\7 \1\2");
    // PutStr(0, 2, 17, "@num_kadoma\xbb\xdd\x20\xc9");
    // PutStr(0, 2, 18, "\xa2\xd0\xbb\xb7\xcc\xab\xdd\xc4\xa3\xc3\xde\xbd!");

    dmginit();
    dmgload(SONGINDEX_TO_PLAY);
    dmgplay();
    while(1) {
        while(reg16(LCDSTAT) ^ LCDSTAT_VBMASK);
        /// Update screen
        while(reg16(LCDSTAT) & LCDSTAT_VBMASK);
    }

    return 0;
}
