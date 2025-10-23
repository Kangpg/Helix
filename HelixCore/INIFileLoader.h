#pragma once

#include "IFileLoader.h"
#include "ILoadable.h"
#include "INIData.h"
#include <memory>

#include <INIReader.h>

namespace Helix::Util {
	template<typename T>
	class INIFileLoader : public IFileLoader<T>
	{
	public:
		INIFileLoader() = default;
		~INIFileLoader() noexcept override = default;

		[[nodiscard]] std::optional<T> Load(const std::string& filePath) const override
		{
			static_assert(std::is_base_of_v<ILoadable<INIData>, T>,"T must implement ILoadable<INIData>");

			_lastError.clear();

			auto reader = std::make_unique<INIReader>(filePath);
			if (reader->ParseError() != 0)
			{
				if (reader->ParseError() < 0)
				{
					_lastError = "Failed to open file: " + filePath;
				}
				else
				{
					_lastError = "Parse error at line " + std::to_string(reader->ParseError()) + " in file: " + filePath;
				}
				return std::nullopt;
			}

			INIData iniData(std::move(reader));

			T config;
			if (!config.LoadFrom(iniData))
			{
				_lastError = "Failed to load configuration from INI data";
				return std::nullopt;
			}

			return config;
		}

		[[nodiscard]] bool Validate(const std::string& filePath) const override
		{
			auto result = Load(filePath);
			return result.has_value();
		}

		[[nodiscard]] std::string GetLastError() const override
		{
			return _lastError;
		}

	private:
		mutable std::string _lastError;
	};
}