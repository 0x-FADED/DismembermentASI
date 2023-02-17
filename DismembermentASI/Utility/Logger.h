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
	static HMODULE GetActiveModule();
	static std::string GetModuleName();
	std::filesystem::path m_logFilePath;
	LogLevel m_logLevel = LogLevel::LOG_NONE;
	void Clear() const;
public:
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger(LogLevel logLevel, bool truncate);

	void Write(std::string_view text) const;
	void Write(LogLevel logLevel, std::string_view text) const;

};

static Logger LOG(LogLevel::LOG_INFO, true); // set LogLevel to LOG_DEBUG for debugging