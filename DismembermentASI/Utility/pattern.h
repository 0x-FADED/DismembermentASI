#pragma once

template <class T>
class Pattern
{
public:
	inline Pattern(std::string_view pattern) : pattern(pattern.data())
	{
		bSuccess = getPattern();
	}

	inline T get(ptrdiff_t offset = 0)
	{
		return pResult + offset;
	}
	inline T rip(ptrdiff_t offset = 0)
	{
		auto pReloffset = *reinterpret_cast<int32_t*>(pResult + offset) + sizeof(int32_t);
		return pResult + offset + pReloffset;
	}

	bool bSuccess;

private:
	auto getPattern() -> bool
	{
		// mov rax, gs:0x30    ; NtCurrentTeb() or ptr to the thread environment block or the PTEB
		// mov rdx, [rax+0x60] ; TEB + 0x60 = PPEB or ptr to the process environment block
		// mov rax, [rdx+0x10] ; PEB + 0x10 = ImageBaseAddress
		// pretty smart huh?

		//static const uintptr_t moduleBase = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(__readgsqword(0x30) + 0x60) + 0x10); // getting module base in this way should save some processing

		static const uintptr_t moduleBase = *reinterpret_cast<uintptr_t*>(__readgsqword(0x60) + 0x10); 

		static const uintptr_t moduleEnd  = moduleBase + static_cast<uintptr_t>(reinterpret_cast<PIMAGE_NT_HEADERS64>(moduleBase + reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase)->e_lfanew)->OptionalHeader.SizeOfImage);

		const uintptr_t address = FindPattern(moduleBase, moduleEnd, pattern);
		if (address != NULL)
		{
			pResult = reinterpret_cast<T>(address);
			return true;
		}

		pResult = NULL;
		return false;
	}

	static auto FindPattern(uintptr_t startAddress, uintptr_t endAddress, const char* mask) -> uintptr_t
	{
		std::vector<std::pair<uint8_t, bool>> pattern;

		for (size_t i = 0; i < strlen(mask);)
		{
			if (mask[i] != '?')
			{
				pattern.emplace_back(static_cast<uint8_t>(strtoul(&mask[i], nullptr, 16)), false);
				i += 3;
			}
			else
			{
				pattern.emplace_back(0x00, true);
				i += 2;
			}
		}

		const auto dataStart = reinterpret_cast<uint8_t*>(startAddress);
		const auto dataEnd = dataStart + endAddress + 1;

		auto sig = std::search(dataStart, dataEnd, pattern.begin(), pattern.end(),
			[](uint8_t currentByte, std::pair<uint8_t, bool> Pattern)
			{
				return Pattern.second || (currentByte == Pattern.first);
			});

		if (sig == dataEnd)
			return NULL;

		return std::distance<uint8_t*>(dataStart, sig) + startAddress;
	}

	const char* pattern;
	T pResult;
};
