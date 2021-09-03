#ifndef _MAIN_H_
#define _MAIN_H_

#define reg16(p) *((volatile unsigned short *) (p))
#define reg32(p) *((volatile unsigned int *) (p))


#define IRQ_VECTOR 0x3007FFC
#define IOREG 0x4000000

#define TM2CNT_L ( IOREG + 0x108 )  // Counter
#define TM2CNT_H ( IOREG + 0x10A )  // Control
#define TM_START        0x80
#define TM_PRESC0001    0
#define TM_IRQEN        0x40

#define IME (IOREG + 0x208)
#define IME_OFF 0
#define IME_ON  1

#define IE  (IOREG + 0x200)
#define IF  (IOREG + 0x202)
#define IF_TM2  0x20

typedef unsigned char   byte;
typedef unsigned short  hword;
typedef unsigned int    word;

#endif
