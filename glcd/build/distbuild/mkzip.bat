@echo off
REM ##################################################################
REM # Windows batch file to create GLCD distribution zip image.
REM #
REM # This batch file has 2 modes
REM # 
REM # 1) If called with no argument, use SVN to checkout a "working"
REM #
REM # 2) If called an argument, use that directory as the source
REM #    to create a "working" directory.
REM #    The directory indicated will be copied to the working directory.
REM #    This method can be used with Windows Drag-n-Drop.
REM #    Simply drag a development SVN glcd directory on top of
REM #    of the batch file and it will create a zip file based on that
REM #    glcd tree rather than grabbing a new from the head of SVN
REM #
REM #
REM # Needed Tools:
REM # -------------
REM # you must have a commandline SVN tool:
REM #	http://www.open.collab.net/downloads/subversion/
REM #
REM # you must have 7zip installed.
REM #	http://www.7-zip.org/
REM # 	Make sure to manually add the install dir to your path as the
REM #	default install does not do this.
REM # 	normally it is: $SystemDrive:\Program Files\7-Zip
REM #
REM # you must have doxygen/doxywizard installed.
REM #	http://www.doxygen.nl/download.html#latestsrc
REM #	(Make sure to have version 1.6.3 or later)
REM #
REM ##################################################################

REM #
REM # grab short name of batch file: basename.extension
REM

set PROGNAME=%~n0.%~x0

REM #
REM # set PWD variable (grab it from path of batch file)
REM #

set PWD=%~dp0

REM #
REM # LOG file name (create based on baename of batch file)
REM #

set LOGFILE=%PWD%\%~n0.log

echo CURRENT WORKING directory: %PWD%

REM #
REM # Setup some date variables
REM #
REM # WARNING:
REM # you would think that getting date would be a simply
REM # operation. But this is the world of Windoze.
REM # since date format can be customized by the user there is no
REM # no way to detect the format.
REM #
REM # This is screwey because windows is so !@#$!# up that
REM # you can't even get a date in a consistent format to parse out
REM # the fields. (it is all localized)
REM # So we go look for a real date command in Arduino binaries
REM # and then the WINAVR binaries.
REM # if none found, then no date variables.
REM #


if not defined ARDUINO-0017_BINDIR set ARDUINO-0017_BINDIR=..\..\..\..\tools\avr\utils\bin
if not defined ARDUINO-0018_BINDIR set ARDUINO-0018_BINDIR=..\..\..\..\hardware\tools\avr\utils\bin
set WINAVR_BINDIR1=%systemDrive%\WinAVR-20090313\utils\bin
set WINAVR_BINDIR2=%systemDrive%\WinVAR-20100110\utils\bin

echo looking for unix tools

for %%G in (%ARDUINO-0017_BINDIR% %ARDUINO-0018_BINDIR% %WINAVR_BINDIR1% %WINAVR_BINDIR2%) do (
	if exist %%G (
		echo found unix tool dir: %%G
		set UNIX_TOOLDIR=%%G
		goto endunixsearch
	)
)
echo no unix tools found
:endunixsearch

REM #
REM # now (finally) load up MYDATE and MYTIME variables with date & time
REM #
if exist %UNIX_TOOLDIR%\date.exe (
	for /F %%G IN ('%UNIX_TOOLDIR%\date.exe +%%Y%%m%%d') do set MYDATE=%%G
	for /F %%G IN ('%UNIX_TOOLDIR%\date.exe +%%H%%M%%S') do set MYTIME=%%G
) else (
	echo no unix date command found so no date/time timestamp in zipfile name
)

:definevars

REM ##################################################################
REM #
REM # Define working variables
REM #
REM ##################################################################

REM #
REM # name of library
REM # This name will be used for the name of the distribution
REM # directory inside the zip file.
REM # it is the name of the directory that will be in the
REM # arduinoXXX/hardware/libraries
REM #

set GLCDLIBNAME=glcd

REM #
REM # location of GLCD lib SVN repository
REM #

set GLCDREPO=http://arduino-glcd.googlecode.com/svn/trunk/glcd

REM #
REM # SVN commands
REM #
set SVNCMD=svn checkout
set SVNINFO=svn info
set SVNLIST=svn list -vR

REM #
REM # zip command
REM #
set ZIPCMD=7z a


