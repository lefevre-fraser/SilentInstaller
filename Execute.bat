if not exist "C:\Temp\Silent" (
    mkdir "C:\Temp\Silent"
)

set outfile="C:\Temp\Silent\ActionLogs.log"

pushd %1

echo %date% %time% >> %outfile%
echo Now Running: %2 >> %outfile%
echo In Directory: %1 >> %outfile%

echo %2 | FindStr /C:".ps1"
set power=%errorlevel%
echo %2 | FindStr /C:".msi"
set msi=%errorlevel%

if %power% EQU 0 (
    echo PowerShell Execution >> %outfile%
    powershell.exe -ExecutionPolicy bypass -File %2 >> %outfile%
) else if %msi% EQU 0 (
    echo MSI Execution >> %outfile%
    msiexec %3 %2 /qn /norestart >> %outfile%
) else (
    echo Regular Execution >> %outfile%
    cmd /c %2 >> %outfile%
)

echo %time% >> %outfile%
echo. >> %outfile%
echo. >> %outfile%
echo. >> %outfile%
echo. >> %outfile%
echo. >> %outfile%

popd