#include "GameServer.h"
#include "Logger.h"
#include <format>

GameServer::GameServer(Core::ServerConfig config)
	: Core::Server(std::move(config))
{
	// Initialize gRPC control service
	_controlService = std::make_unique<Core::ServerControlServiceImpl>(
		_shouldStop,
		[this]() { Stop(); }
	);

	// Set server info for status reporting
	_controlService->SetServerInfo(_config.ip, _config.port, "0.1.0");
}

GameServer::~GameServer()
{
	Stop();
	StopGrpcServer();
}

void GameServer::Run()
{
	LOG_NOTICE("Starting GameServer...");

	// Start gRPC control server
	StartGrpcServer();

	LOG_NOTICE("Server is now running. Use gRPC to control the server.");

	// Main server loop
	while (!_shouldStop.load(std::memory_order_acquire))
	{
		// TODO: Process game logic, network events, etc.

		// Sleep to prevent busy waiting
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	LOG_NOTICE("Server main loop exited");
}

void GameServer::Stop()
{
	LOG_NOTICE("Stopping GameServer...");
	_shouldStop.store(true, std::memory_order_release);
}

void GameServer::StartGrpcServer()
{
	std::string grpcAddress = std::format("0.0.0.0:{}", _config.grpcPort);

	grpc::ServerBuilder builder;
	builder.AddListeningPort(grpcAddress, grpc::InsecureServerCredentials());
	builder.RegisterService(_controlService.get());

	_grpcServer = builder.BuildAndStart();

	if (_grpcServer)
	{
		LOG_NOTICE("gRPC control server listening on " + grpcAddress);
	}
	else
	{
		LOG_WARNING("Failed to start gRPC control server on " + grpcAddress);
	}
}

void GameServer::StopGrpcServer()
{
	if (_grpcServer)
	{
		LOG_NOTICE("Shutting down gRPC control server...");
		_grpcServer->Shutdown();
		_grpcServer.reset();
	}
}
