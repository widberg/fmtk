$confirmation = Read-Host "Would you like to download the DirectX 9 SDK (June 2008) (Y/[N])? "
if ($confirmation -eq 'y') {
	Start-Process "https://archive.org/details/dxsdk_jun08"
	Start-Process "https://archive.org/download/dxsdk_jun08/DXSDK_Jun08.exe"
}

Invoke-Expression "git clone https://github.com/widberg/fmtk.git"
Push-Location fmtk
Invoke-Expression "git submodule update --init --recursive"
New-Item -ItemType directory -Force -Path build | Out-Null
Push-Location build

if ($confirmation -eq 'y') {
	Read-Host "Press Enter to continue once you have the DirectX 9 SDK installed"
}

Invoke-Expression "cmake .. -A Win32"
Invoke-Expression "cmake --build ."

Pop-Location
Pop-Location
