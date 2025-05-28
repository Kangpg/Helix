#pragma once

#include "Network.h"
#include "Noncopable.h"

namespace Helix::Core::Base {
	class Server : public Util::Noncopable
	{
	public:
		Server(::sockaddr_storage addr)
			: _addr(addr)
		{
			Network::Network::Initialize();
		}
		virtual ~Server() 
		{
			Network::Network::Initialize();
		}

		virtual void LoadConfig() = 0;

		virtual void Run() = 0;

	protected:
		::sockaddr_storage _addr;
	};
}
