AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LIBGCC = `arm-none-eabi-gcc -print-libgcc-file-name`
CFLAGS = -Wall -O -fno-builtin -fomit-frame-pointer -finhibit-size-directive \
-fno-ident

OUTNAME = main
FILES = \
	crt.o ${OUTNAME}.o \
	sounddrv.o songdata.o \
	char_gfx/misaki_gothic_2nd_nega.o \
	char_gfx/misaki_gothic_2nd.o \
	bgobjmgr.o keyinput.o


${OUTNAME}.bin: ${FILES} *.h
	arm-none-eabi-ld -o ${OUTNAME}.out -T gcc.ls \
	${FILES} ${LIBGCC}
	arm-none-eabi-objcopy -O binary ${OUTNAME}.out ${OUTNAME}.bin

all:	clean ${OUTNAME}.bin

emu:	${OUTNAME}.bin
	mv ${OUTNAME}.bin ${OUTNAME}.gba && mgba-qt ${OUTNAME}.gba

clean:
	rm -f *.o *.out *.sav *.gba
