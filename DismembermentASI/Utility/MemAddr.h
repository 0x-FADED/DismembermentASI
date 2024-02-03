#pragma once

class MemAddr
{
public:
	MemAddr() : m_ptr(nullptr) {}

	explicit MemAddr(uintptr_t ptr) : m_ptr(reinterpret_cast<void*>(ptr)) {}

	MemAddr(void* obj) : MemAddr(reinterpret_cast<uintptr_t>(obj)) {}

	template <typename T = BYTE>
	MemAddr get(ptrdiff_t offset = 0) const
	{
		return MemAddr(reinterpret_cast<T*>(m_ptr) + offset);
	}

	template<typename T = BYTE>
	MemAddr rip(ptrdiff_t offset = 0) const
	{
		return MemAddr(*reinterpret_cast<int32_t*>(m_ptr) + reinterpret_cast<T*>(m_ptr) + offset);
	}

	void* get_raw()
	{
		return m_ptr;
	}

private:
	void* m_ptr;
};
