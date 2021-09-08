#ifndef _KEYIO_H_
#define _KEYIO_H_

/// Key being pressed
extern volatile unsigned short keyCurrent;

/// Key having been pressed previously
extern volatile unsigned short keyPrevious;

/// Update key input states
void KeyStateUpdate(void);

/// Get key holded state
/// @param target Target key from which the holded state will be obtained
/// @retval If target is holded, 1 is returned. Otherwise, 0.
short KeyPressed(short target);

/// Get key typed state
/// @param target Target key from which the typed state will be obtained
/// @retval If target is typed and not holded, 1 is returned. Otherwise, 0.
short KeyTyped(short target);

/// Get key released state
/// @param target Target key from which the released state will be obtained
/// @retval If target is just released, 1 is returned. Otherwise, 0.
short KeyReleased(short target);

#endif
