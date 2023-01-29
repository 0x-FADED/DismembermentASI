#include "..\stdafx.h"

// Size of each memory block. (= page size of VirtualAlloc)
constexpr uint64_t MEMORY_BLOCK_SIZE = 0x1000;

// Max range for seeking a memory block. (= 1024MB)
constexpr uint64_t MAX_MEMORY_RANGE = 0x40000000;

PVOID HookManager::AllocateFunctionStub(PVOID origin, PVOID function, int type)
{
	static void* g_currentStub = nullptr;
	static void* g_stubMemoryStart = nullptr;

	if (!g_currentStub) 
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		MEM_ADDRESS_REQUIREMENTS addressReqs = { 0 };
		MEM_EXTENDED_PARAMETER param = { 0 };

		ULONG_PTR minAddr = (ULONG_PTR)si.lpMinimumApplicationAddress;

		if ((ULONG_PTR)origin > MAX_MEMORY_RANGE && minAddr < (ULONG_PTR)origin - MAX_MEMORY_RANGE)
			minAddr = (ULONG_PTR)origin - MAX_MEMORY_RANGE;

		auto pVirtualAlloc2 = (decltype(&::VirtualAlloc2))GetProcAddress(GetModuleHandleA("kernelbase.dll"), "VirtualAlloc2");
		LPVOID pAlloc = origin;

		while ((ULONG_PTR)pAlloc >= minAddr)
		{
			pAlloc = FindPrevFreeRegion(pAlloc, (LPVOID)minAddr, si.dwAllocationGranularity);
			if (pAlloc == NULL)
				break;

			addressReqs.Alignment = NULL; // 0 = any alignment
			addressReqs.LowestStartingAddress = pAlloc;

			param.Type = MemExtendedParameterAddressRequirements;
			param.Pointer = &addressReqs;
			
			g_currentStub = pVirtualAlloc2(GetCurrentProcess(), nullptr, (SIZE_T)MEMORY_BLOCK_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE, &param, 1);
			if (g_currentStub != NULL)
				g_stubMemoryStart = g_currentStub;
			break;
		}
	}

	if (!g_currentStub)
		return nullptr;

	char* code = (char*)g_currentStub;

	*(uint8_t*)code = 0x48;
	*(uint8_t*)(code + 1) = 0xb8 | type;

	*(uint64_t*)(code + 2) = (uint64_t)function;

	*(uint16_t*)(code + 10) = 0xE0FF | (type << 8);

	*(uint64_t*)(code + 12) = 0xCCCCCCCCCCCCCCCC;

	g_currentStub = (void*)((uint64_t)g_currentStub + 20);

	// the page is full, allocate a new page next time a stub is needed  
	if (((uint64_t)g_currentStub - (uint64_t)g_stubMemoryStart) >= (MEMORY_BLOCK_SIZE - 20))
		g_currentStub = nullptr;

	return code;
}

LPVOID HookManager::FindPrevFreeRegion(LPVOID pAddress, LPVOID pMinAddr, DWORD dwAllocationGranularity) 
{
	ULONG_PTR tryAddr = (ULONG_PTR)pAddress;

	// Round down to the next allocation granularity.
	tryAddr -= tryAddr % dwAllocationGranularity;

	// Start from the previous allocation granularity multiply.
	tryAddr -= dwAllocationGranularity;

	while (tryAddr >= (ULONG_PTR)pMinAddr) 
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery((LPVOID)tryAddr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == NULL)
			break;

		if (mbi.State == MEM_FREE)
			return (LPVOID)tryAddr;

		if ((ULONG_PTR)mbi.AllocationBase < dwAllocationGranularity)
			break;

		tryAddr = (ULONG_PTR)mbi.AllocationBase - dwAllocationGranularity;
	}

	return NULL;
}