#include "pch.h"
#include "INIData.h"
#include <INIReader.h>

namespace Helix::Util {
	INIData::INIData(std::unique_ptr<INIReader> reader)
		: _reader(std::move(reader))
	{
	}

	INIData::~INIData() = default;

	INIData::INIData(INIData&&) noexcept = default;
	INIData& INIData::operator=(INIData&&) noexcept = default;

	std::string INIData::GetString(const std::string& section, const std::string& name, const std::string& defaultValue) const
	{
		return _reader->GetString(section, name, defaultValue);
	}

	long INIData::GetInteger(const std::string& section, const std::string& name, long defaultValue) const
	{
		return _reader->GetInteger(section, name, defaultValue);
	}

	unsigned long INIData::GetUnsigned(const std::string& section, const std::string& name, unsigned long defaultValue) const
	{
		return static_cast<unsigned long>(_reader->GetInteger(section, name, static_cast<long>(defaultValue)));
	}

	double INIData::GetReal(const std::string& section, const std::string& name, double defaultValue) const
	{
		return _reader->GetReal(section, name, defaultValue);
	}

	bool INIData::GetBoolean(const std::string& section, const std::string& name, bool defaultValue) const
	{
		return _reader->GetBoolean(section, name, defaultValue);
	}
}
