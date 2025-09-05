#include "pch.h"
#include "Iocp.h"

namespace Helix::Core::Network {
	Iocp::Iocp()
	{

	}

	void Iocp::Run()
	{

	}

	HANDLE Iocp::GetHandle() const
	{
		return _iocp;
	}
}