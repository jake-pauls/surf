@ECHO OFF

:: GO!
:: Execute this in the root directory

IF NOT EXIST "build" (
    ECHO GO! Creating build directory
    mkdir build
)

ECHO GO! Creating the Visual Studio solution
PUSHD build
CALL cmake ..
POPD

ECHO GO! Open the Visual Studio solution with
ECHO     ./build/surf.sln
ECHO GO! Surf's up!