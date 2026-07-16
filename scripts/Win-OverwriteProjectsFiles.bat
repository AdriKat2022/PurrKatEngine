@echo off
setlocal

pushd ..\

echo ! WARNING !
echo This will delete within the whole solution:
echo - The solution file
echo - ALL *.vcxproj files
echo - ALL *.vcxproj.user files
echo - ALL *.vcxproj.filters files
echo.
echo Only then to run premake to re-generate project files.
echo.
echo Press any key to proceed.
echo Close this window to cancel.

pause

echo Cleaning Visual Studio generated files...

REM Delete solution file
del /Q *.sln

REM Delete project files recursively
for /R %%f in (*.vcxproj *.vcxproj.filters *.vcxproj.user) do (
    del "%%f"
)

REM Optional: delete .vs folder (Visual Studio cache)
if exist .vs (
    rmdir /S /Q .vs
)

echo.
echo Regenerating project files with Premake...

REM Run premake (adjust if needed)
call premake5 vs2026

echo.
echo Done!

popd

pause

endlocal