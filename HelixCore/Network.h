#pragma once

#include <WinSock2.h>
#include <MSWSock.h>

namespace Helix::Core::Network {
	class Network
	{
		Network() = delete;
		virtual ~Network() = delete;

		static bool NetworkInitialize();
		static void NetworkFinalize();

		static bool LoadEx();

		inline static LPFN_CONNECTEX g_connectEx = nullptr;
		inline static LPFN_DISCONNECTEX g_disconnectEx = nullptr;
		inline static LPFN_ACCEPTEX g_acceptEx = nullptr;
	};
}
