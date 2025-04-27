#pragma once

#include "WinSock2.h"

namespace Helix::Network {
	class CNetwork
	{
	public:
		CNetwork() = default;
		virtual ~CNetwork() = default;

		void Init()
		{
			::WSADATA wsa;
			if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
			{

			}
		}
	};
} // namespace Helix::Network