# MML Style Overview

## Commands

All commands are *case-sensitive*.
Commands can be saparated by spaces, but no spaces can be put between a command and its parameters.

| Command     | Meaning                           |
| ----------- | --------------------------------- |
| `a` to `g`  | Note of A, B, ... G               |
| `+` and `-` | Sharp and flat                    |
| `r`         | Rest                              |
| `l`         | Perpetual length                  |
| `.`         | Dot                               |
| `o`         | Perpetual octave                  |
| `<` and `>` | Perpetual octave shift (-1, +1)   |
| `_` and `~` | Single octave shift (-1, +1)      |
| `t`         | Tempo                             |
| `;` and `,` | "End of Track" Marker             |
| `v`         | Perpetual velocity                |
| `[` and `]` | Loop section                      |
| `N`         | (Exclusive) Noise parameter       |
| `S`         | (Exclusive) Sweep control         |
| ---         | **Below are NOT YET implemented** |
| `&`         | Tie                               |
| `w`         | Whole volume                      |
| `'`         | Single velocity                   |
| `/* ... */` | Comment                           |

## Remarks

Notes and rests can be accompanied with its length next to them (e.g. `a4`).
The default length is 4, the length of a quarter note.

One or two dots can be put right of them.
If the length and dot(s) are put together, the dot(s) must be put after the length (e.g. `c8 f+2. b-..`).

Length command specifies the note value of all notes or rests after it.
Its numeric parameter (1-255) can also be followed by dot(s) (e.g. `l3 l8. l2..`).
Irregular values (the length not multiples of 2 or 3) are not recommended due to the sound driver precision.

Octave ranges between 2 and 9, or exactly, from **C2** to **G9**.
It is 4 if not specified.
Notes out of the range cause a conversion failure.

Tempo can vary from 1 to 510.
Extremely slow or fast tempo may cause glitches due to the tempo implementation.

Each track must be separated by an "End of Track" Marker (`;` or `,`).
Track 1-4 of MML will be converted to GBA Sound 1-4 respectively.
Any other tracks will be ignored.

Perpetual velocity is, to be exact, the initial envelope volume from 0 to 15.
Velocity scaling can be specified for the conversion from other style MML,
which may have different velocity limit to 15.

Loop is useful to reduce not only the MML length but also the binary size.
Maximum loop count is needed after the section termination marker (e.g. `[cegb]3`).
It will be assumed to be 1 if omitted.
Note that loops do not save octave shift data (exclusive spec for my driver).
For example, `o4 [ ceg > c ]2` plays the sequence of "C4 E4 G4 C5" twice,
not of "C4 E4 G4 C5 C5 E5 G5 C6".

## Exclusive commands for my GBA sound driver

The hex parameter is *case-insensitive* unlike commands.

### Noise

#### Syntax

```
    Nxy[+|-][length][.|..]
```

Note: `[ *** ]` means that `***` can be omitted.

##### `x`

Noise clock prestep fed to bit 7-4 of 0x4000078.

The value can be between 0 and F (0 1 .. 9 A B .. F).

Corresponds to the parameter `p` of `RLN_pstep(p)` macro.

##### `y`

Noise clock divisor fed to bit 2-0 of 0x4000078.

The value can be between 0 and F (0 1 .. 9 A B .. F).

Corresponds to the parameter `d` of `RLN_div(d)` macro.

If `y` is more than 7, it implicitly sets the counter 7-step flag,
and `d` will be `y & 0x07` (bit-wise AND).

##### `+`/`-`

Both symbols explicitly set the counter step to 7.

##### `length`, `.`/`..`

Same as that of the normal notes.

### Sweep

#### Syntax

```
    Sxy[+|-]
```

##### `x`

Sweep time fed to 0x4000060 bit 6-4.

The value can be between 0 and 7.

This command has perpetual effects and needs to be turned off by setting `x` to zero, if you want.

Corresponds to the parameter `d` of `SWEEP_time(d)` macro.

##### `y`

Sweep shift value fed to 0x4000060 bit 2-0.

The value can be between 0 and 7.

Corresponds to the parameter `s` of `SWEEP_shift(s)` macro.

##### `+`/`-`

`-` sets sweep direction flag to 1 (the frequency is decreasing).

`+` sets it to 0 (the frequency is increasing).

The default behaviour is *increasing*.
