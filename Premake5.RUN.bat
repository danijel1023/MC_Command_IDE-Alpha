del /Q *.sln
for /f "delims=" %%i in ('dir /a:d /s /b') do del /Q "%%i\*.vcxproj"
for /f "delims=" %%i in ('dir /a:d /s /b') do del /Q "%%i\*.filters"
for /f "delims=" %%i in ('dir /a:d /s /b') do del /Q "%%i\*.user"

vendor\premake\premake5.exe vs2019
pause
