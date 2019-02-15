@echo off

mkdir ..\Build
pushd ..\Build

REM if cl compiler is not a defined var, call vcvars setup and hope it is in the same folder for everyone
IF NOT DEFINED cl call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars32"

cl -DHANDMADE_WIN32=1 -FC -Zi ..\Code\Win32_Renderer.cpp ..\Code\Model.cpp ..\Code\Std.cpp ..\Code\MathLib.cpp  user32.lib gdi32.lib
popd
