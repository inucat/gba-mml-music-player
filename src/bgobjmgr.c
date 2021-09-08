#include "meta.h"

hword* const bgMap[] = {
    (hword*) VRAM_MAP(16),
    (hword*) VRAM_MAP(20),
    (hword*) VRAM_MAP(24),
    (hword*) VRAM_MAP(28)};

void BgMakeFrame(void)
{
    for (short t=0; t<1024; t++)
        bgMap[3][t] = 0x20;
    for (short tx=0; tx<LCD_VTWIDT; tx++) {
        PutTile(BGN_WINDOWFRAME, tx, 0, 0x80);
        PutTile(BGN_WINDOWFRAME, tx, 2, 0x80);
        PutTile(BGN_WINDOWFRAME, tx,17, 0x80);
        PutTile(BGN_WINDOWFRAME, tx,19, 0x80);
    }
    PutStr(BGN_WINDOWFRAME, 2, 1, STAB_MUSICPLAYER);
    PutStr(BGN_MENULIST, BGL_MENU_KEY_TX, 4, STAB_SONGINDEXIS);
    PutStr(BGN_WINDOWFRAME, 3, 18, STAB_CONTROLHINT);
}

void PutTile(short bgnum, short tx, short ty, short tileId) {
    bgMap[bgnum][ tx +  ty * LCD_VTWIDT ] = tileId;
}

void BgInit(void)
{
    reg16(LCDCNT) = LCDCNT_BGMODE(0) |\
        LCDCNT_BG0ON | LCDCNT_BG1ON | LCDCNT_BG2ON | LCDCNT_BG3ON;
    reg16(BG0CNT) = BGCNT_MAP3232 | BGCNT_COL256 | BGCNT_TILENo(0) | BGCNT_MAPNo(16); // 32x32 Tiles VMap with 256 colors
    reg16(BG1CNT) = BGCNT_MAP3232 | BGCNT_COL256 | BGCNT_TILENo(0) | BGCNT_MAPNo(20);
    reg16(BG2CNT) = BGCNT_MAP3232 | BGCNT_COL256 | BGCNT_TILENo(0) | BGCNT_MAPNo(24);
    reg16(BG3CNT) = BGCNT_MAP3232 | BGCNT_COL256 | BGCNT_TILENo(0) | BGCNT_MAPNo(28);
    hword *vramTile = (hword *) VRAM_TILE(0);
    hword *palBg = (hword *) PALETTE_BG;
    for (short i=0; i<(PALETTE_LEN>>1); i++)
        palBg[i] = misaki_gothic_2nd_negaPal[i];

    for (short i=0; i < (misaki_gothic_2nd_negaTilesLen>>1); i++)
        vramTile[i] = misaki_gothic_2nd_negaTiles[i<<1] | (misaki_gothic_2nd_negaTiles[(i<<1)+1] << 8);
    vramTile = (hword *) VRAM_TILE(1);  /// Light Theme
    for (short i=0; i < (misaki_gothic_2ndTilesLen>>1); i++)
        vramTile[i] = misaki_gothic_2ndTiles[i<<1] | (misaki_gothic_2ndTiles[(i<<1)+1] << 8);
}

void PutStr(short bgnum, short tx, short ty, const char *str)
{
    for (short i=0; str[i]; i++)
        bgMap[bgnum][tx + i + ty * LCD_VTWIDT] = str[i];
}

void PrintShort(short bgnum, short tx, short ty, short num)
{
    char szDigits[] = "     ";  // 3,2,7,6,7,\0
    for (short i=0; i<=4; i++) {
        szDigits[4-i] = num % 10 + 0x30;
        num /= 10;
        if (!num) break;
    }
    PutStr(bgnum, tx, ty, szDigits);
}
