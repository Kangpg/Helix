#pragma once

namespace Helix::Util {
	template<typename DataSource>
	class ILoadable
	{
	public:
		virtual ~ILoadable() noexcept = default;

		[[nodiscard]] virtual bool LoadFrom(const DataSource& source) = 0;
	};
}
