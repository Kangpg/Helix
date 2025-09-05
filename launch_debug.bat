@echo off
setlocal EnableExtensions

rem ===== 설정 =====
set "CONFIG=Debug"
set "VCPKG_TRIPLET=x64-windows"
set "ROOT=%~dp0"
set "ROOT=%ROOT:~0,-1%"
set "VCPKG_DIR=%ROOT%\vcpkg"
set "TOOLCHAIN=%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake"
set "OPEN_SOLUTION=1"   rem 1이면 .sln 자동 실행, 0이면 안 열기

echo [INFO] ROOT=%ROOT%
echo [INFO] CONFIG=%CONFIG%
echo [INFO] TRIPLET=%VCPKG_TRIPLET%

rem ===== vcpkg 준비(없으면 clone+bootstrap) =====
if not exist "%TOOLCHAIN%" (
  if not exist "%VCPKG_DIR%" (
    git clone --depth 1 https://github.com/microsoft/vcpkg "%VCPKG_DIR%" || goto :ERR
  )
  call "%VCPKG_DIR%\bootstrap-vcpkg.bat" -disableMetrics || goto :ERR
) else (
  echo [OK] vcpkg toolchain found
)

set "BDIR=%ROOT%\build\vs2022-%CONFIG%"

rem ===== CMake Configure/Generate (VS 솔루션 생성만) =====
cmake -S "%ROOT%" -B "%BDIR%" -G "Visual Studio 17 2022" -A x64 ^
  -DVCPKG_MANIFEST_MODE=ON ^
  -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN%" ^
  -DVCPKG_TARGET_TRIPLET=%VCPKG_TRIPLET%
if errorlevel 1 goto :ERR

echo.
echo [INFO] 생성된 솔루션:
dir "%BDIR%\*.sln" /b

if "%OPEN_SOLUTION%"=="1" (
  for %%S in ("%BDIR%\*.sln") do start "" "%%~fS"
)

echo.
echo [DONE] 솔루션만 생성 완료
echo.
pause
exit /b 0

:ERR
echo.
echo [ERROR] CMake configure 실패
echo [HINT] 로그: "%BDIR%\CMakeFiles\CMakeError.log", CMakeOutput.log
echo.
pause
exit /b 1
