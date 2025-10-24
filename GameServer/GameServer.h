#pragma once

// gRPC 헤더를 맨 먼저 include
#include <grpcpp/grpcpp.h>

#include "ServerConfig.h"
#include "ServerControlService.h"
#include "Server.h"
#include <atomic>
#include <memory>
#include <thread>

using namespace Helix;

class GameServer final : public Core::Server
{
public:
	explicit GameServer(Core::ServerConfig config);

	virtual ~GameServer();

	void Run() override;

	void Stop() override;

private:
	void StartGrpcServer();
	void StopGrpcServer();

	std::atomic<bool> _shouldStop{ false };
	std::unique_ptr<Core::ServerControlServiceImpl> _controlService;
	std::unique_ptr<grpc::Server> _grpcServer;
};
