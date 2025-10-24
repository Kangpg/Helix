@echo off
REM Proto 파일 컴파일 스크립트

echo Compiling proto files for Python...

cd /d %~dp0

python -m grpc_tools.protoc ^
  -I../protos ^
  --python_out=. ^
  --grpc_python_out=. ^
  ../protos/server_control.proto

if %ERRORLEVEL% EQU 0 (
    echo Proto files compiled successfully!
    echo Generated files:
    echo   - server_control_pb2.py
    echo   - server_control_pb2_grpc.py
) else (
    echo Failed to compile proto files.
    echo Make sure you have grpcio-tools installed:
    echo   pip install grpcio grpcio-tools
)

pause
