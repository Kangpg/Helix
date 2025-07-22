#pragma once

#include "Server.h"

using namespace Helix;
class GameServer final : public Core::Base::Server
{
public:
	GameServer(::sockaddr_storage addr) 
		: Core::Base::Server(addr)
	{

	}

	~GameServer() = default;

	void LoadConfig() override {}

	void Run() override {}
};