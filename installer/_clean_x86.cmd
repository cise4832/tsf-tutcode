@echo off
setlocal

pushd %~dp0

call _version.cmd

del "%TARGETDIR%\corvusskk-%VERSION%-x86.*"

popd

endlocal
