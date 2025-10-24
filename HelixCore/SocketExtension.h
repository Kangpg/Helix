#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "Noncopyable.h"

namespace Helix::Core::Network
{
	class SocketExtension : public Util::Noncopyable
	{
	public:
		SocketExtension();
		virtual ~SocketExtension() noexcept = default;

		[[nodiscard]] bool IsInitialized() const noexcept { return _isInitialized; }

		[[nodiscard]] static LPFN_ACCEPTEX GetAcceptEx() noexcept { return g_acceptEx; }
		[[nodiscard]] static PRIO_EXTENSION_FUNCTION_TABLE GetRIOEx() noexcept { return g_rioEx; }

	private:
		bool LoadExtensions();

		bool _isInitialized = false;
		inline static LPFN_ACCEPTEX g_acceptEx{};
		inline static PRIO_EXTENSION_FUNCTION_TABLE g_rioEx{};
	};
} // namespace Helix::Core::Network
