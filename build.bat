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

REM GENERATE PS.EXE
set linkerflags=/OUT:bin\ps.exe
cl.exe %compilerflags% ps\\ps.cpp /link %linkerflags%

REM GENERATE RM.EXE
set linkerflags=/OUT:bin\rm.exe
cl.exe %compilerflags% rm\\rm.cpp /link %linkerflags%

REM GENERATE RMDIR.EXE
set linkerflags=/OUT:bin\rmdir.exe
cl.exe %compilerflags% rmdir\\rmdir.cpp /link %linkerflags%

REM GENERATE CAT.EXE
set linkerflags=/OUT:bin\cat.exe
cl.exe %compilerflags% cat\\cat.cpp /link %linkerflags%

REM DELETE THE LINKER GENERATED FILES
del /S *.ilk
del /S *.pdb
del /S *.obj
