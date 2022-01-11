set exeFile=.\Debug\RayTracer.Tester.exe
set scenesFolder=.\scenes\book
set imagesFolder=.\scenes\images

for %%f in (%scenesFolder%\*.test) do (
 %exeFile% %%f %imagesFolder%\%%~nf.bmp)
