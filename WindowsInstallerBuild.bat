@ECHO OFF
ECHO Checking prerequisites
IF "%WIX%"=="" GOTO NOWIX
echo %path% | FINDSTR /c:"MinGW" >nul
IF ERRORLEVEL 1 GOTO NOMINGW
IF NOT EXIST "Makefile" GOTO NOMAKEFILE
for %%x in (
			SDL2.dll
			RESOURCE.001
			RESOURCE.MAP
			sound0.wav
			sound1.wav
			sound3.wav
			sound4.wav
			sound5.wav
			sound6.wav
			sound7.wav
			sound8.wav
			sound9.wav
			sound10.wav
			sound12.wav
			sound14.wav
			sound15.wav
			sound16.wav
			sound17.wav
			sound18.wav
			sound19.wav
			sound2.wav
			sound20.wav
			sound21.wav
			sound22.wav
			sound23.wav
			sound24.wav
		   ) do (
			SET filename=%%x
			IF NOT EXIST %%x GOTO FILEMISSING
           )

ECHO building
REM Run ordinary make
make
IF ERRORLEVEL 1 GOTO MAKEERROR
IF EXIST "jc_reborn.scr" DEL jc_reborn.scr
RENAME jc_reborn.exe jc_reborn.scr

ECHO Creating MSI
"%WIX%\bin\candle" MSWindows\*.wxs -o MSWindows\obj\
IF ERRORLEVEL 1 GOTO CANDLEERROR
"%WIX%\bin\light" MSWindows\obj\*.wixobj -o jc_reborn.msi
IF ERRORLEVEL 1 GOTO LIGHTERROR
echo Done, you can now install from jc_reborn.msi
GOTO :EOF

:NOWIX
ECHO ERROR: Could not find WiX Toolset
GOTO :EOF

:NOMINGW
ECHO ERROR: Could not find MinGW, mare sure it's in the PATH
GOTO :EOF

:NOMAKEFILE
ECHO ERROR: Please rename Makefile.MinGW to MakeFile
GOTO :EOF

:MAKEERROR
ECHO ERROR: Build failed
GOTO :EOF

:FILEMISSING
ECHO ERROR: %filename% not found

:CANDLEERROR
ECHO ERROR: compiling WiX installer WXS file

:LIGHTERROR
ECHO ERROR: assembling MSI
