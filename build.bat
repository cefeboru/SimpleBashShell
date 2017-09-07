@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64     
SET mypath=%~dp0
echo %mypath:~0,-1%
set compilerflags=/Od /Zi /EHsc
set linkerflags=/OUT:%mypath:~0,-1%\bin\bash.exe
cl.exe %compilerflags% %mypath:~0,-1%\main.cpp /link %linkerflags%
cd %mypath:~0,-1%