#pragma once

#include "Server.h"
#include "ServerConfig.h"

using namespace Helix;
class GameServer final : public Core::Server
{
public:
	explicit GameServer(Core::ServerConfig config)
		: Core::Server(std::move(config))
	{

	}

	virtual ~GameServer() = default;

	void Run() override {}

	void Stop() override {}
};
