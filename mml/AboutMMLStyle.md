# MML Style Overview

## Commands

| Command | Meaning                           |
| ------- | --------------------------------- |
| `l`     | Perpetual length (note value)     |
| `o`     | Perpetual octave                  |
| `<`,`>` | Octave shift (lower, heighten)    |
| `t`     | Tempo (BPM)                       |
| `_`,`~` | (mjapa only?) Single octave shift |
| `v`     | Perpetual velocity (unstable)     |
| `N`     | (Exclusive) Noise parameter       |
| `S`     | (Exclusive) Sweep control         |
| ---     | **↓Not yet implemented↓**         |
| `&`     | Tie                               |
| `w`     | Whole volume                      |
| `'`     | (mjapa only?) Single velocity     |

Exclusive commands of this sound driver are described below.

The hex parameter is case-insensitive.

### `N` Noise

#### Syntax

```
    Nxy[+|-][length][.|..]
```

Note: `[ *** ]` means that `***` can be omitted.

##### `x`

Noise clock prestep fed to 0x4000078 bit 7-4.

The value can be between 0 and F (0 1 .. 9 A B .. F).

Corresponds to the parameter `p` of `RLN_pstep(p)` macro.

##### `y`

Noise clock divisor fed to 0x4000078 bit 2-0.

The value can be between 0 and F (0 1 .. 9 A B .. F).

Corresponds to the parameter `d` of `RLN_div(d)` macro.

If `y` is more than 7, it sets the counter step to 7 (default is 15).

##### `+`/`-`

Both symbols set the counter step to 7.

##### `length`, `.`/`..`

Same as that of the normal notes.

### `S` Sweep

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
