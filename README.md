# FMTK

[![FMTK Logo](https://github.com/widberg/fmtk/blob/master/docs/source/img/icon.png?raw=true)](#)

FUEL Modding Toolkit

[![Build Status](https://github.com/widberg/fmtk/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/widberg/fmtk/actions/workflows/build.yml)
[![Documentation Status](https://readthedocs.org/projects/fmtk/badge/?version=latest)](https://fmtk.readthedocs.io/en/latest/?badge=latest)
[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/widberg/fmtk)](https://github.com/widberg/fmtk/releases)
[![Join the chat at https://discord.gg/CQgMNbYeUR](https://img.shields.io/badge/chat-on_discord-7389D8.svg?logo=discord&logoColor=ffffff&labelColor=6A7EC2)](https://discord.gg/CQgMNbYeUR)

## Features

* Fully compatible with "FUEL: REFUELED", "FUEL: RESHADED", and other legacy mods by default
* [Luau](https://luau.org/) scripting
* Callback hooks to game events
* Interface functions to peek and poke game data
* Works with Games for Windows Live if present
* Better error messages for shaders and crashes
* Cool "Blue FUEL" launcher logo [:)](https://github.com/widberg/fmtk/wiki/Logo-Lore)

## Compatibility

FMTK is fully compatible with legally obtained retail and Steam release copies of FUEL. Pirated versions of the game are not explicitly compatible with FMTK although they may work.

## Installing (For Users and Mod Developers)

1. Download the latest release from the [releases page](https://github.com/widberg/fmtk/releases). This will be the asset with a name matching the pattern `fmtk-*.zip`.
2. Unzip the release into the root FUEL directory (the directory containing `FUEL.exe`)
3. Verify that `fmtk.exe` and `FUEL.exe` are in the same directory.
4. Run `fmtk.exe`.

In the event of a crash or other game state locking event, i.e. "Error: Shaders processing has failed."  or "The Game Disc could not be read.", please check the `fmtk.log` file in the `mods/fmtk` directory. If you cannot resolve the reported error yourself then please open an issue or contact an FMTK developer directly.

**FMTK mods are only loaded when the fmtk.exe "Blue FUEL" launcher is used to run the game. To run the game without mods enabled, simply launch it how you normally would without FMTK; uninstalling FMTK is unnecessary. You can uninstall FMTK and all FMTK mods at any time by deleting the `mods` directory.**

## Discord

Join the [Zouna Underground Discord](https://discord.gg/CQgMNbYeUR) for general Asobo Studio modding, beyond and including FUEL. If you plan on joining to ask a specific question, please read the [FMTK FAQ](https://github.com/widberg/fmtk/wiki/FAQ) first to see if it has already been answered.

## Documentation

Documentation for users and mod developers is available on [Read the Docs](https://fmtk.rtfd.io). Documentation for FMTK developers is available on the [FMTK Wiki](https://github.com/widberg/fmtk/wiki). Bad documentation is a bug! If a piece of documentation is incomplete/incorrect open an issue so it can be improved.

## Contributing

See the [FMTK Contributing Guidelines](https://github.com/widberg/fmtk/blob/master/CONTRIBUTING.md). For an inexhaustive list of things you can help with check out the [How To Help](https://github.com/widberg/fmtk/wiki/How-To-Help) wiki entry.

## Related Projects

These are projects that are related to researching FUEL but not directly part of the modding process.

* [bff](https://github.com/widberg/bff) - A GUI/CLI utility to work with Zouna BigFiles
* [fuel-map](https://github.com/widberg/fuel-map) - Information about the map and a website
* [fuel-save-editor](https://github.com/widberg/fuel-save-editor) - Save editor for Asobo's FUEL
* [blender_fuel](https://github.com/widberg/blender_fuel) - Blender scripts for object formats
* [fuel-brute](https://github.com/widberg/fuel-brute) - CRC32 Brute Force Solver For FUEL
* [atk](https://github.com/widberg/atk) - Frida agent for asobo games
* [ImZouna](https://github.com/widberg/ImZouna) - ImHex patterns for Zouna data structures
* ~~[dpc](https://github.com/widberg/dpc) - Tar analogous utility to work with the DPC file format~~
* ~~[fmt_fuel](https://github.com/widberg/fmt_fuel) - Noesis scripts for object formats~~

## Support

You can support the project by [contributing](#Contributing).

## Getting Started (For FMTK Developers)

### Prerequisites

* Git
* CMake
* [llvm-project-widberg-extensions](https://github.com/widberg/llvm-project-widberg-extensions)
* Any Win32 toolchain
* A PC copy of FUEL (Optional)

### Checkout

```sh
git clone https://github.com/widberg/fmtk.git --recurse-submodules --shallow-submodules
```

### Building On Windows

Use the `x86 Native Tools Command Prompt for VS 2022` environment while generating and building the
project.

#### Ninja

```sh
cmake -B build -G Ninja -DLLVM_WIDBERG="/path/to/llvm-project-widberg-extensions/"
cmake --build build
```
