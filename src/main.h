#ifndef _MAIN_H_
#define _MAIN_H_

#define reg16(p) *((volatile unsigned short *) (p))
#define reg32(p) *((volatile unsigned int *) (p))

#define IRQ_VECTOR 0x3007FFC
#define IOREG   0x4000000

#define LCDCNT          (IOREG)
#define LCDCNT_BG0ON    0x0100
#define LCDCNT_BG1ON    0x0200
#define LCDCNT_BG2ON    0x0400
#define LCDCNT_BG3ON    0x0800
/// i = 0 ~ 5 (6, 7 are prohibited)
#define LCDCNT_BGMODE(i) ((i)&7)


#define BG0CNT  (IOREG + 0x8)
#define BG1CNT  (IOREG + 0xA)
#define BG2CNT  (IOREG + 0xC)
#define BG3CNT  (IOREG + 0xE)
#define BGCNT_PRIOR1    0x0000
#define BGCNT_PRIOR2    0x0001
#define BGCNT_PRIOR3    0x0002
#define BGCNT_PRIOR4    0x0003
#define BGCNT_COL256    0x0080
#define BGCNT_MAP3232   0x0000
#define BGCNT_MAP6432   0x4000
#define BGCNT_MAP3264   0x8000
#define BGCNT_MAP6464   0xC000
/// n = 0 ~ 3
#define BGCNT_TILENo(n) ((n)<<2)
/// n = 0 ~ 31
#define BGCNT_MAPNo(n)  ((n)<<8)


#define LCDSTAT (IOREG + 0x4)
#define LCDSTAT_VBMASK      0x0001
#define LCDSTAT_VBIRQEN     0x0008


#define PALETTE_BG  0x5000000
#define PALETTE_OBJ 0x5000200
#define PALETTE_LEN 0x200


#define VRAM    0x6000000
/// a = 0 ~ 3
#define VRAM_TILE(a)   (VRAM + (a)*0x4000)
/// a = 0 ~ 31
#define VRAM_MAP(a)    (VRAM + (a)*0x0800)


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
