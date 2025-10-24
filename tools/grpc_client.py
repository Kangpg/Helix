#!/usr/bin/env python3
"""
gRPC 클라이언트 예제 - Helix 서버 제어

필요 패키지:
  pip install grpcio grpcio-tools

사용법:
  python grpc_client.py stop "Manual shutdown"
  python grpc_client.py status
  python grpc_client.py reload
"""

import sys
import grpc
from pathlib import Path

# Proto 파일에서 생성된 모듈 import
# 먼저 proto를 컴파일해야 합니다:
# python -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/server_control.proto

try:
    import server_control_pb2
    import server_control_pb2_grpc
except ImportError:
    print("Error: Proto files not compiled!")
    print("Run: python -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/server_control.proto")
    sys.exit(1)


class HelixServerClient:
    def __init__(self, host='localhost', port=50051):
        self.address = f'{host}:{port}'
        self.channel = grpc.insecure_channel(self.address)
        self.stub = server_control_pb2_grpc.ServerControlStub(self.channel)

    def stop(self, reason="", grace_period=0):
        """서버 종료"""
        try:
            request = server_control_pb2.StopRequest(
                reason=reason,
                grace_period_seconds=grace_period
            )
            response = self.stub.Stop(request)
            print(f"✓ Stop command sent")
            print(f"  Success: {response.success}")
            print(f"  Message: {response.message}")
            return response
        except grpc.RpcError as e:
            print(f"✗ RPC failed: {e.code()} - {e.details()}")
            return None

    def get_status(self):
        """서버 상태 조회"""
        try:
            request = server_control_pb2.StatusRequest()
            response = self.stub.GetStatus(request)
            print(f"✓ Server Status:")
            print(f"  Running: {response.is_running}")
            print(f"  Version: {response.server_version}")
            print(f"  Uptime: {response.uptime_seconds}s ({response.uptime_seconds // 3600}h {(response.uptime_seconds % 3600) // 60}m)")
            print(f"  Active Connections: {response.active_connections}")
            print(f"  Address: {response.server_ip}:{response.server_port}")
            return response
        except grpc.RpcError as e:
            print(f"✗ RPC failed: {e.code()} - {e.details()}")
            return None

    def reload_config(self):
        """설정 리로드"""
        try:
            request = server_control_pb2.ReloadConfigRequest()
            response = self.stub.ReloadConfig(request)
            print(f"✓ Reload config command sent")
            print(f"  Success: {response.success}")
            print(f"  Message: {response.message}")
            return response
        except grpc.RpcError as e:
            print(f"✗ RPC failed: {e.code()} - {e.details()}")
            return None

    def close(self):
        """채널 닫기"""
        self.channel.close()


def main():
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python grpc_client.py stop [reason] [grace_period_seconds]")
        print("  python grpc_client.py status")
        print("  python grpc_client.py reload")
        sys.exit(1)

    command = sys.argv[1].lower()
    client = HelixServerClient()

    try:
        if command == 'stop':
            reason = sys.argv[2] if len(sys.argv) > 2 else ""
            grace_period = int(sys.argv[3]) if len(sys.argv) > 3 else 0
            client.stop(reason, grace_period)
        elif command == 'status':
            client.get_status()
        elif command == 'reload':
            client.reload_config()
        else:
            print(f"Unknown command: {command}")
            print("Available commands: stop, status, reload")
    finally:
        client.close()


if __name__ == '__main__':
    main()
