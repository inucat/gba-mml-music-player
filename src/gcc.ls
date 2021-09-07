/* I put this file into the public domain */
OUTPUT_ARCH(arm)

SECTIONS
{
    .text 0x2000000 : {*(.text)}
    .data   : {*(.data)}
    .rodata : {*(.rodata*)}
    .bss    : {*(.bss)}
}
