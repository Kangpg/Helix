#include "pch.h"
#include "ServerConfig.h"
#include "INIData.h"
#include <thread>

namespace Helix::Core {
	bool ServerConfig::LoadFrom(const Util::INIData& source)
	{
		// Network
		ip = source.GetString("Network", "ip", "0.0.0.0");
		port = static_cast<uint16>(source.GetUInt("Network", "port", 5000));
		grpcPort = static_cast<uint16>(source.GetUInt("Network", "grpc_port", 50051));

		// Threads
		ioThreadCount = source.GetUInt("Threads", "io_thread_count", std::thread::hardware_concurrency());
		workerThreadCount = source.GetUInt("Threads", "worker_thread_count", std::thread::hardware_concurrency());

		// Server
		maxConnections = source.GetUInt("Server", "max_connections", 1000);
		backlogSize = source.GetUInt("Server", "backlog_size", 10);

		// Timeouts
		connectTimeoutSec = source.GetUInt("Timeouts", "connect_timeout_sec", 30);
		readTimeoutSec = source.GetUInt("Timeouts", "read_timeout_sec", 30);
		writeTimeoutSec = source.GetUInt("Timeouts", "write_timeout_sec", 30);

		// Buffers
		receiveBufferSize = source.GetUInt("Buffers", "receive_buffer_size", 1024);
		sendBufferSize = source.GetUInt("Buffers", "send_buffer_size", 1024);

		return true;
	}

	uint32 ServerConfig::GetIOThreadCount() const noexcept
	{
		return ioThreadCount;
	}

	uint32 ServerConfig::GetWorkerThreadCount() const noexcept
	{
		return workerThreadCount;
	}
}
