# FMTK

[![FMTK Logo](https://github.com/widberg/fmtk/blob/master/docs/source/img/logo.png?raw=true)](https://github.com/widberg/fmtk#fmtk)

FUEL Modding Toolkit

[![Build Status](https://github.com/widberg/fmtk/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/widberg/fmtk/actions/workflows/build.yml)
[![Documentation Status](https://readthedocs.org/projects/fmtk/badge/?version=latest)](https://fmtk.readthedocs.io/en/latest/?badge=latest)
[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/widberg/fmtk)](https://github.com/widberg/fmtk/releases)
[![Join the chat at https://discord.gg/CQgMNbYeUR](https://img.shields.io/badge/chat-on_discord-7389D8.svg?logo=discord&logoColor=ffffff&labelColor=6A7EC2)](https://discord.gg/CQgMNbYeUR)

## Features

* Fully compatible with "FUEL: REFUELED", "FUEL: RESHADED", and other legacy mods by default
* Lua scripting engine
* C++ Native Mod interface for advanced mods
* Callback hooks to game events
* Interface functions to peek and poke game data
* Full integration with the in-game command palette
* Works with Games for Windows Live if present
* Better error messages for shaders and crashes
* File aliasing (like a programmatic REFUELED)
* Seamless mod compatibility so everyone plays nice
* Configurable launcher and mods
* Cool "Blue FUEL" launcher logo [:)](https://github.com/widberg/fmtk/wiki/Logo-Lore)
* Easy to install for inexperienced users

## Compatibility

FMTK is fully compatible with legally obtained retail and Steam release copies of FUEL. Pirated versions of the game are not explicitly compatible with FMTK although they may work.

## Installing (For Users and Mod Developers)

1. Verify that your FUEL installation is stock and all of the executable files have their original names.
  - If you have renamed `FUEL.exe`, `GameSetup.exe`, or `SecuLauncher.exe`, now would be the time to revert them to their original names. If you have a Steam copy, this can be done with the "Verify integrity of game files..." button in the "Local Files" tab of FUEL's Steam "Properties..." menu.
  - This requirement will be removed in future releases once fingerprinting is implemented.
2. Download the latest release from the [releases page](https://github.com/widberg/fmtk/releases). This will be the asset with a name matching the pattern `fmtk-*.zip`.
3. Unzip the release into the root FUEL directory (the directory containing `FUEL.exe`)
4. Verify that the `mods` directory and `FUEL.exe` are in the same directory.
5. Run `fmtk.exe` in the `mods/fmtk` directory.

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

* [dpc](https://github.com/widberg/dpc) - Tar analogous utility to work with the DPC file format
* [fuel-brute](https://github.com/widberg/fuel-brute) - CRC32 Brute Force Solver For FUEL
* [fuel-map](https://github.com/widberg/fuel-map) - It's just information about the map
* [fmt_fuel](https://github.com/widberg/fmt_fuel) - Noesis scripts for object formats
* [blender_fuel](https://github.com/widberg/blender_fuel) - Blender scripts for object formats
* [atk](https://github.com/widberg/atk) - Frida agent for asobo games
* [fuel-save-editor](https://github.com/widberg/fuel-save-editor) - Save editor for Asobo's FUEL
* [ImFUEL](https://github.com/widberg/ImFUEL) - ImHex patterns for FUEL data structures

## Support

You can support the project by [contributing](#Contributing).

## Getting Started (For FMTK Developers)

### Prerequisites

* Git
* CMake
* Bison or [WinFlexBison](https://github.com/lexxmark/winflexbison)
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
cmake -B build -G Ninja -DWIDBERG_CLANG_CL="/path/to/llvm-project-with-widberg-extensions/clang-cl.exe" -DCMAKE_LINKER="link.exe"
cmake --build build
```
