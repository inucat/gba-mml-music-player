/**
 * @file exgba.h
 * @brief GBA definitions (not comprehensive)
 * @author inucat
 * @note BSD 3-Clause License
 */
#ifndef _GBA_H_
#define _GBA_H_

/// Type definitions

typedef unsigned char   byte, uint8;
typedef unsigned short  hword, uint16;
typedef unsigned int    word, uint32;

/// Register access

#define reg16(p) *((volatile unsigned short *) (p))
#define reg32(p) *((volatile unsigned int *) (p))

/// Memory Map

#define IWORKRAM        0x3000000
#define IRQ_VECTOR      0x3007FFC
#define IOREG           0x4000000
#define PALETTE_BG      0x5000000
#define PALETTE_OBJ     0x5000200
#define VRAM            0x6000000
#define VRAM_TILE(n)    ((VRAM) + ((n) << 14))
#define VRAM_MAP(n)     ((VRAM) + ((n) << 11))
#define OBJ_TILE        0x6010000
#define OBJ_BITMAP      0x6014000
#define OAM             0x7000000

/// LCD

#define LCD_PWIDTH  240
#define LCD_PHEIGH  160
#define LCD_VTWIDT  32
#define LCD_VTHEIG  32
#define LCD
#define LCDCNT          (IOREG)
#define     LCDCNT_BG0ON    0x0100
#define     LCDCNT_BG1ON    0x0200
#define     LCDCNT_BG2ON    0x0400
#define     LCDCNT_BG3ON    0x0800
#define     LCDCNT_OBJON    0x1000
#define     LCDCNT_OBJ2D    0
#define     LCDCNT_OBJ1D    0x0040
/// i = 0 ~ 5 (6, 7 are prohibited)
#define     LCDCNT_BGMODE(i) ((i)&7)
#define LCDSTAT (IOREG + 0x4)
#define     LCDSTAT_VBMASK      0x0001
#define     LCDSTAT_VBIRQEN     0x0008

/// BG

#define BG0CNT  (IOREG + 0x8)
#define BG1CNT  (IOREG + 0xA)
#define BG2CNT  (IOREG + 0xC)
#define BG3CNT  (IOREG + 0xE)
#define     BGCNT_PRIOR1    0x0000
#define     BGCNT_PRIOR2    0x0001
#define     BGCNT_PRIOR3    0x0002
#define     BGCNT_PRIOR4    0x0003
#define     BGCNT_COL16     0
#define     BGCNT_COL256    0x0080
#define     BGCNT_MAP3232   0x0000
#define     BGCNT_MAP6432   0x4000
#define     BGCNT_MAP3264   0x8000
#define     BGCNT_MAP6464   0xC000
/// n = 0 ~ 3
#define     BGCNT_TILENo(n) ((n)<<2)
/// n = 0 ~ 31
#define     BGCNT_MAPNo(n)  ((n)<<8)
#define BG0OFSH (IOREG + 0x10)
#define BG0OFSV (IOREG + 0x12)
#define BG1OFSH (IOREG + 0x14)
#define BG1OFSV (IOREG + 0x16)
#define BG2OFSH (IOREG + 0x18)
#define BG2OFSV (IOREG + 0x1A)
#define BG3OFSH (IOREG + 0x1C)
#define BG3OFSV (IOREG + 0x1E)

/// Keypad

#define KEYINPUT    (IOREG + 0x130)
#define KEYIRQCNT   (IOREG + 0x132)
#define     KEY_A           0x0001
#define     KEY_B           0x0002
#define     KEY_SEL         0x0004
#define     KEY_STA         0x0008
#define     KEY_DR          0x0010
#define     KEY_DL          0x0020
#define     KEY_DU          0x0040
#define     KEY_DD          0x0080
#define     KEY_SR          0x0100
#define     KEY_SL          0x0200
#define     KEY_ALL         0x03FF
#define     KEYIRQCNT_ON    0x4000
#define     KEYIRQCNT_AND   0x8000


/// Palette

#define PALETTE_BG  0x5000000
#define PALETTE_OBJ 0x5000200
#define PALETTE_LEN 0x200
#define PALETTE_COL 256

/// Timer

#define TM0COUNT    ( IOREG + 0x100 )
#define TM1COUNT    ( IOREG + 0x104 )
#define TM2COUNT    ( IOREG + 0x108 )
#define TM3COUNT    ( IOREG + 0x10C )
#define TM0CTRL     ( IOREG + 0x102 )
#define TM1CTRL     ( IOREG + 0x106 )
#define TM2CTRL     ( IOREG + 0x10A )
#define TM3CTRL     ( IOREG + 0x10E )
#define     TM_PRESC0001    0x00
#define     TM_PRESC0064    0x01
#define     TM_PRESC0256    0x02
#define     TM_PRESC1024    0x03
#define     TM_CASCADE      0x04
#define     TM_IRQEN        0x40
#define     TM_START        0x80

/// IRQ

#define IME (IOREG + 0x208)
#define     IME_OFF 0
#define     IME_ON  1
#define IE  (IOREG + 0x200)
#define IF  (IOREG + 0x202)
#define     IF_VBL  0x0001
#define     IF_TM0  0x0008
#define     IF_TM1  0x0010
#define     IF_TM2  0x0020
#define     IF_TM3  0x0040
#define     IF_DMA0 0x0100
#define     IF_DMA1 0x0200
#define     IF_DMA2 0x0400
#define     IF_DMA3 0x0800
#define     IF_KEY  0x1000

