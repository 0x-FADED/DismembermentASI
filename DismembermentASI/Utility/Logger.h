#pragma once

enum class LogLevel : uint8_t
{
	LOG_NONE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_ERROR
};

class Logger final
{
private:
	static constexpr const char* _logLevelPrefixes[4] = { "", "DEBUG", "INFO", "ERROR" };
	static inline auto GetActiveModule() -> HMODULE;
	static auto GetModuleName() -> std::string;
	mutable std::ofstream m_logFile;
	std::filesystem::path m_logFilePath;
	LogLevel m_logLevel = LogLevel::LOG_NONE;
public:
	explicit Logger(const LogLevel logLevel, bool truncate) noexcept;
	~Logger() noexcept;
	void Write(std::string_view text) const&;
	void Write(LogLevel logLevel, std::string_view text) const&;
};

extern const std::unique_ptr<Logger> s_LOG;