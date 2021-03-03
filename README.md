# FMTK

FUEL Modding Toolkit

## Getting Started

### Prerequisites

* CMake
* A Steam copy of FUEL

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
powershell -command "Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://raw.githubusercontent.com/widberg/fmtk/master/bootstrap.bat'))"
```
