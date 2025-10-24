#include "pch.h"
#include "SocketExtension.h"

namespace Helix::Core::Network
{
	SocketExtension::SocketExtension()
	{
		_isInitialized = LoadExtensions();
	}

	bool SocketExtension::LoadExtensions()
	{
		auto socket = INVALID_SOCKET;
		if (socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, nullptr, 0, WSA_FLAG_REGISTERED_IO);
			INVALID_SOCKET == socket)
		{
			return false;
		}

		auto loadExtension = [](SOCKET socket, DWORD cCode, GUID guid, auto& func) -> bool
		{
			DWORD bytes = 0;
			return 0 == ::WSAIoctl(
				socket,
				cCode,
				&guid,
				sizeof(guid),
				&func,
				sizeof(func),
				&bytes,
				nullptr,
				nullptr
			);
		};

		GUID acceptExGuid = WSAID_ACCEPTEX;
		if (!loadExtension(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, acceptExGuid, g_acceptEx)) {
			// Raise error
			return false;
		}

		GUID rioFunctionTable = WSAID_MULTIPLE_RIO;
		if (!loadExtension(socket, SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER, rioFunctionTable, g_rioEx)) {
			// Raise error
			return false;
		}

		::closesocket(socket);

		return true;
	}
}
