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
		~INIData() = default;

		[[nodiscard]] std::string GetString(const std::string& section, const std::string& name, const std::string& defaultValue = "") const;
		[[nodiscard]] int32 GetInt(const std::string& section, const std::string& name, int32 defaultValue = 0) const;
		[[nodiscard]] int64 GetInt64(const std::string& section, const std::string& name, int64 defaultValue = 0) const;
		[[nodiscard]] uint32 GetUInt(const std::string& section, const std::string& name, uint32 defaultValue = 0) const;
		[[nodiscard]] uint64 GetUInt64(const std::string& section, const std::string& name, uint64 defaultValue = 0) const;
		[[nodiscard]] double GetDouble(const std::string& section, const std::string& name, double defaultValue = 0.0) const;
		[[nodiscard]] bool GetBoolean(const std::string& section, const std::string& name, bool defaultValue = false) const;

	private:
		std::unique_ptr<INIReader> _reader;
	};
}
