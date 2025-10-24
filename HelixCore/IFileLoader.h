#pragma once

#include <optional>
#include <string>
#include "Noncopyable.h"

namespace Helix::Util {
	template<typename T>
	class IFileLoader : public Util::Noncopyable
	{
	public:
		virtual ~IFileLoader() noexcept = default;

		[[nodiscard]] virtual std::optional<T> Load(const std::string& filePath) const = 0;

		[[nodiscard]] virtual T LoadFile(const std::string& filePath) const
		{
			auto data = Load(filePath);
			return data.value_or(T{});
		}

		[[nodiscard]] virtual std::string_view GetLastError() const = 0;
	};
}
