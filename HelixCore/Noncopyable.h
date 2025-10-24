#pragma once

namespace Helix::Util
{
	class Noncopyable
	{
	protected:
		Noncopyable() noexcept = default;
		~Noncopyable() noexcept = default;

		Noncopyable(Noncopyable&&) = default;
		Noncopyable& operator=(Noncopyable&&) = default;

		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};

	class NoncopyableEx
	{
	protected:
		NoncopyableEx() noexcept = default;
		~NoncopyableEx() noexcept = default;

		NoncopyableEx(const NoncopyableEx&) = delete;
		NoncopyableEx& operator=(const NoncopyableEx&) = delete;

		NoncopyableEx(NoncopyableEx&&) = delete;
		NoncopyableEx& operator=(NoncopyableEx&&) = delete;
	};
}