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
    reg16(LCDCNT)= LCDCNT_BG0ON | LCDCNT_BGMODE(0);
    reg16(BG0CNT)= BGCNT_COL256 | BGCNT_MAP3232 | BGCNT_TILENo(8) | BGCNT_MAPNo(16);
    hword *palBg = (hword *) PALETTE_BG;
    hword *vramMap = (hword *) VRAM_MAP(16);
    hword *vramTile = (hword *) VRAM_TILE(0);
    for (short i=0; i<PALETTE_LEN; i++)
        palBg[i] = meguri_gothic_2nd_negaPal[i];

    for (short i=0; i < (meguri_gothic_2nd_negaTilesLen>>1); i++)
        vramTile[i] = meguri_gothic_2nd_negaTiles[i<<1] | (meguri_gothic_2nd_negaTiles[(i<<1)+1] << 8);
    vramTile = (hword *) VRAM_TILE(8);
    for (short i=0; i < (meguri_gothic_2ndTilesLen>>1); i++)
        vramTile[i] = meguri_gothic_2ndTiles[i<<1] | (meguri_gothic_2ndTiles[(i<<1)+1] << 8);

    for (short i=0; i < (meguri_gothic_2ndTilesLen>>6); i++)
    {
        vramMap[7 + (i&15) + ((i>>4)<<5) ] = 0xBB;
        "\xa1";
    }

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
