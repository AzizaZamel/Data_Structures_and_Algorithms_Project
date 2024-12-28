@echo off
SETLOCAL EnableDelayedExpansion

:: ===========================
:: Compiler and Flags
:: ===========================
set CC=g++
set CFLAGS=-Iinclude -Wall -Wextra -std=c++11

:: Debug and Release Flags
set DEBUG_FLAGS=-g -DDEBUG
set RELEASE_FLAGS=-O2 -DNDEBUG

:: ===========================
:: Directories
:: ===========================
set SRCDIR=src
set INCDIR=include
set OBJDIR=obj
set BINDIR=bin
set LIBDIR=lib
set TESTDIR=tests

:: ===========================
:: Targets
:: ===========================
set TARGET=%BINDIR%\xml_checker.exe
set STATIC_LIB=%LIBDIR%\libxmlchecker.a
set DYNAMIC_LIB=%LIBDIR%\libxmlchecker.dll
set TEST_TARGET=%BINDIR%\test_xml_checker.exe

:: ===========================
:: Build Type
:: ===========================
:: Default build type is Release
if "%BUILD_TYPE%"=="" set BUILD_TYPE=release

if /I "%BUILD_TYPE%"=="debug" (
    set BUILD_FLAGS=%DEBUG_FLAGS%
    set LIB_BUILD_TYPE=debug
) else if /I "%BUILD_TYPE%"=="release" (
    set BUILD_FLAGS=%RELEASE_FLAGS%
    set LIB_BUILD_TYPE=release
) else (
    echo Invalid BUILD_TYPE. Use "debug" or "release".
    exit /b 1
)

:: ===========================
:: Function: Compile Source Files
:: ===========================
:compile_sources
echo Compiling source files...
if not exist %OBJDIR% mkdir %OBJDIR%
for %%f in (%SRCDIR%\*.cpp) do (
    if /I not "%%~nxf"=="main.cpp" (
        %CC% %CFLAGS% %BUILD_FLAGS% -c "%%f" -o "%OBJDIR%\%%~nf.o"
        if errorlevel 1 (
            echo Failed to compile %%f
            exit /b 1
        )
        echo Compiled: %%f -> %OBJDIR%\%%~nf.o
    )
)
goto :eof

:: ===========================
:: Function: Compile Main Executable
:: ===========================
:compile_main
echo Compiling main executable...
%CC% %CFLAGS% %BUILD_FLAGS% -c "%SRCDIR%\main.cpp" -o "%OBJDIR%\main.o"
if errorlevel 1 (
    echo Failed to compile main.cpp
    exit /b 1
)
echo Compiled: main.cpp -> %OBJDIR%\main.o
goto :eof

:: ===========================
:: Function: Link Executable
:: ===========================
:link_executable
echo Linking executable...
if not exist %BINDIR% mkdir %BINDIR%
%CC% %CFLAGS% %BUILD_FLAGS% %OBJDIR%\*.o -o %TARGET%
if errorlevel 1 (
    echo Failed to link executable
    exit /b 1
)
echo Built executable: %TARGET%
goto :eof

:: ===========================
:: Function: Build Static Library
:: ===========================
:build_static
echo Building static library...
call :compile_sources
if not exist %LIBDIR% mkdir %LIBDIR%
ar rcs %STATIC_LIB% %OBJDIR%\*.o
if errorlevel 1 (
    echo Failed to build static library
    exit /b 1
)
echo Built static library: %STATIC_LIB%
goto :eof

:: ===========================
:: Function: Build Dynamic Library
:: ===========================
:build_dynamic
echo Building dynamic library...
call :compile_sources
if not exist %LIBDIR% mkdir %LIBDIR%
%CC% -shared -fPIC %CFLAGS% %BUILD_FLAGS% -o %DYNAMIC_LIB% %OBJDIR%\*.o
if errorlevel 1 (
    echo Failed to build dynamic library
    exit /b 1
)
echo Built dynamic library: %DYNAMIC_LIB%
goto :eof

