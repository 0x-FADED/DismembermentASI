#pragma once


class Pattern
{
private:
	const char* m_pattern;
	MemAddr<> m_address;
	bool getPattern();

	static auto FindPattern(uintptr_t startAddress, uintptr_t endAddress, const char* mask) -> uintptr_t;
public:
	bool bSuccess = false;

	explicit Pattern(std::string_view pattern) noexcept;
	inline MemAddr<> find() const
	{
		return m_address;
	}
};