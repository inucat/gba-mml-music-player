# GBA MML Music Player (Demo)

You can play and add songs.

## Requirements

- gcc, make (For amd64)
- GNU Arm Embedded Toolchain
  - With your package manager (distributed as `binutils-arm-none-eabi` or something)
  - Direct download from [official site](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads))
  - From [devkitPro](https://devkitpro.org/) devkitARM (feature-rich, multi-purpose with other utilities)
- Emulator such as mGBA (Optional)

## Usage

### To play a demo:

```sh
    cd src/
    make
```

And feed the `main.gba` to your GBA emulator or actual device.

This demo includes:

- "Gymnopedie No.1" (Erik Satie)
- "12 Variationen über ein französisches Lied 'Ah, vous dirai-je, maman'" (Wolfgang Amadeus Mozart)

### To add songs:

First, prepare your song in MML format.
The MML style supported are
[Mabinogi](https://mabinogi.nexon.co.jp/) style
and
[mjapa.jp](https://music-school.mjapa.jp/) style (Both are quite similar).
Note that not all syntaxes are recognized so far.

- Reference:
  - [Mabinogi MML](https://wikiwiki.jp/mabinogi/%E9%9F%B3%E6%A5%BD/MML)
  - [mjapa.jp MML](https://music-school.mjapa.jp/mml_to_midi_converter.html#mml_image)

Now basic syntax manual is available: [MML Style Overview](./mml/AboutMMLStyle.md)

If you get ready:

```sh
    cd mml/
    make
    ./mml2dmg your_mml_filename_here
```

It converts MML to C array format.

(TODO: automate procedures from here.)

Copy the output, paste the array definitions beginning with `const` to the separate file in the `src/` (e.g. `song_newsong.c`).

Add the declaration of tracks of your song to the file (e.g. `const unsigned char *tracks_newsong[4] ={xxx_1, xxx_2, xxx_3, xxx_4}`).
`xxx` is your song name.

Create a new header (e.g. `src/song_newsong.h`) with contents similar to the following:

```c
extern const unsigned char *tracks_gymno1[4];
```

(TODO: automate procedures until here.)

Modify `songdata.c` to include the header file, and add the track to `songs` array, like:

```c
#include "song_newsong.h"

const uint8 **songs[] = {
    tracks_gymno1, tracks_ttls, tracks_newsong
    0 // sentinel
};
```

Then add the object file requisites to `OSONGS` in `Makefile`. It will look like this:

    OSONGS = \
        song_ttls.o \
        song_gymno1.o \
        song_newsong.o

Finally, run `make` in `src` directory, and play the ROM.

The songs are indexed in the order of `songs` array.

### About `mml2dmg`

`mml2dmg` has several options.

- `-pX` shifts pitch by X (integer)
- `-oX` shifts octave by X (integer)

If you see an error "Note number under 36!" or "Note number over 127!",
it means that one or more notes are "out of octave range (= not in C2 to A9)".

Then you should try `-o` or `-p` options.

- `-vX` specifies the velocity scaling mode

Available parameters are:

| `X`               | Maximum velocity is                |
| ----------------- | ---------------------------------- |
| `w`               | 255                                |
| `m`               | 15 (like Mabinogi MML, etc.)       |
| (`-v` is omitted) | 127 (like mjapa style, MIDI, etc.) |

## Credit

### Bitmap Font

Free, redistributable and modifiable
"Misaki Font" by Num Kadoma :house: [Web site](https://littlelimit.net/misaki.htm)
