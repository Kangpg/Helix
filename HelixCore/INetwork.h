#pragma once

#include <WinSock2.h>

namespace Helix::Core::Network::Interface {
	class ICompletion
	{
	public:
		virtual ~ICompletion() = default;

		virtual void Run() abstract;
		
		virtual HANDLE GetHandle() const abstract;
	};

	class IAcceptor
	{
	public:
		virtual ~IAcceptor() = default;

		virtual bool Listen(const ::sockaddr_storage& address) abstract;
		virtual bool Accept() abstract;
	};

	class IDispatcher
	{
	public:
		virtual ~IDispatcher() = default;

		virtual void OnCompletion() abstract;
	};

	class ISession
	{
	public:
		virtual ~ISession() = default;

		virtual void OnConnect() abstract;
		virtual void OnDisconnect() abstract;
		virtual void OnRecv() abstract;
		virtual void OnSend() abstract;
	};
}
