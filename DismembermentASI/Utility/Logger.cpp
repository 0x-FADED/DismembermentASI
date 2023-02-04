#include "..\stdafx.h"

Logger::Logger(std::string_view logFileName, LogLevel logLevel, bool truncate)
{
	m_logFilePath = std::filesystem::current_path().append(logFileName);
	m_logLevel = logLevel;

	if (truncate)
	{
		Clear();
	}
}

HMODULE Logger::GetActiveModule()
{
	HMODULE hModule = NULL;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCSTR>(&GetActiveModule),
		&hModule);

	return hModule;
}

std::string Logger::GetModuleName(HMODULE hModule)
{
	TCHAR inBuf[MAX_PATH];

	if (!hModule)
		hModule = GetActiveModule();

	GetModuleFileName(hModule, inBuf, sizeof inBuf);

	auto str = std::string(inBuf);

	auto seperator = str.find_last_of("\\");

	if (seperator != std::string::npos)
		seperator += 1;

	return str.substr(seperator, str.find_last_of(".") - seperator);
}

void Logger::SetFileName(std::string_view newLogFileName)
{
	m_logFilePath = std::filesystem::current_path().append(newLogFileName);
}

void Logger::SetLogLevel(LogLevel newLogLevel)
{
	m_logLevel = newLogLevel;
}

void Logger::Write(std::string_view text) const
{
	Write(m_logLevel, text);
}

void Logger::Write(LogLevel logLevel, std::string_view text) const
{
	if (m_logFilePath.empty() || m_logLevel == LogLevel::LOG_NONE || m_logLevel > logLevel || logLevel < LogLevel::LOG_NONE || logLevel > LogLevel::LOG_ERROR)
	{
		return;
	}

	std::ofstream logFile(m_logFilePath, std::ofstream::out | std::ofstream::app);
	if (logFile)
	{
		auto time = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now()};
		logFile << std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}", time, _logLevelPrefixes[static_cast<int>(logLevel)], text) << "\n";
	}
}

void Logger::Clear() const
{
	std::ofstream logFile(m_logFilePath, std::ofstream::out | std::ofstream::trunc);
}