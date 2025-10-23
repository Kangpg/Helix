#include "pch.h"
#include "ServerConfig.h"
#include "INIData.h"
#include <thread>

namespace Helix::Core {
	bool ServerConfig::LoadFrom(const Util::INIData& source)
	{
		// [Network] section
		ip = source.GetString("Network", "ip", ip);
		port = static_cast<uint16>(source.GetUnsigned("Network", "port", port));

		// [Threads] section
		ioThreadCount = static_cast<uint32>(source.GetUnsigned("Threads", "io_thread_count", ioThreadCount));
		workerThreadCount = static_cast<uint32>(source.GetUnsigned("Threads", "worker_thread_count", workerThreadCount));

		// [Server] section
		maxConnections = static_cast<uint32>(source.GetUnsigned("Server", "max_connections", maxConnections));
		backlogSize = static_cast<uint32>(source.GetUnsigned("Server", "backlog_size", backlogSize));

		// [Timeouts] section
		connectTimeoutSec = static_cast<uint32>(source.GetUnsigned("Timeouts", "connect_timeout_sec", connectTimeoutSec));
		readTimeoutSec = static_cast<uint32>(source.GetUnsigned("Timeouts", "read_timeout_sec", readTimeoutSec));
		writeTimeoutSec = static_cast<uint32>(source.GetUnsigned("Timeouts", "write_timeout_sec", writeTimeoutSec));

		// [Buffers] section
		receiveBufferSize = static_cast<uint32>(source.GetUnsigned("Buffers", "receive_buffer_size", receiveBufferSize));
		sendBufferSize = static_cast<uint32>(source.GetUnsigned("Buffers", "send_buffer_size", sendBufferSize));

		return true;
	}

	uint32 ServerConfig::GetIOThreadCount() const noexcept
	{
		if (ioThreadCount == 0)
		{
			return std::thread::hardware_concurrency();
		}
		return ioThreadCount;
	}

	uint32 ServerConfig::GetWorkerThreadCount() const noexcept
	{
		if (workerThreadCount == 0)
		{
			return std::thread::hardware_concurrency();
		}
		return workerThreadCount;
	}
}
