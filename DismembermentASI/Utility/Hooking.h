#pragma once

template <class T>
class CallHook 
{
public:
	CallHook(PBYTE addr, T func) : address(addr), fn(func) { }
	~CallHook();
	void remove();
	PBYTE address;
	T fn;
};

template <class T>
void CallHook<T>::remove()
{
	DWORD oldProtect;
	VirtualProtect((BYTE*)address, 5Ui64, PAGE_EXECUTE_READWRITE, &oldProtect);

	*reinterpret_cast<int32_t*>(address + 1) = static_cast<int32_t>((intptr_t)fn - (intptr_t)address - 5);

	VirtualProtect((BYTE*)address, 5Ui64, oldProtect, &oldProtect);
	FlushInstructionCache(GetCurrentProcess(), (BYTE*)address, 5Ui64);
}

template <class T>
CallHook<T>::~CallHook()
{
	CallHook<T>::remove();
}

class HookManager
{
public:
	template <class T, int Register>
	static std::enable_if_t<(Register < 8 && Register >= 0), CallHook<T>> *SetCall(PBYTE address, T target)
	{
		if (address != nullptr && *reinterpret_cast<PBYTE>(address) != 0xE8)
		{
			LOG.Write(LogLevel::LOG_ERROR, "wrong opcode! cannot hook and continue. expected opcode 0xE8");
			return nullptr;
		}

		T orig = reinterpret_cast<T>(*reinterpret_cast<int32_t*>(address + 1) + (address + 5));

		ptrdiff_t distance = ((intptr_t)target - (intptr_t)address - 5);

		if (distance >= INT32_MAX || distance <= INT32_MIN) // we only need function stub if distance is not in int32_t range
		{

			auto functionStub = AllocateFunctionStub(GetModuleHandle(nullptr), (void*)target, Register);

		//	LOG.Write(LogLevel::LOG_DEBUG, std::format("allocated stub memory at {:#X}", (intptr_t)functionStub));

			 distance = static_cast<int32_t>((intptr_t)functionStub - (intptr_t)address - 5);
		}

		DWORD oldProtect;
		VirtualProtect((BYTE*)address, 5Ui64, PAGE_EXECUTE_READWRITE, &oldProtect);

		*reinterpret_cast<PBYTE>(address) = 0xE8;
		*reinterpret_cast<int32_t*>(address + 1) = distance;

		VirtualProtect((BYTE*)address, 5Ui64, oldProtect, &oldProtect);
		FlushInstructionCache(GetCurrentProcess(), (BYTE*)address, 5Ui64);

		return new CallHook<T>(address, orig);
	}
	
	static PVOID AllocateFunctionStub(PVOID origin, PVOID function, int type);
	static inline ULONG_PTR AlignUp(ULONG_PTR stack, SIZE_T align);
	static inline ULONG_PTR AlignDown(ULONG_PTR stack, SIZE_T align);
};
