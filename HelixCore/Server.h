#pragma once

#include "Noncopyable.h"
#include "ServerConfig.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace Helix::Core {
	class Server : public Util::Noncopyable
	{
	public:
		explicit Server(ServerConfig config)
			: _config(std::move(config))
		{
			InitializeAddress();
		}

		virtual ~Server() noexcept = default;

		virtual void Run() = 0;

		virtual void Stop() = 0;

		[[nodiscard]] const ServerConfig& GetConfig() const noexcept { return _config; }

		[[nodiscard]] const ::sockaddr_storage& GetAddress() const noexcept { return _addr; }

	protected:
		ServerConfig _config;

		::sockaddr_storage _addr{};

	private:
		void InitializeAddress()
		{
			auto addr_in = reinterpret_cast<::sockaddr_in*>(&_addr);
			addr_in->sin_family = AF_INET;
			addr_in->sin_port = ::htons(_config.port);

			::inet_pton(AF_INET, _config.ip.c_str(), &addr_in->sin_addr);
		}
	};
}
