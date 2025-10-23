#pragma once

#include <string>
#include <memory>

#include "Noncopyable.h"

class INIReader;

namespace Helix::Util {
	class INIData : public Noncopyable
	{
	public:
		explicit INIData(std::unique_ptr<INIReader> reader);
		~INIData();

		INIData(const INIData&) = delete;
		INIData& operator=(const INIData&) = delete;
		INIData(INIData&&) noexcept;
		INIData& operator=(INIData&&) noexcept;

		[[nodiscard]] std::string GetString(const std::string& section, const std::string& name, const std::string& defaultValue = "") const;
		[[nodiscard]] long GetInteger(const std::string& section, const std::string& name, long defaultValue = 0) const;
		[[nodiscard]] unsigned long GetUnsigned(const std::string& section, const std::string& name, unsigned long defaultValue = 0) const;
		[[nodiscard]] double GetReal(const std::string& section, const std::string& name, double defaultValue = 0.0) const;
		[[nodiscard]] bool GetBoolean(const std::string& section, const std::string& name, bool defaultValue = false) const;

	private:
		std::unique_ptr<INIReader> _reader;
	};
}
