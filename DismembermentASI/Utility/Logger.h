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
	static auto GetModuleName() -> std::wstring;
	std::filesystem::path m_logFilePath;
	LogLevel m_logLevel = LogLevel::LOG_NONE;
	void Clear() const;
public:
	explicit Logger(const LogLevel logLevel, bool truncate) noexcept;
	void Write(std::string_view text) const;
	void Write(LogLevel logLevel, std::string_view text) const;
};

static Logger LOG(LogLevel::LOG_INFO, true); // set LogLevel to LOG_DEBUG for debugging