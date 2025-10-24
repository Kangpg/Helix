# Helix gRPC 클라이언트 도구

## 설치

Python gRPC 도구 설치:
```bash
pip install grpcio grpcio-tools
```

## Proto 파일 컴파일

Windows:
```bash
compile_proto.bat
```

Linux/Mac:
```bash
python -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/server_control.proto
```

## 사용법

### 서버 상태 조회
```bash
python grpc_client.py status
```

### 서버 종료
```bash
python grpc_client.py stop "Manual shutdown" 5
```
- 첫 번째 인자: 종료 사유 (옵션)
- 두 번째 인자: grace period (초) (옵션)

### 설정 리로드
```bash
python grpc_client.py reload
```

## 예제 출력

```
$ python grpc_client.py status
✓ Server Status:
  Running: True
  Version: 0.1.0
  Uptime: 3661s (1h 1m)
  Active Connections: 42
  Address: 0.0.0.0:5000
```
