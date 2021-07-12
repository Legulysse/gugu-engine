@echo off

cd "Build"
"Premake/Premake5.exe" --file="BuildAll.lua" vs2019

pause
