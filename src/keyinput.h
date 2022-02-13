#ifndef _KEYIO_H_
#define _KEYIO_H_

/// Key being pressed
extern volatile unsigned short keyCurrent;

/// Key having been pressed previously
extern volatile unsigned short keyPrevious;

/// Update key input states
void KeyStateUpdate(void);

/// @retval If target is holded, 1 is returned; otherwise, 0.
short KeyPressed(short target);

/// @retval If target is typed and not holded, 1 is returned; otherwise, 0.
short KeyTyped(short target);

/// @retval If target turns released, 1 is returned; otherwise, 0.
short KeyReleased(short target);

#endif
