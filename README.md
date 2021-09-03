# GBA MML Music Player \[Demo\]

## Requirements

- GNU Arm Embedded Toolchain (`binutils-arm-none-eabi` or [Web](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads))
- gcc, make (For x86 or x86_64)
- mGBA (Optional)

## Usage

### To play a demo:

```sh
cd src/
make                # `main.gba' will be made
mgba-qt main.gba
```

The classical song "Gymnopedie No.1 (Erik Satie)" will be played with cute Gameboy sound.

### To add songs:

First, prepare for your song in MML format
(the format must be based on
[Mabinogi](https://mabinogi.nexon.co.jp/) MML
or
[mjapa.jp](https://music-school.mjapa.jp/) MML sequencer)

Reference:

- [Mabinogi MML](https://wikiwiki.jp/mabinogi/%E9%9F%B3%E6%A5%BD/MML)
- [mjapa.jp MML](https://music-school.mjapa.jp/mml_to_midi_converter.html#mml_image)

If you get ready:

```sh
cd mml/
make
./mml2dmg your_mml_filename_here
```

It converts MML to C array forms for `src/songdata.c`.

Copy the output, paste lines beginning with `const` to above `songs` array in the file,
and paste `{~~~_1, ~~~_2, ~~~_3, ~~~_4},` into `songs`.

Finally, change the value of `SONGINDEX_TO_PLAY` (in `main.c`) to your song index in `songs`,
then do as you play a demo (see above).

`mml2dmg` has several options.

- `-pX` shifts pitch by X
- `-oX` shifts octave by X
<!-- - `-vX` tells converter the velocity scale mode (X=w for SMW MML (256), =m for Mabinogi (16)) (Not implemented yet) -->

If you see an error "Note number under 36!" or "Note number over 127!" (meaning "octave out of range"),
try these options.
