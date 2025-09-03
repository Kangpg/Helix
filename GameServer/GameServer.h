#pragma once

#include "Server.h"

using namespace Helix;
class GameServer final : public Core::Base::Server
{
public:
	explicit GameServer(::sockaddr_storage addr) 
		: Core::Base::Server(addr)
	{

	}

	virtual ~GameServer() = default;

	void LoadConfig() override {}

	void Run() override {}
};