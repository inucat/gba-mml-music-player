#ifndef _BGOBJMGR_H_
#define _BGOBJMGR_H_

/// BG constants for Layout

#define BGL_MENU_KEY_TX     2
#define BGL_MENU_VALUE_TX   22

enum BGNUMBER_FOR_GUI_ELEMENTS {
    BGN_WINDOWFRAME,
    BGN_MENULIST,
    BGN_BACKDROP2,
    BGN_BACKDROP3
};

void BgMakeFrame(void);

/// Put a tile
/// @param bgnum Specify which BG to put the string to (0-3)
/// @param tx Tile-based x coordinate
/// @param ty Tile-based y coordinate
/// @param tileId Tile ID (0-1023?)
/// @remarks ALL tiles including Id=0 are put.
void PutTile(
    short bgnum,
    short tx,
    short ty,
    short tileId);

/// Put string to vram map
/// @param bgnum Specify which BG to put the string to (0-3)
/// @param tx Tile-based x coordinate
/// @param ty Tile-based y coordinate
/// @param str String to be put
/// @remarks ALL chars including control characters are put, except only '\0'.
void PutStr(
    short bgnum,
    short tx,
    short ty,
    const char *str);

/// Put a short value to vram map
/// @param bgnum Specify which BG to put the string to (0-3)
/// @param tx Tile-based x coordinate
/// @param ty Tile-based y coordinate
/// @param num Value to put
/// @remarks Right-aligned.
void PrintShort(
    short bgnum,
    short tx,
    short ty,
    short num);

void BgInit(void);
#endif
