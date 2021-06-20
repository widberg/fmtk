# FMTK

![FMTK Logo](https://github.com/widberg/fmtk/blob/master/logo.png?raw=true)

FUEL Modding Toolkit

[![Build Status](https://github.com/widberg/fmtk/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/widberg/fmtk/actions/workflows/build.yml)
[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/widberg/fmtk)](https://github.com/widberg/fmtk/releases)
[![Join the chat at https://discord.gg/8uJVY9mWZT](https://img.shields.io/badge/chat-on_discord-7389D8.svg?logo=discord&logoColor=ffffff&labelColor=6A7EC2)](https://discord.gg/8uJVY9mWZT)

## Features

* Fully compatible with "FUEL: REFUELED" by default
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

## Installing (For Users and Mod Developers)

1. Download the latest release from the [releases page](https://github.com/widberg/fmtk/releases)
2. Unzip the release into the root FUEL directory
3. Verify that `fmtk.exe`, `fmtkdll.dll`, `secudll.dll`, `mods`, and `FUEL.exe` are all in the same directory
4. Run `fmtk.exe`

## Discord

Join the [Unofficial FUEL Discord](https://discord.gg/8uJVY9mWZT) for general FUEL discussion and FMTK support.

## Documentation

Documentation for mod developers and FMTK developers is available on the [FMTK Wiki](https://github.com/widberg/fmtk/wiki).

## Getting Started (For FMTK Developers)

### Prerequisites

* CMake
* DirectX 9 SDK
* A PC copy of FUEL (Optional)

### Manual Setup

#### Checkout

```sh
git clone https://github.com/widberg/fmtk.git
cd fmtk
git submodule update --init --recursive
```

#### Build

```sh
mkdir build
cd build
cmake -A Win32 ..
cmake --build .
```

### Bootstrap (Recommended)

```sh
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://raw.githubusercontent.com/widberg/fmtk/master/bootstrap.ps1'))
```