:: ===========================
:: Function: Compile Test Files
:: ===========================
:compile_tests
echo Compiling test files...
if not exist %OBJDIR% mkdir %OBJDIR%
for %%f in (%TESTDIR%\*.cpp) do (
    %CC% %CFLAGS% %BUILD_FLAGS% -c "%%f" -o "%OBJDIR%\%%~nf.o"
    if errorlevel 1 (
        echo Failed to compile %%f
        exit /b 1
    )
    echo Compiled test: %%f -> %OBJDIR%\%%~nf.o
)
goto :eof

:: ===========================
:: Function: Link Test Executable
:: ===========================
:link_test
echo Linking test executable...
if not exist %BINDIR% mkdir %BINDIR%
%CC% %CFLAGS% %BUILD_FLAGS% %OBJDIR%\*.o -o %TEST_TARGET%
if errorlevel 1 (
    echo Failed to link test executable
    exit /b 1
)
echo Built test executable: %TEST_TARGET%
goto :eof

:: ===========================
:: Function: Run Tests
:: ===========================
:run_tests
echo Running tests...
%TEST_TARGET%
if errorlevel 1 (
    echo Tests failed.
    exit /b 1
)
echo All tests completed.
goto :eof

:: ===========================
:: Function: Clean Build Artifacts
:: ===========================
:clean
echo Cleaning build artifacts...
if exist %OBJDIR% rd /s /q %OBJDIR%
if exist %BINDIR% rd /s /q %BINDIR%
if exist %LIBDIR% rd /s /q %LIBDIR%
echo Cleaned build artifacts.
goto :eof

:: ===========================
:: Function: Show Help
:: ===========================
:help
echo ==========================================
echo           XML Consistency Checker         
echo ==========================================
echo Available Targets:
echo   all               - Build the executable.
echo   static            - Build the static library.
echo   dynamic           - Build the dynamic library.
echo   test              - Build the test executable.
echo   run_tests         - Build and run tests.
echo   clean             - Remove all build artifacts.
echo   help              - Show this help message.
echo.
echo Build Types:
echo   release           - Optimize the build (default).
echo   debug             - Include debug information.
echo.
echo Usage Examples:
echo   build.bat all
echo   build.bat all BUILD_TYPE=debug
echo   build.bat static
echo   build.bat static BUILD_TYPE=debug
echo   build.bat dynamic
echo   build.bat dynamic BUILD_TYPE=debug
echo   build.bat test
echo   build.bat run_tests
echo   build.bat clean
echo   build.bat help
echo ==========================================
goto :eof

:: ===========================
:: Main Script Logic
:: ===========================
if "%1"=="" (
    set TARGET=all
) else (
    set TARGET=%1
)

:: Handle BUILD_TYPE if passed as second argument
if not "%2"=="" (
    for %%A in (%*) do (
        if /I "%%~A"=="debug" (
            set BUILD_TYPE=debug
        ) else if /I "%%~A"=="release" (
            set BUILD_TYPE=release
        )
    )
)

echo Building target: %TARGET% with BUILD_TYPE=%BUILD_TYPE%

if /I "%TARGET%"=="all" (
    call :compile_sources
    call :compile_main
    call :link_executable
) else if /I "%TARGET%"=="static" (
    call :build_static
) else if /I "%TARGET%"=="dynamic" (
    call :build_dynamic
) else if /I "%TARGET%"=="test" (
    call :compile_sources
    call :compile_tests
    call :link_test
) else if /I "%TARGET%"=="run_tests" (
    call :test
) else if /I "%TARGET%"=="clean" (
    call :clean
) else if /I "%TARGET%"=="help" (
    call :help
) else (
    echo Unknown target: %TARGET%
    echo Use "help" to see available targets.
    exit /b 1
)

ENDLOCAL
exit /b 0
