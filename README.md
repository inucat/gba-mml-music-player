# GBA MML Music Player \[Demo\]

## Requirements

- GNU Arm Embedded Toolchain (`binutils-arm-none-eabi` or download from [official site](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads))
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

First, prepare your song in MML format.
The format must follow
[Mabinogi](https://mabinogi.nexon.co.jp/) style
or
[mjapa.jp](https://music-school.mjapa.jp/) style (Both are quite simillar).

- Reference:
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
and paste the line of `{~~~_1, ~~~_2, ~~~_3, ~~~_4},` into `songs`.
`~~~` is your song name.

Finally, change the value of `SONGINDEX_TO_PLAY` in `main.c` to your song index of `songs`,
then do as you play a demo (see above).

`mml2dmg` has several options.

- `-pX` shifts pitch by X (integer)
- `-oX` shifts octave by X (integer)
<!-- - `-vX` tells converter the velocity scale mode (X=w for SMW MML (256), =m for Mabinogi (16)) (Not implemented yet) -->

If you see an error "Note number under 36!" or "Note number over 127!",
it means that one or more notes are "out of octave range (= not in C2 to A9)".
If so, try these options.

## Credit

### Bitmap Font

Free, redistributable and modifiable
"Misaki Font" by Num Kadoma :house: [Web site](https://littlelimit.net/misaki.htm)
