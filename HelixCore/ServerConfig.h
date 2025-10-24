#pragma once

#include <string>
#include "Types.h"
#include "ILoadable.h"

namespace Helix::Util {
	class INIData;
}

namespace Helix::Core {
	struct ServerConfig : public Util::ILoadable<Util::INIData>
	{
		ServerConfig() = default;

		[[nodiscard]] bool LoadFrom(const Util::INIData& source) override;

		[[nodiscard]] uint32 GetIOThreadCount() const noexcept;
		[[nodiscard]] uint32 GetWorkerThreadCount() const noexcept;

		std::string ip = "0.0.0.0";
		uint16 port = 5000;
		uint16 grpcPort = 50051;

		uint32 ioThreadCount = 0;
		uint32 workerThreadCount = 0;

		uint32 maxConnections = 10000;
		uint32 backlogSize = 128;

		uint32 connectTimeoutSec = 30;
		uint32 readTimeoutSec = 60;
		uint32 writeTimeoutSec = 60;

		uint32 receiveBufferSize = 8192;
		uint32 sendBufferSize = 8192;
	};
}
