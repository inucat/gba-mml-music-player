#include "meta.h"

static hword *vramMap = (hword *) VRAM_MAP(16);

void BgInit(void)
{
    hword *vramTile = (hword *) VRAM_TILE(0);
    hword *palBg = (hword *) PALETTE_BG;
    reg16(LCDCNT)= LCDCNT_BG0ON | LCDCNT_BGMODE(0);
    reg16(BG0CNT)= BGCNT_COL256 | BGCNT_MAP3232 | BGCNT_TILENo(0) | BGCNT_MAPNo(16);
    for (short i=0; i<PALETTE_LEN; i++)
        palBg[i] = meguri_gothic_2nd_negaPal[i];

    for (short i=0; i < (meguri_gothic_2nd_negaTilesLen>>1); i++)
        vramTile[i] = meguri_gothic_2nd_negaTiles[i<<1] | (meguri_gothic_2nd_negaTiles[(i<<1)+1] << 8);
    vramTile = (hword *) VRAM_TILE(1);
    for (short i=0; i < (meguri_gothic_2ndTilesLen>>1); i++)
        vramTile[i] = meguri_gothic_2ndTiles[i<<1] | (meguri_gothic_2ndTiles[(i<<1)+1] << 8);
}

/// Put string to vram map
/// @param bgnum Reserved. Must be 0.
/// @param tx Tile-based x coordinate
/// @param ty Tile-based y coordinate
/// @param str String to be put
/// @remarks ALL chars including control characters are put, except only '\0'.
void PutStr(short bgnum, short tx, short ty, const char *str)
{
    for (short i=0; str[i]; i++)
        vramMap[tx + i + ty * BG_WIDTHT] = str[i];
}
