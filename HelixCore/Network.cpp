#include "Network.h"

namespace Helix::Core::Network {
	bool Network::NetworkInitialize()
	{
		WSADATA wsa;
		if (auto ret = ::WSAStartup(MAKEWORD(2, 2), &wsa); ret) 
		{
			return false;
		}

		return true;
	}

	void Network::NetworkFinalize()
	{
		::WSACleanup();
	}

	bool Network::LoadEx()
	{
		auto socket = INVALID_SOCKET;
		if (socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED); INVALID_SOCKET == socket)
		{
			return false;
		}

		auto lambLoadEx = [](SOCKET socket, GUID guid, auto& func)
			{
				DWORD bytes = 0;
				return 0 == ::WSAIoctl(
					socket,
					SIO_GET_EXTENSION_FUNCTION_POINTER,
					&guid,
					sizeof(guid),
					&func,
					sizeof(func),
					&bytes,
					nullptr,
					nullptr
				);
			};

		GUID connectEx = WSAID_CONNECTEX;
		if (!lambLoadEx(socket, connectEx, g_connectEx)) {
			return false;
		}

		GUID disconnectEx = WSAID_DISCONNECTEX;
		if (!lambLoadEx(socket, disconnectEx, g_disconnectEx)) {
			return false;
		}

		GUID acceptEx = WSAID_ACCEPTEX;
		if (!lambLoadEx(socket, acceptEx, g_acceptEx)) {
			return false;
		}

		::closesocket(socket);

		return true;
	}

}
