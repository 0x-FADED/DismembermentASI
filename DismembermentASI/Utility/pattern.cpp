#include "..\stdafx.h"

Pattern::Pattern(std::string_view pattern) noexcept : m_pattern(pattern.data())
{
    bSuccess = getPattern();
}

bool Pattern::getPattern()
{
	// mov rax, gs:0x30    ; NtCurrentTeb() or ptr to the thread environment block or the PTEB
	// mov rdx, [rax+0x60] ; TEB + 0x60 = PPEB or ptr to the process environment block
	// mov rax, [rdx+0x10] ; PEB + 0x10 = ImageBaseAddress
	// pretty smart huh?

	//static const uintptr_t moduleBase = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(__readgsqword(0x30) + 0x60) + 0x10); // getting module base in this way should save some processing

	static const uintptr_t moduleBase = *reinterpret_cast<uintptr_t*>(__readgsqword(0x60) + 0x10);

	static const uintptr_t moduleEnd = moduleBase + static_cast<uintptr_t>(reinterpret_cast<PIMAGE_NT_HEADERS64>(moduleBase + reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase)->e_lfanew)->OptionalHeader.SizeOfImage);

	const uintptr_t address = FindPattern(moduleBase, moduleEnd, m_pattern);
	if (address != NULL)
	{
		m_address = MemAddr(address);
        return true;
	}

	m_address = MemAddr();
    return false;
}

auto Pattern::FindPattern(uintptr_t startAddress, uintptr_t endAddress, const char* mask) -> uintptr_t
{
    std::vector<std::pair<uint8_t, bool>> pattern;
    const size_t length = std::char_traits<char>::length(mask);

    for (size_t i = 0; i < length;)
    {
        if (mask[i] != '?')
        {
            uint8_t value;
            std::from_chars(&mask[i], nullptr, value, 16);
            pattern.emplace_back(value, false);
            i += 3;
        }
        else
        {
            pattern.emplace_back(0x00, true);
            i += 2;
        }
    }

    const auto dataStart = std::bit_cast<uint8_t*>(startAddress);
    const auto dataEnd = dataStart + endAddress + 1;
    const auto patternSize = pattern.size();

    for (auto iter = dataStart; iter + patternSize <= dataEnd; ++iter)
    {
        if (std::equal(iter, iter + patternSize, pattern.begin(),
            [](uint8_t currentByte, const std::pair<uint8_t, bool>& PatternElements)
            {
                return PatternElements.second || (currentByte == PatternElements.first);
            }))
        {
            return std::bit_cast<uintptr_t>(iter);
        }
    }

    return NULL;
}