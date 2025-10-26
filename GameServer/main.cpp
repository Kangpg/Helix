#include <iostream>
#include <filesystem>
#include <cassert>

// gRPC�� �����ϴ� GameServer.h�� �� ���� include
#include "GameServer.h"

// �� ���� �ٸ� �����
#include "Logger.h"
#include "INIFileLoader.h"
// Network.h�� Server.h�� �̹� ���ԵǾ� �����Ƿ� ����

#pragma comment(lib, "ws2_32.lib")

#define RPMALLOC_ENABLE_OVERRIDE

using namespace Helix;
int main(int argc, const char* argv[])
{
#pragma region initializer
#if !defined(_DEBUG)
	mi_option_set(mi_option_t::mi_option_show_stats, 0);
	mi_option_set(mi_option_t::mi_option_verbose, 0);
#endif // _DEBUG

	wchar_t path[MAX_PATH];
	GetModuleFileNameW(nullptr, path, MAX_PATH);

	std::filesystem::path serverPath(path);

	Helix::Util::Log::Logger::Init(serverPath.filename().string());
	LOG_NOTICE("Logger initialized");

	std::filesystem::path configPath = serverPath.parent_path() / "server_config.ini";
	LOG_NOTICE("Loading configuration from: " + configPath.string());

	Util::INIFileLoader<Core::ServerConfig> configLoader;

	auto config = configLoader.Load(configPath.string());
	if (!config.has_value()) {
		LOG_WARNING(std::format("Config load error: {}", configLoader.GetLastError()));
		return EXIT_FAILURE;
	}

	const auto configVal = config.value();

	// Log configuration
	LOG_NOTICE("Server Configuration:");
	LOG_NOTICE("  IP: " + config.value().ip);
	LOG_NOTICE("  Port: " + std::to_string(config.value().port));
	LOG_NOTICE("  IO Threads: " + std::to_string(config.value().GetIOThreadCount()));
	LOG_NOTICE("  Worker Threads: " + std::to_string(config.value().GetWorkerThreadCount()));
	LOG_NOTICE("  Max Connections: " + std::to_string(config.value().maxConnections));

	if (!Core::Network::g_networkInitializer.IsInitialized())
	{
		LOG_WARNING("Failed to initialize network");
		return EXIT_FAILURE;
	}

	LOG_NOTICE("Network initialized successfully");

#pragma endregion

	// Create server with loaded configuration
	auto server = std::make_unique<GameServer>(std::move(config.value()));

	LOG_NOTICE("Server initialized successfully");

	server->Run();

#pragma region finalizer
	return EXIT_SUCCESS;
#pragma endregion
}
