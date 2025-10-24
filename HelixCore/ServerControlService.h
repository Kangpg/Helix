#pragma once

#include <grpcpp/grpcpp.h>
#include "server_control.grpc.pb.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <string>
#include "Types.h"

namespace Helix::Core {

	class ServerControlServiceImpl final : public helix::control::ServerControl::Service
	{
	public:
		explicit ServerControlServiceImpl(
			std::atomic<bool>& shouldStop,
			std::function<void()> stopCallback = nullptr
		)
			: _shouldStop(shouldStop)
			, _stopCallback(std::move(stopCallback))
			, _startTime(std::chrono::steady_clock::now())
		{
		}

		grpc::Status Stop(
			grpc::ServerContext* context,
			const helix::control::StopRequest* request,
			helix::control::StopResponse* response
		) override;

		grpc::Status GetStatus(
			grpc::ServerContext* context,
			const helix::control::StatusRequest* request,
			helix::control::StatusResponse* response
		) override;

		grpc::Status ReloadConfig(
			grpc::ServerContext* context,
			const helix::control::ReloadConfigRequest* request,
			helix::control::ReloadConfigResponse* response
		) override;

		void SetActiveConnections(uint32 count) { _activeConnections = count; }
		void SetServerInfo(const std::string& ip, uint32 port, const std::string& version)
		{
			_serverIp = ip;
			_serverPort = port;
			_serverVersion = version;
		}

	private:
		std::atomic<bool>& _shouldStop;
		std::function<void()> _stopCallback;
		std::chrono::steady_clock::time_point _startTime;

		std::atomic<uint32> _activeConnections{ 0 };
		std::string _serverIp;
		uint32 _serverPort = 0;
		std::string _serverVersion = "0.1.0";
	};

} // namespace Helix::Core
