# FMTK

[![FMTK Logo](https://github.com/widberg/fmtk/blob/master/docs/source/img/logo.png?raw=true)](https://github.com/widberg/fmtk#fmtk)

FUEL Modding Toolkit

[![Build Status](https://github.com/widberg/fmtk/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/widberg/fmtk/actions/workflows/build.yml)
[![Documentation Status](https://readthedocs.org/projects/fmtk/badge/?version=latest)](https://fmtk.readthedocs.io/en/latest/?badge=latest)
[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/widberg/fmtk)](https://github.com/widberg/fmtk/releases)
[![Join the chat at https://discord.gg/8uJVY9mWZT](https://img.shields.io/badge/chat-on_discord-7389D8.svg?logo=discord&logoColor=ffffff&labelColor=6A7EC2)](https://discord.gg/8uJVY9mWZT)

## Features

* Fully compatible with "FUEL: REFUELED", "FUEL: RESHADED", and other legacy mods by default
* Lua scripting engine
* C++ Native Plugin interface for advanced mods
* Callback hooks to game events
* Interface functions to peek and poke game data
* Full integration with the in-game command palette
* Better error messages for shaders and crashes
* File aliasing (like a programmatic REFUELED)
* Seamless mod compatibility so everyone plays nice
* Configurable launcher and mods
* Cool "Blue FUEL" launcher logo :)
* Easy to install for inexperienced users

## Compatibility

FMTK is fully compatible with legally obtained retail and Steam release copies of FUEL. Pirated versions of the game are not explicitly compatible with FMTK although they may work.

## Installing (For Users and Mod Developers)

1. Verify that your FUEL installation is stock and all of the executable files have their original names.
  - If you have renamed `FUEL.exe`, `GameSetup.exe`, or `SecuLauncher.exe`, now would be the time to revert them to their original names. If you have a Steam copy, this can be done with the "Verify integrity of game files..." button in the "Local Files" tab of FUEL's Steam "Properties..." menu.
  - This requirement will be removed in future releases once fingerprinting is implemented.
2. Download the latest release from the [releases page](https://github.com/widberg/fmtk/releases)
3. Unzip the release into the root FUEL directory (the directory containing `FUEL.exe`)
4. Verify that `fmtk.exe`, `fmtkdll.dll`, `secudll.dll`, `mods`, and your `FUEL.exe` are all in the same directory
5. Run `fmtk.exe`

In the event of a crash or other game state locking event, i.e. "Error: Shaders processing has failed."  or "The Game Disk Could Not be Read", please check the `fmtk.log` file in the root FUEL directory. If you cannot resolve the reported error yourself then please open an issue or contact an FMTK developer directly.

**FMTK mods are only loaded when the fmtk.exe "Blue FUEL" launcher is used to run the game. To run the game without mods enabled, simply launch it how you normally would without FMTK; uninstalling FMTK is unnecessary. You can uninstall FMTK and all FMTK mods at any time by running the included `uninstall_fmtk.bat` script from the root FUEL directory.**

## Discord

Join the [Unofficial FUEL Discord](https://discord.gg/8uJVY9mWZT) for general FUEL discussion and FMTK support.

## Documentation

Documentation for users and mod developers is available on [Read the Docs](https://fmtk.rtfd.io). Documentation for FMTK developers is available on the [FMTK Wiki](https://github.com/widberg/fmtk/wiki).

## Related Projects

These are projects that are related to researching FUEL but not directly part of the modding process.

* [dpc](https://github.com/widberg/dpc) - Tar analogous utility to work with the DPC file format
* [fuel-brute](https://github.com/widberg/fuel-brute) - CRC32 Brute Force Solver For FUEL
* [fuel-map](https://github.com/widberg/fuel-map) - It's just information about the map

## Getting Started (For FMTK Developers)

### Prerequisites

* CMake
* A PC copy of FUEL (Optional)

### Checkout

```sh
git clone https://github.com/widberg/fmtk.git
cd fmtk
git submodule update --init --recursive
```

### Build

```sh
mkdir build
cd build
cmake -A Win32 ..
cmake --build .
```
