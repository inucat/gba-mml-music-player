# include songs/songs.mk

## Set this value correctly if the compiler tools are NOT in your PATH
BINDIR = /opt/devkitpro/devkitARM/bin/

AS = $(BINDIR)arm-none-eabi-as
CC = $(BINDIR)arm-none-eabi-gcc
LD = $(BINDIR)arm-none-eabi-ld
OBJCOPY = $(BINDIR)arm-none-eabi-objcopy
LIBGCC = `$(CC) -print-libgcc-file-name`
CFLAGS = -Wall -O -fno-builtin -fomit-frame-pointer -finhibit-size-directive -fno-ident

OUTNAME = main

OSONGS = \
	song_ttls.o \
	song_gymno1.o

OFILES = \
	crt.o $(OUTNAME).o \
	sounddrv.o songdata.o \
	char_gfx/misaki_gothic_2nd_nega.o \
	char_gfx/misaki_gothic_2nd.o \
	bgobjmgr.o keyinput.o \
	$(OSONGS)


.PHONY: all clean

$(OUTNAME).gba: $(OFILES) *.h
	$(LD) -o $(OUTNAME).out -T gcc.ls $(OFILES) $(LIBGCC)
	$(OBJCOPY) -O binary $(OUTNAME).out $(OUTNAME).gba

clean:
	-rm -f *.o *.out *.sav *.gba
