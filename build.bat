@echo off
pushd %cd%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64     
popd

set compilerflags=/Od /Zi /EHsc
REM GENERATE BASH.EXE
set linkerflags=/OUT:bin\bash.exe
cl.exe %compilerflags% main.cpp /link %linkerflags%

REM GENERATE LS.EXE
set linkerflags=/OUT:bin\ls.exe
cl.exe %compilerflags% ls\\ls.cpp /link %linkerflags%

REM GENERATE MKDIR.EXE
set linkerflags=/OUT:bin\mkdir.exe
cl.exe %compilerflags% mkdir\\mkdir.cpp /link %linkerflags%

REM GENERATE CHMOD.EXE
set linkerflags=/OUT:bin\chmod.exe
cl.exe %compilerflags% chmod\\chmod.cpp /link %linkerflags%

REM DELETE THE LINKER GENERATED FILES
del /S *.ilk
del /S *.pdb
