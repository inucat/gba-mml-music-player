#include "meta.h"

volatile hword keyCurrent = 0;
volatile hword keyPrevious = 0;

short KeyPressed(short target) {
    return (keyCurrent & target) && 1;
}

short KeyTyped(short target) {
    return (target & keyCurrent & ~keyPrevious) && 1;
}

short KeyReleased(short target) {
    return (target & ~keyCurrent & keyPrevious) && 1;
}

__inline__ void KeyStateUpdate(void)
{
    keyPrevious = keyCurrent;
    keyCurrent = reg16(KEYINPUT) ^ KEY_ALL;
}
