@echo off

@setlocal

set PHAMLP_PATH=%~dp0

if "%PHP_COMMAND%" == "" set PHP_COMMAND=c:\xampp\php\php.exe

%PHP_COMMAND% "%PHAMLP_PATH%sassc.php" %*

@endlocal