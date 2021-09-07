#ifndef _BGOBJMGR_H_
#define _BGOBJMGR_H_
#define BG_WIDTHT 32

/// Put string to vram map
/// @param bgnum Reserved. Must be 0.
/// @param tx Tile-based x coordinate
/// @param ty Tile-based y coordinate
/// @param str String to be put
/// @remarks ALL chars including control characters are put, except only '\0'.
void PutStr(
    short bgnum,
    short tx,
    short ty,
    const char *str);

void BgInit(void);
#endif
