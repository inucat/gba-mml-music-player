/* This file is in the Public Domain */
OUTPUT_ARCH(arm)

SECTIONS
{
    .text 0x2000000 : {*(.text)}
    .data   : {*(.data)}
    .rodata : {*(.rodata*)}
    .bss    : {*(.bss)}
}
