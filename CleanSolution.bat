for /F "tokens=*" %%G in ('dir /B /AD /S bin') do rmdir /S /Q "%%G"
for /F "tokens=*" %%G in ('dir /B /AD /S obj') do rmdir /S /Q "%%G"
for /F "tokens=*" %%G in ('dir /B /AD /S Debug') do rmdir /S /Q "%%G"
if exist "./bin" (
    rmdir /S /Q "./bin"
)
if exist "./Debug" (
    rmdir /S /Q "./Debug"
)
if exist ".vs" (
    rmdir /S /Q ".vs"
)
if exist "./Packages" (
    RMDIR /S /Q "./Packages"
)
pause