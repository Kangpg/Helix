#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>
#include <bit>
#include <curl/curl.h>
#include "Logger.h"

#pragma comment(lib, "crypt32.lib")

namespace Helix::Core::Network 
{
	class Network
	{
	public:
		Network() = default;

		static bool Initialize()
		{
			if (const auto ret = ::curl_global_init(CURL_GLOBAL_ALL); ret != CURLcode::CURLE_OK) {

				LOG_FATAL(std::format("curl initialize error {}", ::curl_easy_strerror(ret)));
				return false;
			}

			return true;
		}

		static bool Finalize() 
		{
			::curl_global_cleanup();
			return true;
		}

		inline static LPFN_CONNECTEX g_connectEx = nullptr;
		inline static LPFN_ACCEPTEX g_acceptEx = nullptr;
		inline static LPFN_DISCONNECTEX g_disconnectEx = nullptr;

		static bool LoadExFunction()
		{
			auto socket = INVALID_SOCKET;
			if (socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_REGISTERED_IO); INVALID_SOCKET == socket)
			{
				LOG_FATAL("");
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
			lambLoadEx(socket, connectEx, g_connectEx);

			GUID disconnectEx = WSAID_DISCONNECTEX;
			lambLoadEx(socket, disconnectEx, g_disconnectEx);

			GUID acceptEx = WSAID_ACCEPTEX;
			lambLoadEx(socket, acceptEx, g_acceptEx);

			::closesocket(socket);

			return true;
		}
	};
} // namespace Helix::Network