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

#pragma comment(lib, "Dbghelp.lib")

using namespace Helix::Types;
namespace Helix::Logger {
	enum class eLogLevel
	{
		Debug = 0,
		Notice,
		Warnning,
		Fatal,
	};

	class CLogger
	{
	public:
		CLogger() = default;
		~CLogger() = default;

		static void Init(std::string_view logName) 
		{
			_currentLogDay = GetDate();
			_fileName = logName;

			OpenLogFile(logName);


		}

		static void Log(eLogLevel level, std::string_view funcName, int32 line, std::string_view logVal)
		{
			OpenLogFile(_fileName);

			_stream << funcName << "(" << line << ")" << " : " << logVal;
		}

		static void OpenLogFile(std::string_view logName) 
		{
			std::lock_guard<std::recursive_mutex> lock(_lock);

			if (_stream.is_open())
			{
				_stream.close();
			}

			const int32 date = GetDate();

			if (_currentLogDay > date)
			{
				_currentLogDay = date;
			}

			_stream.open(std::format("{}_{}.log", logName, std::to_string(_currentLogDay), std::ios::out | std::ios::app));
		}

		static void StackTrace()
		{
			constexpr int MaxFrames = 62;
			void* stack[MaxFrames];
			USHORT frames = CaptureStackBackTrace(0, MaxFrames, stack, nullptr);

			HANDLE process = GetCurrentProcess();
			SymInitialize(process, nullptr, TRUE);

			static thread_local SYMBOL_INFO symbolInfoPool = {};
			SYMBOL_INFO* symbol = &symbolInfoPool;
			symbol->MaxNameLen = 255;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			for (USHORT i = 0; i < frames; ++i)
			{
				if (SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol))
				{
					std::string line = std::format("[{}] {} - 0x{:X}\n", i, std::string_view(symbol->Name), symbol->Address);
					std::cout << line;
					if (_stream.is_open())
					{
						_stream << line;
					}
				}
				else
				{
					std::string line = std::format("[{}] (unknown symbol) - 0x{:X}\n", i, (uintptr_t)stack[i]);
					std::cout << line;
					if (_stream.is_open())
					{
						_stream << line;
					}
				}
			}
		}

		static int32 GetDate() 
		{
			auto now = std::chrono::system_clock::now();
			std::time_t t = std::chrono::system_clock::to_time_t(now);

			struct tm timeinfo;
			localtime_s(&timeinfo, &t);

			return (timeinfo.tm_year + 1900) * 10000 + (timeinfo.tm_mon + 1) * 100 + timeinfo.tm_mday;
		}

	private:
		static inline std::recursive_mutex _lock;
		static inline std::ofstream _stream;
		static inline std::string _fileName;
		static inline int32 _currentLogDay;
	};
} // Helix::Logger