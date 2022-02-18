#include "meta.h"

volatile hword keys_input = 0;
volatile hword keys_prev = 0;

short key_holded(short target) {
    return (keys_input & target) && 1;
}

short key_pushed(short target) {
    return (target & keys_input & ~keys_prev) && 1;
}

short key_released(short target) {
    return (target & ~keys_input & keys_prev) && 1;
}

__inline__ void keys_update(void) {
    keys_prev = keys_input;
    keys_input = reg16(KEYINPUT) ^ KEY_ALL;
}
