#include "pch.h"
#include "INIData.h"
#include <INIReader.h>

namespace Helix::Util {
	INIData::INIData(std::unique_ptr<INIReader> reader)
		: _reader(std::move(reader))
	{
	}

	[[nodiscard]] std::string INIData::GetString(const std::string& section, const std::string& name, const std::string& defaultValue /*= 0*/) const
	{
		return _reader->GetString(section, name, defaultValue);
	}

	[[nodiscard]] int32 INIData::GetInt(const std::string& section, const std::string& name, int32 defaultValue /*= 0*/) const
	{
		return _reader->GetInteger(section, name, defaultValue);
	}

	[[nodiscard]] int64 INIData::GetInt64(const std::string& section, const std::string& name, int64 defaultValue /*= 0*/) const
	{
		return _reader->GetInteger64(section, name, defaultValue);
	}

	[[nodiscard]] uint32 INIData::GetUInt(const std::string& section, const std::string& name, uint32 defaultValue /*= 0*/) const
	{
		return _reader->GetUnsigned(section, name, defaultValue);
	}

	[[nodiscard]] uint64 INIData::GetUInt64(const std::string& section, const std::string& name, uint64 defaultValue /*= 0*/) const
	{
		return _reader->GetUnsigned64(section, name, defaultValue);
	}

	[[nodiscard]] double INIData::GetDouble(const std::string& section, const std::string& name, double defaultValue /*= 0*/) const
	{
		return _reader->GetReal(section, name, defaultValue);
	}

	[[nodiscard]] bool INIData::GetBoolean(const std::string& section, const std::string& name, bool defaultValue /*= 0*/) const
	{
		return _reader->GetBoolean(section, name, defaultValue);
	}
}
