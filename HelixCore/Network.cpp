#include "Network.h"

namespace Helix::Core::Network
{
	NetworkInitializer::NetworkInitializer()
	{
		_isInitialized = Initialize();
	}

	NetworkInitializer::~NetworkInitializer() noexcept
	{
		Finalize();
	}

	[[nodiscard]] bool NetworkInitializer::Initialize() noexcept
	{
		WSADATA wsa;
		return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
	}

	bool NetworkInitializer::Finalize()
	{
		return WSACleanup() == 0;
	}
}