@echo off
cd /d %~dp0
del *.ilk
IF EXIST "source"	(
cd source
)
IF EXIST "debug"	(
cd debug
echo.y | del /A:H *.*
echo.y | del *.*
cd ..
rd debug
)
del *.sdf
del *.user
del *.ncb
del *.htm
del *.pfl
del *.md
cd ..
@echo on