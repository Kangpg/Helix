#include "ServerControlService.h"
#include "Logger.h"
#include <string>

namespace Helix::Core {

	grpc::Status ServerControlServiceImpl::Stop(
		grpc::ServerContext* context,
		const helix::control::StopRequest* request,
		helix::control::StopResponse* response
	)
	{
		LOG_NOTICE("Received Stop request via gRPC");

		if (!request->reason().empty())
		{
			LOG_NOTICE("Stop reason: " + request->reason());
		}

		if (request->grace_period_seconds() > 0)
		{
			LOG_NOTICE("Grace period: " + std::to_string(request->grace_period_seconds()) + " seconds");
		}

		// Set the stop flag
		_shouldStop.store(true, std::memory_order_release);

		// Call the stop callback if provided
		if (_stopCallback)
		{
			_stopCallback();
		}

		response->set_success(true);
		response->set_message("Server stop initiated");

		LOG_NOTICE("Server stopping gracefully...");

		return grpc::Status::OK;
	}

	grpc::Status ServerControlServiceImpl::GetStatus(
		grpc::ServerContext* context,
		const helix::control::StatusRequest* request,
		helix::control::StatusResponse* response
	)
	{
		auto now = std::chrono::steady_clock::now();
		auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();

		response->set_is_running(!_shouldStop.load(std::memory_order_acquire));
		response->set_server_version(_serverVersion);
		response->set_uptime_seconds(uptime);
		response->set_active_connections(_activeConnections.load(std::memory_order_acquire));
		response->set_server_ip(_serverIp);
		response->set_server_port(_serverPort);

		return grpc::Status::OK;
	}

	grpc::Status ServerControlServiceImpl::ReloadConfig(
		grpc::ServerContext* context,
		const helix::control::ReloadConfigRequest* request,
		helix::control::ReloadConfigResponse* response
	)
	{
		LOG_NOTICE("Received ReloadConfig request via gRPC");

		// TODO: Implement config reload logic
		response->set_success(false);
		response->set_message("Config reload not yet implemented");

		return grpc::Status::OK;
	}

} // namespace Helix::Core
