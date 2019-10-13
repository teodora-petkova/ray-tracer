FOR /F "tokens=*" %%G IN ('DIR /B /AD /S Debug') DO RMDIR /S /Q "%%G"
if exist "./Debug" (
    RMDIR /S /Q "./Debug"
)
if exist "./Packages" (
    RMDIR /S /Q "./Packages"
)

pause