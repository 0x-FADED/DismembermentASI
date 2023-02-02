#pragma once

template <typename T>
class CallHook 
{
public:
	CallHook(PBYTE addr, T func) : address(addr), fn(func) { }
	~CallHook();
	void remove();
	PBYTE address;
	T fn;
};

template <typename T>
void CallHook<T>::remove()
{
	DWORD oldProtect;
	VirtualProtect((BYTE*)address, 5Ui64, PAGE_EXECUTE_READWRITE, &oldProtect);

	*reinterpret_cast<int32_t*>(address + 1) = static_cast<int32_t>((intptr_t)fn - (intptr_t)address - 5);

	VirtualProtect((BYTE*)address, 5Ui64, oldProtect, &oldProtect);
	FlushInstructionCache(GetCurrentProcess(), (BYTE*)address, 5Ui64);
}

template <typename T>
CallHook<T>::~CallHook()
{
	CallHook<T>::remove();
}

class HookManager
{
public:
	template <typename T>
	static CallHook<T> *SetCall(PBYTE address, T target)
	{
		T orig = reinterpret_cast<T>(*reinterpret_cast<int32_t*>(address + 1) + (address + 5));

		auto pFunc = AllocateFunctionStub(GetModuleHandle(nullptr), (void*)target, NULL);

		DWORD oldProtect;
		VirtualProtect((BYTE*)address, 5Ui64, PAGE_EXECUTE_READWRITE, &oldProtect);

		*reinterpret_cast<BYTE*>(address) = 0xE8;
		*reinterpret_cast<int32_t*>(address + 1) = static_cast<int32_t>((intptr_t)pFunc - (intptr_t)address - 5);

		VirtualProtect((BYTE*)address, 5Ui64, oldProtect, &oldProtect);
		FlushInstructionCache(GetCurrentProcess(), (BYTE*)address, 5Ui64);

		return new CallHook<T>(address, orig);
	}

	static PVOID AllocateFunctionStub(PVOID origin, PVOID function, int type);
	static LPVOID FindPrevFreeRegion(LPVOID pAddress, LPVOID pMinAddr, DWORD dwAllocationGranularity);
};
