#pragma once

#include "Noncopyable.h"

namespace Helix::Core::Base {
	class Server : public Util::Noncopyable
	{
	public:
		Server(::sockaddr_storage addr)
			: _addr(addr)
		{
			Network::Network::Initialize();
		}
		virtual ~Server() noexcept
		{
			Network::Network::Finalize();
		}

		virtual void LoadConfig() abstract;

		virtual void Run() abstract;

		virtual void Stop() abstract;

	protected:
		::sockaddr_storage _addr;
	};
}
