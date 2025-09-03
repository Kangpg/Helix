#pragma once

namespace Helix::Util
{
	class Noncopyable
	{
	public:
		Noncopyable() = default;
		Noncopyable(Noncopyable&&) = default;
		Noncopyable& operator=(Noncopyable&&) = default;

		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};
}