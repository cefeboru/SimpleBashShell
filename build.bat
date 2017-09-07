@echo off
pushd %cd%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64     
popd
set compilerflags=/Od /Zi /EHsc
set linkerflags=/OUT:bin\bash.exe
cl.exe %compilerflags% main.cpp /link %linkerflags%
cd %mypath:~0,-1%