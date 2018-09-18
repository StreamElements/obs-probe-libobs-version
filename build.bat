call prep.bat

if exist build32 (
    cd build32
    msbuild /t:Clean,Build /p:Configuration=Release obs-probe-libobs-version.sln
    if exist Release\obs-probe-libobs-version.exe copy Release\obs-probe-libobs-version.exe ..\obs-probe-libobs-version-32.exe
    cd ..
)

if exist build64 (
    cd build64
    msbuild /t:Clean,Build /p:Configuration=Release obs-probe-libobs-version.sln
    if exist Release\obs-probe-libobs-version.exe copy Release\obs-probe-libobs-version.exe ..\obs-probe-libobs-version-64.exe
    cd ..
)
