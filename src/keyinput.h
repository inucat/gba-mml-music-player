#ifndef _KEYIO_H_
#define _KEYIO_H_

/// Key being pressed
extern volatile unsigned short keys_input;

/// Key having been pressed previously
extern volatile unsigned short keys_prev;

/// Update key input states
void keys_update(void);

/// @retval If target is holded, 1 is returned; otherwise, 0.
short key_holded(short target);

/// @retval If target is typed and not holded, 1 is returned; otherwise, 0.
short key_pushed(short target);

/// @retval If target turns released, 1 is returned; otherwise, 0.
short key_released(short target);

#endif
