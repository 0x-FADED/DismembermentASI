#pragma once

template <class T>
class CallHook 
{
public:
	PBYTE address; //0x00 - 0x08
	T fn; //0x08 - 0x10 cuz T is a pointer here?
	CallHook(PBYTE addr, T func) : address(addr), fn(func) { }
	~CallHook();
private:
	void remove();
}; //size: 16

template <class T>
void CallHook<T>::remove() //restore the call to the original function
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

class HookManager // our main class for Hook Logic
{
public:
	template <class T, uint8_t Register>
	static std::enable_if_t<(Register < 8 && Register >= 0), CallHook<T>> *SetCall(PBYTE address, T target)
	{
		if (address != nullptr && *reinterpret_cast<PBYTE>(address) != 0xE8) // just in case measures
		{
			s_LOG->Write(LogLevel::LOG_ERROR, "wrong opcode! cannot hook and continue. expected opcode 0xE8");
			return nullptr;
		}

		T orig = reinterpret_cast<T>(*reinterpret_cast<int32_t*>(address + 1) + (address + 5));

		ptrdiff_t distance = ((intptr_t)target - (intptr_t)address - 5);

		if (distance >= INT32_MAX || distance <= INT32_MIN) // we only need function stub if distance is not within int32_t range or 4 bytes
		{

			auto functionStub = AllocateFunctionStub((PVOID)(*reinterpret_cast<uintptr_t*>(__readgsqword(0x60) + 0x10)), (void*)target, Register);

		//	s_LOG->Write(LogLevel::LOG_DEBUG, std::format("allocated stub memory at {:#X}", (intptr_t)functionStub));

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

	static void FreeFunctionStubMemory();
private:
	struct MemoryBlock
	{
		void* startAddress;
	};
	static std::vector<MemoryBlock> g_allocatedMemoryBlocks;
	static PVOID AllocateFunctionStub(PVOID origin, PVOID function, uint8_t type);
	static inline ULONG_PTR AlignUp(ULONG_PTR stack, SIZE_T align);
	static inline ULONG_PTR AlignDown(ULONG_PTR stack, SIZE_T align);
};