/// Direct Sound

#define SNDMASTER   (IOREG + 0x84)
#define     SNDMASTER_ON    0x80
#define SNDCNT  (IOREG + 0x82)
#define     SNDCNT_SAFULL   0x0004
#define     SNDCNT_SBFULL   0x0008
#define     SNDCNT_SARIGHT  0x0100
#define     SNDCNT_SALEFT   0x0200
#define     SNDCNT_SATMR0   0x0000
#define     SNDCNT_SATMR1   0x0400
#define     SNDCNT_SARESET  0x0800
#define     SNDCNT_SBRIGHT  0x1000
#define     SNDCNT_SBLEFT   0x2000
#define     SNDCNT_SBTMR0   0x0000
#define     SNDCNT_SBTMR1   0x4000
#define     SNDCNT_SBRESET  0x8000
#define SNDFIFO_A (IOREG + 0xA0)
#define SNDFIFO_B (IOREG + 0xA4)

/// DMA

#define DMA0SAD     (IOREG + 0xB0)
#define DMA0DAD     (IOREG + 0xB4)
#define DMA0COUNT   (IOREG + 0xB8)
#define DMA0CNT     (IOREG + 0xBA)
#define DMA1SAD     (IOREG + 0xBC)
#define DMA1DAD     (IOREG + 0xC0)
#define DMA1COUNT   (IOREG + 0xC4)
#define DMA1CNT     (IOREG + 0xC6)
#define DMA2SAD     (IOREG + 0xC8)
#define DMA2DAD     (IOREG + 0xCC)
#define DMA2COUNT   (IOREG + 0xD0)
#define DMA2CNT     (IOREG + 0xD2)
#define DMA3SAD     (IOREG + 0xD4)
#define DMA3DAD     (IOREG + 0xD8)
#define DMA3COUNT   (IOREG + 0xDA)
#define DMA3CNT     (IOREG + 0xDE)
#define     DMACNT_DADINC   0x0000
#define     DMACNT_DADDEC   0x0020
#define     DMACNT_DADFIX   0x0040
#define     DMACNT_DADREL   0x0060
#define     DMACNT_SADINC   0x0000
#define     DMACNT_SADDEC   0x0080
#define     DMACNT_SADFIX   0x0100
#define     DMACNT_REPEAT   0x0200
#define     DMACNT_SIZE16   0x0000
#define     DMACNT_SIZE32   0x0400
#define     DMACNT_STAIMD   0x0000
#define     DMACNT_STAVBL   0x1000
#define     DMACNT_STAHBL   0x2000
#define     DMACNT_STAFIFO  0x3000
#define     DMACNT_IRQENB   0x4000
#define     DMACNT_ENABLE   0x8000

/// OBJ

#define OBJMAXNUMBER    128
#define OBJMAXIMAGE     1024

#define OBJA0_YMASK         0x00FF
#define OBJA0_ROTATE_ON     0x0100
#define OBJA0_DOUBLESIZE    0x0200
#define OBJA0_HIDDEN        0x0200
#define OBJA0_COL256        0x2000
#define OBJA0_SHAPESQ       0x0000
#define OBJA0_SHAPEHZ       0x4000
#define OBJA0_SHAPEVT       0x8000

#define OBJA1_XMASK 0x01FF
#define OBJA1_RSPMASK   0x3E00
#define OBJA1_FLIP_H      0x1000
#define OBJA1_FLIP_V      0x2000
#define OBJA1_FLIPMASK   0x3000

#define OBJA2_TILEIDMASK    0x03FF
#define OBJA2_PRIORMASK     0x0C00

typedef struct tagSPRITEATTR {
    hword attr0;
    hword attr1;
    hword attr2;
    hword dummy;
} SPRITEATTR;

typedef struct tagRSPARAMETER {
    hword dummy1[3];
    hword pa;
    hword dummy2[3];
    hword pb;
    hword dummy3[3];
    hword pc;
    hword dummy4[3];
    hword pd;
} RSPARAMETER;

/// 8x8 px, 64 Bytes.
typedef struct tagOBJIMG {
    hword data[32];
} OBJIMG;

enum OBJVISIBILITY {
    OBJ_OFF,
    OBJ_ON
};

enum OBJSIZEINDEX {
    OBJSIZEINDEX_0808,
    OBJSIZEINDEX_1616,
    OBJSIZEINDEX_3232,
    OBJSIZEINDEX_6464,
    OBJSIZEINDEX_1608,
    OBJSIZEINDEX_3208,
    OBJSIZEINDEX_3216,
    OBJSIZEINDEX_6432,
    OBJSIZEINDEX_0816,
    OBJSIZEINDEX_0832,
    OBJSIZEINDEX_1632,
    OBJSIZEINDEX_3264
};

#define OBJDATASIZE_0808    1
#define OBJDATASIZE_1616    4
#define OBJDATASIZE_3232    16
#define OBJDATASIZE_6464    64
#define OBJDATASIZE_1608    2
#define OBJDATASIZE_3208    4
#define OBJDATASIZE_3216    8
#define OBJDATASIZE_6432    32
#define OBJDATASIZE_0816    2
#define OBJDATASIZE_0832    4
#define OBJDATASIZE_1632    8
#define OBJDATASIZE_3264    32

enum BGOBJ_PRIORITY_INDEX {
    BGOBJ_PRIOR1,
    BGOBJ_PRIOR2,
    BGOBJ_PRIOR3,
    BGOBJ_PRIOR4
};

#endif
