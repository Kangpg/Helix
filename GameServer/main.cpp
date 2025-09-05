#include <iostream>
#include <filesystem>
#include <cassert>

#include "GameServer.h"

#include "../HelixCore/Logger.h"

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
	LOG_NOTICE("Logger cannot init...");
#pragma endregion
	
#pragma region main_logic	
	std::unique_ptr<Helix::Core::Base::Server> server = std::make_unique<GameServer>([] {
		sockaddr_storage storage{};
		*reinterpret_cast<sockaddr_in*>(&storage) = sockaddr_in{
			.sin_family = AF_INET,
			.sin_port = htons(5000),
			.sin_addr = [] {
				in_addr addr{};
				inet_pton(AF_INET, "0.0.0.0", &addr);
				return addr;
			}(),
		};
		return storage;
	}());

	LOG_NOTICE("Now server is processing...");
#pragma endregion 

#pragma region finalizer
	return EXIT_SUCCESS;
#pragma endregion
}