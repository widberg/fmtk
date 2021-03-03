@echo off

SET AREYOUSURE=N
SET /P AREYOUSURE=Would you like to download the DirectX 9 SDK (June 2008) (Y/[N])? 
IF /I "%AREYOUSURE%" NEQ "Y" GOTO NO

:YES

explorer "https://archive.org/details/dxsdk_jun08"
explorer "https://archive.org/download/dxsdk_jun08/DXSDK_Jun08.exe"

:NO

git clone https://github.com/widberg/fmtk.git
cd fmtk
git submodule update --init --recursive
mkdir build
cd build

IF /I "%AREYOUSURE%" NEQ "Y" GOTO SKIP
echo Continue once you have the DirectX 9 SDK installed.
pause
:SKIP

cmake .. -A Win32
cmake --build .

cd ..\..
