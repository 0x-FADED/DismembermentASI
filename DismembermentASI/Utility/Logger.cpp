#include "..\stdafx.h"

Logger::Logger(const LogLevel logLevel, bool truncate) noexcept
	: m_logFilePath(std::filesystem::current_path().append(std::format(L"{}.log", GetModuleName().data())))
	, m_logLevel(logLevel)
{
	if (truncate)
	{
		Clear();
	}
}

inline auto Logger::GetActiveModule() -> HMODULE
{
	HMODULE hModule = nullptr;

	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&GetActiveModule),
		&hModule);

	return hModule;
}

auto Logger::GetModuleName() -> std::wstring
{
	WCHAR inBuf[MAX_PATH];

	GetModuleFileNameW(GetActiveModule(), inBuf, std::size(inBuf));

	auto str = std::wstring(inBuf);

	auto seperator = str.find_last_of(L"\\");

	if (seperator != std::wstring::npos)
		seperator += 1;

	return str.substr(seperator, str.find_last_of(L".") - seperator);
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
		logFile << std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}", time, _logLevelPrefixes[static_cast<uint8_t>(logLevel)], text) << "\n";
	}
}

void Logger::Clear() const
{
	std::ofstream logFile(m_logFilePath, std::ofstream::out | std::ofstream::trunc);
}