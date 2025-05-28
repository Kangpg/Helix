#pragma once

namespace Helix::Util
{
	class Noncopable
	{
	public:
		Noncopable() = default;
		Noncopable(Noncopable&&) = default;
		Noncopable& operator=(Noncopable&&) = default;

		Noncopable(const Noncopable&) = delete;
		Noncopable& operator=(const Noncopable&) = delete;
	};
}