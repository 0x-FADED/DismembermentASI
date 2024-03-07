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
	static constinit inline const char* _logLevelPrefixes[4] = { "", "DEBUG", "INFO", "ERROR" };
	static inline auto GetActiveModule() -> HMODULE;
	static auto GetModuleName() -> std::string;
	std::filesystem::path m_logFilePath;
	LogLevel m_logLevel = LogLevel::LOG_NONE;
	void Clear() const;
public:
	explicit Logger(const LogLevel logLevel, bool truncate) noexcept;
	void Write(std::string_view text) const;
	void Write(LogLevel logLevel, std::string_view text) const;
};

extern std::unique_ptr<Logger> s_LOG;