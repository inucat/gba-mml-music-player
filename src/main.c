#include "main.h"
#include "songdata.h"
#include "sounddrv.h"

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
    dmginit();
    dmgload(SONGINDEX_TO_PLAY);
    dmgplay();
    while(1);

    return 0;
}
