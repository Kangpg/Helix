#pragma once

#include "INetwork.h"

namespace Helix::Core::Network
{
	class Iocp : public Helix::Core::Network::Interface::ICompletion
	{
	public:
		Iocp() = default;

		void Run() override;

		HANDLE GetHandle() const override;

	private:
		HANDLE _iocp = nullptr;
	};
} // namespace Helix::Network