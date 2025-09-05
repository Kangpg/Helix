#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>
#include <bit>
#include "Logger.h"

namespace Helix::Core::Network 
{
	class Network
	{
	public:
		Network() = default;

		static bool Initialize();
		static bool Finalize();

		inline static LPFN_CONNECTEX g_connectEx = nullptr;
		inline static LPFN_ACCEPTEX g_acceptEx = nullptr;
		inline static LPFN_DISCONNECTEX g_disconnectEx = nullptr;

		static bool LoadExFunction();
	};
} // namespace Helix::Network