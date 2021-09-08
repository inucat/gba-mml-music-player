# GBA MML Music Player \[Demo\]

:information_source: You can play and add songs.

## Requirements

- GNU Arm Embedded Toolchain (`binutils-arm-none-eabi` or download from [official site](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads))
- gcc, make (For x86 or x86_64)
- mGBA (Optional)

## Usage

### To play a demo:

```sh
    cd src/
    make
    mgba-qt main.gba
```

This demo includes:
- "Gymnopedie No.1" (Erik Satie)
- "12 Variationen über ein französisches Lied 'Ah, vous dirai-je, maman'" (Wolfgang Amadeus Mozart)

### To add songs:

First, prepare your song in MML format.
The format must follow
[Mabinogi](https://mabinogi.nexon.co.jp/) style
or
[mjapa.jp](https://music-school.mjapa.jp/) style (Both are quite similar).

- Reference:
    - [Mabinogi MML](https://wikiwiki.jp/mabinogi/%E9%9F%B3%E6%A5%BD/MML)
    - [mjapa.jp MML](https://music-school.mjapa.jp/mml_to_midi_converter.html#mml_image)

If you get ready:

```sh
    cd mml/
    make
    ./mml2dmg your_mml_filename_here
```

It converts MML to C array format.

Copy the output, paste the array definitions beginning with `const` to above `songs` array in the file `src/songdata.c`,
and paste the line of `{xxx_1, xxx_2, xxx_3, xxx_4},` into `songs`.
`xxx` is your song name.

Finally, run `make` in `src` directory, and play the ROM.

Song index is your song's index in `songs` array.

### About `mml2dmg`

`mml2dmg` has several options.

- `-pX` shifts pitch by X (integer)
- `-oX` shifts octave by X (integer)

If you see an error "Note number under 36!" or "Note number over 127!",
it means that one or more notes are "out of octave range (= not in C2 to A9)".

Then you should try `-o` or `-p` options.

- `-vX` specifies the velocity normalization mode

Available parameters are:

| `X`               | Velocity limit is                  |
| ----------------- | ---------------------------------- |
| `w`               | 255                                |
| `m`               | 15 (like Mabinogi MML, etc.)       |
| (`-v` is omitted) | 127 (like mjapa style, MIDI, etc.) |

## Credit

### Bitmap Font

Free, redistributable and modifiable
"Misaki Font" by Num Kadoma :house: [Web site](https://littlelimit.net/misaki.htm)
