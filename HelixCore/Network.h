#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "Noncopyable.h"

namespace Helix::Core::Network
{
	class NetworkInitializer : public Util::Noncopyable
	{
	public:
		NetworkInitializer();
		virtual ~NetworkInitializer() noexcept;

		[[nodiscard]] bool IsInitialized() const noexcept { return _isInitialized; }

	private:
		bool Initialize() noexcept;
		bool Finalize();

		bool _isInitialized = false;
	};

	static NetworkInitializer g_networkInitializer{};
} // namespace Helix::Core::Network