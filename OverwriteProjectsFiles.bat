@echo off
setlocal

echo This will delete the solution file and ALL *.vcxproj *.vcxproj.filters *.vcxproj.user files within this entire folder.

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
call premake5 vs2022

echo.
echo Done!
pause

endlocal