REM #
REM # Name of working directory for distibution tree
REM #  note/warning the path needs to be full vs relative
REM #  as svn has special code to deal with the @#@$@! 
REM #  windows long path issue but it only kicks in on absolute paths.
REM #
set GLCDDISTDIR=%TEMP%\%GLCDLIBNAME%


REM #
REM # Doxygen command and config file stuff
REM #  note: doxygen must be started in the directory
REM #  where the config file is since all paths in the config file
REM #  are relative to that point.
REM #

set DOXYGEN=doxygen
set DOXYGENDIR=%GLCDDISTDIR%\build\doc\doxygen
set DOXYGENCFG=Doxyfile.doxcfg

REM #
REM # Name of ZIP file
REM # has date as part of file name
REM #

if defined MYDATE (
	set GLCDZIPNAME=%GLCDLIBNAME%-%MYDATE%.zip
) else (
	set GLCDZIPNAME=%GLCDLIBNAME%.zip
)

REM #
REM # name of manifest file
REM #
set GLCDMANIFEST=%GLCDDISTDIR%\manifest.txt

REM ##################################################################
REM # Now start to actually do something

echo Starting build %DATE% %TIME% >"%LOGFILE%"

REM # check to see if there is an argument from drag-n-drop
REM # if no argument passed in, then get files from SVN

if [%1]==[] goto get_svnfiles

echo Drag and drop mode for file %1

echo Copying existing devlopment tree
REM DO NOT TURN THIS ON
REM echo copy %1 "%GLCDDISTDIR%"
echo DRAG and DROP not yet supported
pause
goto done
goto working_tree_ready


:get_svnfiles
echo Checking out SVN working tree: %GLCDDISTDIR%
echo ======== Checkout SVN tree to %GLCDDISTDIR% >> "%LOGFILE%"
%SVNCMD% %GLCDREPO% "%GLCDDISTDIR%" >>"%LOGFILE%"

if not ERRORLEVEL 1 goto working_tree_ready
echo Fatal Error: could not checkout SVN tree
goto done

:working_tree_ready
echo Working tree is ready for processing
pause

echo Creating SVN Manifest file
echo Distribution files created %DATE% %TIME% >"%GLCDMANIFEST%"
echo =====================================================================>>"%GLCDMANIFEST%"
cd "%GLCDDISTDIR%"
%SVNINFO% >>"%GLCDMANIFEST%"
echo =====================================================================>>"%GLCDMANIFEST%"
%SVNLIST% >>"%GLCDMANIFEST%"
cd %PWD%

echo Removing SVN control files 
cd ""%GLCDDISTDIR%""
for /D /r %%G in (.svn) DO rmdir /S /Q %%G 
cd %PWD%

REM # Must build doxygen docs before build directory is removed
echo Building Doxygen Documents
echo ======== Building Doxygen Documents >> "%LOGFILE%"
cd %DOXYGENDIR%
%DOXYGEN% %DOXYGENCFG%  >> "%LOGFILE%" 2>&1
cd %PWD%

echo Removing debug and build directory from SVN working tree
echo ======== Removing debug and build directories from %GLCDDISTDIR% >> "%LOGFILE%"
cd "%GLCDDISTDIR%"
rmdir /S /Q debug
rmdir /S /Q build
cd %PWD%


echo Creating Zip file
echo ======== Creating Zip file from %GLCDDISTDIR%>> "%LOGFILE%"
if exist "%GLCDZIPNAME%" erase /F "%GLCDZIPNAME%"
echo About to create zip file from %GLCDDISTDIR%
pause
%ZIPCMD% %GLCDZIPNAME% "%GLCDDISTDIR%" >> "%LOGFILE%"

echo Zip file %GLCDZIPNAME% created
echo ======== Zip file %GLCDZIPNAME% created >> "%LOGFILE%"

echo Removing SVN working tree %GLCDDISTDIR%
echo About to remove SVN working tree %GLCDDISTDIR%
pause
rmdir /S /Q "%GLCDDISTDIR%"
echo ======== Removed SVN working tree %GLCDDISTDIR% >> "%LOGFILE%"

echo ======== %PROGNAME% completed normally >> "%LOGFILE%"
:done
echo %PROGNAME% Finished
pause
