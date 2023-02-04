#pragma once

enum class LogLevel
{
	LOG_NONE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_ERROR
};

static constexpr const char* _logLevelPrefixes[4] = { "", "DEBUG", "INFO", "ERROR" };

class Logger final
{
private:
	std::filesystem::path m_logFilePath;
	LogLevel m_logLevel = LogLevel::LOG_NONE;
	static HMODULE GetActiveModule();
public:
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger(std::string_view logFileName, LogLevel logLevel, bool truncate);
	void SetFileName(std::string_view newLogFileName);
	void SetLogLevel(LogLevel newLogLevel);

	void Write(std::string_view text) const;
	void Write(LogLevel logLevel, std::string_view text) const;

	void Clear() const;
	static std::string GetModuleName(HMODULE hModule);
};

static Logger LOG(std::format("{}.log", Logger::GetModuleName(nullptr)), LogLevel::LOG_INFO, true); // set LogLevel to LOG_DEBUG for debugging