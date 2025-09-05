#pragma once

#include <Windows.h>
#include <string_view>
#include <mutex>
#include <iostream>
#include <fstream>
#include <DbgHelp.h>
#include <chrono>
#include <string>

#include "Types.h"

#define LOG_DEBUG(msg) Helix::Util::Log::Logger::Log(Helix::Util::Log::eLogLevel::Debug, __FUNCTION__, __LINE__, msg);
#define LOG_NOTICE(msg) Helix::Util::Log::Logger::Log(Helix::Util::Log::eLogLevel::Notice, __FUNCTION__, __LINE__, msg);
#define LOG_WARNING(msg) Helix::Util::Log::Logger::Log(Helix::Util::Log::eLogLevel::Warning, __FUNCTION__, __LINE__, msg);
#define	LOG_FATAL(msg) Helix::Util::Log::Logger::Log(Helix::Util::Log::eLogLevel::Fatal, __FUNCTION__, __LINE__, msg);

using namespace Helix::Util::Types;
namespace Helix::Util::Log 
{
	enum class eLogLevel { Debug, Notice, Warning, Fatal };

	class Logger final
	{
	public:
		Logger() = default;
		~Logger() = default;

		static void Init(std::string_view logName) 
		{
			_currentLogDay = GetDate();

			OpenLogFile(logName);
		}

		static void Log(eLogLevel level, std::string_view funcName, int32 line, std::string_view logVal = "")
		{
			_stream << "[" << std::this_thread::get_id() << "]" << funcName << "(" << line << ")" << " : " << logVal << "\n";
		}

		static void OpenLogFile(std::string_view fileName) 
		{
			std::lock_guard<std::recursive_mutex> lock(_lock);

			if (_stream.is_open())
			{
				_stream.close();
			}

			const int32 date = GetDate();

			if (_currentLogDay < date)
			{
				_currentLogDay = date;
			}
#if defined(_MSC_VER)
			_fileName = std::format("{}_{}.log", fileName, _currentLogDay);
			_stream.open(_fileName, std::ios::out | std::ios::app);
#endif // _MSC_VER
		}

		static void StackTrace()
		{
			constexpr int maxFrames = 64;
			void* stack[maxFrames];
			USHORT frames = CaptureStackBackTrace(0, maxFrames, stack, nullptr);

			HANDLE process = GetCurrentProcess();
			SymInitialize(process, nullptr, TRUE);

			static thread_local SYMBOL_INFO storedSymbol = {};
			ZeroMemory(&storedSymbol, sizeof(storedSymbol));

			SYMBOL_INFO* symbol = &storedSymbol;
			symbol->MaxNameLen = 255;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			for (USHORT i = 0; i < frames; ++i)
			{
				if (SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol))
				{
#if defined(_MSC_VER)
					std::string line = std::format("[{}] {} - 0x{:X}\n", i, std::string_view(symbol->Name), symbol->Address);
#endif // _MSC_VER
					std::cout << line;
					if (_stream.is_open())
					{
						_stream << line;
					}
				}
				else
				{
#if defined(_MSC_VER)
					std::string line = std::format("[{}] (unknown symbol) - 0x{:X}\n", i, (uintptr_t)stack[i]);
#endif // _MSC_VER
					std::cout << line;
					if (_stream.is_open())
					{
						_stream << line;
					}
				}
			}

			SymCleanup(process);
		}

		static int32 GetDate() 
		{
			auto now = std::chrono::system_clock::now();
			std::time_t t = std::chrono::system_clock::to_time_t(now);


			struct tm timeInfo;
			localtime_s(&timeInfo, &t);
			
			return (timeInfo.tm_year + 1900) * 10000 + (timeInfo.tm_mon + 1) * 100 + timeInfo.tm_mday;
		}

	private:
		static inline std::recursive_mutex _lock;
		static inline std::ofstream _stream;
		static inline std::string _fileName;
		static inline int32 _currentLogDay = 0;
	};
} // Helix::Logger