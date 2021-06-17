# FMTK

![FMTK Logo](https://github.com/widberg/fmtk/blob/master/logo.png?raw=true)

FUEL Modding Toolkit

![Build Status](https://github.com/widberg/fmtk/actions/workflows/build.yml/badge.svg?branch=master)
![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/widberg/fmtk)

## Installing

1. Download the latest release from the [releases page](https://github.com/widberg/fmtk/releases)
2. Unzip the release into the root FUEL directory
3. Verify that `fmtk.exe`, `fmtkdll.dll`, `secudll.dll`, and `FUEL.exe` are all in the same directory
4. Run `fmtk.exe`

## Getting Started

### Prerequisites

* CMake
* DirectX 9 SDK
* A Steam copy of FUEL (Optional)

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
cmake .. -A Win32
cmake --build .
```

### Bootstrap

```sh
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://raw.githubusercontent.com/widberg/fmtk/master/bootstrap.ps1'))
```
