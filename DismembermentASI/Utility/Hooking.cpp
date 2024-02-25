#include "..\stdafx.h"


std::vector<HookManager::MemoryBlock> HookManager::g_allocatedMemoryBlocks;

void HookManager::FreeFunctionStubMemory()
{
	constexpr SIZE_T MEMORY_BLOCK_SIZE = 0x1000;

	for (const auto& blockInfo : g_allocatedMemoryBlocks)
	{
		if (blockInfo.startAddress != nullptr)
		{
			DWORD oldProtect;
			VirtualProtect(blockInfo.startAddress, MEMORY_BLOCK_SIZE, PAGE_NOACCESS, &oldProtect);
			VirtualFree(blockInfo.startAddress, 0, MEM_RELEASE);
		}
	}
	g_allocatedMemoryBlocks.clear();
}

PVOID HookManager::AllocateFunctionStub(PVOID origin, PVOID function, uint8_t type)
{
	// Size of each memory block. (= page size of VirtualAlloc)
	constexpr SIZE_T MEMORY_BLOCK_SIZE = 0x1000;

	// Max range for seeking a memory block. (= 1024MB)
	constexpr SIZE_T MAX_MEMORY_RANGE = 0x40000000;

	static void* s_currentStub = nullptr;
	static void* s_stubMemoryStart = nullptr;

	if (!s_currentStub) 
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		MEM_ADDRESS_REQUIREMENTS addressReqs = { 0 };
		MEM_EXTENDED_PARAMETER param = { 0 };

		ULONG_PTR minAddr = (ULONG_PTR)si.lpMinimumApplicationAddress;
		ULONG_PTR maxAddr = (ULONG_PTR)si.lpMaximumApplicationAddress;

		if ((ULONG_PTR)origin > MAX_MEMORY_RANGE && minAddr < (ULONG_PTR)origin - MAX_MEMORY_RANGE)
			minAddr = (ULONG_PTR)origin - MAX_MEMORY_RANGE;

		if (maxAddr > (ULONG_PTR)origin + MAX_MEMORY_RANGE)
			maxAddr = (ULONG_PTR)origin + MAX_MEMORY_RANGE;

		maxAddr -= MEMORY_BLOCK_SIZE - 1;

		auto start = AlignUp(minAddr, si.dwAllocationGranularity);
		auto end = AlignDown(maxAddr, si.dwAllocationGranularity);

		addressReqs.Alignment = NULL; // any alignment
		addressReqs.LowestStartingAddress = (PVOID)start < si.lpMinimumApplicationAddress ? si.lpMinimumApplicationAddress : (PVOID)start;
		addressReqs.HighestEndingAddress = (PVOID)(end - 1) > si.lpMaximumApplicationAddress ? si.lpMaximumApplicationAddress : (PVOID)(end - 1);

		param.Type = MemExtendedParameterAddressRequirements;
		param.Pointer = &addressReqs;

		auto pVirtualAlloc2 = (decltype(&::VirtualAlloc2))GetProcAddress(GetModuleHandleW(L"kernelbase.dll"), "VirtualAlloc2");
			
		s_currentStub = pVirtualAlloc2(GetCurrentProcess(), nullptr, MEMORY_BLOCK_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE, &param, 1);
		if (s_currentStub != nullptr)
		{
			s_stubMemoryStart = s_currentStub;
			MemoryBlock newBlockInfo;
			newBlockInfo.startAddress = s_stubMemoryStart;
			g_allocatedMemoryBlocks.push_back(newBlockInfo);

		}
	}

	if (!s_currentStub)
		return nullptr;

	char* code = (char*)s_currentStub;

	*(uint8_t*)code = 0x48;
	*(uint8_t*)(code + 1) = 0xb8 | type;

	*(uint64_t*)(code + 2) = (uint64_t)function;

	*(uint16_t*)(code + 10) = 0xE0FF | (type << 8);

	*(uint64_t*)(code + 12) = 0xCCCCCCCCCCCCCCCC;

	s_currentStub = (void*)((uint64_t)s_currentStub + 20);

	// the page is full, allocate a new page next time a stub is needed  
	if (((uint64_t)s_currentStub - (uint64_t)s_stubMemoryStart) >= (MEMORY_BLOCK_SIZE - 20))
		s_currentStub = nullptr;

	return code;
}

inline ULONG_PTR HookManager::AlignUp(ULONG_PTR stack, SIZE_T align)
{
	assert(align > 0 && (align & (align - 1)) == 0); // Power of 2 
	assert(stack != 0);

	auto addr = stack;
	if (addr % align != 0)
		addr += align - (addr % align);

	assert(addr >= stack);
	return addr;
}

inline ULONG_PTR HookManager::AlignDown(ULONG_PTR stack, SIZE_T align)
{
	assert(align > 0 && (align & (align - 1)) == 0); // Power of 2 
	assert(stack != 0);

	auto addr = stack;
	addr &= ~(align - 1);
	assert(addr <= stack);
	return addr;
}