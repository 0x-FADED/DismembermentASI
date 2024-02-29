#pragma once

template<class T = std::byte>
class MemAddr 
{
public:
    constexpr MemAddr() = default;

    inline explicit MemAddr(std::uintptr_t ptr) : m_ptr(std::bit_cast<T*>(ptr)) {}

    inline explicit MemAddr(void* obj) : MemAddr(std::bit_cast<std::uintptr_t>(obj)) {}

    template <typename U = T>
    [[nodiscard]] inline MemAddr<> get(std::ptrdiff_t offset = 0) const 
    {
        return MemAddr<>(std::bit_cast<U*>(m_ptr) + offset);
    }

    template <typename R = T>
    [[nodiscard]] inline MemAddr<> rip(std::ptrdiff_t offset = 0) const
    {
        return MemAddr<>(*std::bit_cast<std::int32_t*>(m_ptr) + std::bit_cast<R*>(m_ptr) + offset);
    }

protected:
    [[nodiscard]] inline T* get_raw() const
    {
        return m_ptr;
    }

private:
    friend class AddressPool;
    T* m_ptr = nullptr;
};
