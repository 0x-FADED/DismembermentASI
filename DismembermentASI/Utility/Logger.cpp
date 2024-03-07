#include "..\stdafx.h"

Logger::Logger(const LogLevel logLevel, bool truncate) noexcept
	: m_logFilePath(std::filesystem::current_path().append(std::format("{}.log", GetModuleName())))
	, m_logLevel(logLevel)
	, m_logFile{}
{
	if (truncate)
	{
		m_logFile.open(m_logFilePath, std::ofstream::out | std::ofstream::trunc);
	}
	else
	{
		m_logFile.open(m_logFilePath, std::ofstream::out | std::ofstream::app);

	}
}
Logger::~Logger() noexcept
{
	if (m_logFile.is_open())
	{
		m_logFile.close();
	}
	m_logFilePath.clear();
}

inline auto Logger::GetActiveModule() -> HMODULE
{
	HMODULE hModule = nullptr;

	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&GetModuleName),
		&hModule);

	return hModule;
}

auto Logger::GetModuleName() -> std::string
{
	CHAR inBuf[MAX_PATH];

	GetModuleFileName(GetActiveModule(), inBuf, std::size(inBuf));

	const auto str = std::string(inBuf);

	auto seperator = str.find_last_of("\\");

	if (seperator != std::string::npos)
		seperator += 1;

	return str.substr(seperator, str.find_last_of(".") - seperator).data();
}

void Logger::Write(std::string_view text) const&
{
	Write(m_logLevel, text);
}

void Logger::Write(LogLevel logLevel, std::string_view text) const&
{
	if (m_logFilePath.empty() || m_logLevel == LogLevel::LOG_NONE || m_logLevel > logLevel || m_logFile.fail())
	{
		return;
	}

	auto time = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now()};
	m_logFile << std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}", time, _logLevelPrefixes[static_cast<uint8_t>(logLevel)], text) << std::endl;
}

const std::unique_ptr<Logger> s_LOG = std::make_unique<Logger>(LogLevel::LOG_INFO, true);