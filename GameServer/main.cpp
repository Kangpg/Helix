#include <iostream>
#include <filesystem>
#include <cassert>

#include "GameServer.h"

#include "Logger.h"
#include "INIFileLoader.h"

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
#pragma endregion

	std::filesystem::path configPath = serverPath.parent_path() / "server_config.ini";
	LOG_NOTICE("Loading configuration from: " + configPath.string());

	Util::INIFileLoader<Core::ServerConfig> configLoader;

	if (!configLoader.Validate(configPath.string()))
	{
		LOG_WARNING("Config file not found, using default configuration");
	}

	auto config = configLoader.LoadOrDefault(configPath.string());

	if (!configLoader.GetLastError().empty())
	{
		LOG_WARNING("Config load error: " + configLoader.GetLastError());
	}

	// Log configuration
	LOG_NOTICE("Server Configuration:");
	LOG_NOTICE("  IP: " + config.ip);
	LOG_NOTICE("  Port: " + std::to_string(config.port));
	LOG_NOTICE("  IO Threads: " + std::to_string(config.GetIOThreadCount()));
	LOG_NOTICE("  Worker Threads: " + std::to_string(config.GetWorkerThreadCount()));
	LOG_NOTICE("  Max Connections: " + std::to_string(config.maxConnections));

	// Create server with loaded configuration
	std::unique_ptr<Helix::Core::Server> server = std::make_unique<GameServer>(std::move(config));

	server->Run();

	LOG_NOTICE("Server initialized successfully");
	LOG_NOTICE("Now server is processing...");

#pragma region finalizer
	return EXIT_SUCCESS;
#pragma endregion
}
