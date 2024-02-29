#pragma once

struct LiteralHash
{
	uint64_t val; //0x00 - 0x08

	LiteralHash() = delete;

	template<size_t Size>
	consteval LiteralHash(const char(&val)[Size]) : val(_FNV1A(val, Size - 1))
	{
	}

	static constexpr auto FNV1A(const std::string_view str) -> uint64_t
	{
		return _FNV1A(str.data(), str.length());
	}

private:
	static constexpr auto _FNV1A(const char* str, size_t length) -> uint64_t
	{
		uint64_t hash = 0x100000001B3ull;

		for (size_t i = 0; i < length; i++)
		{
			hash ^= str[i];
			hash *= 0xCBF29CE484222325ull;
		}

		return hash;
	}
}; // size: 0x08

class AddressPool //we are gonna store addresses as hashes instead of strings for faster lookup
{
private:
	ankerl::unordered_dense::segmented_map<std::uint64_t, MemAddr<>> map;
	static LiteralHash* s_hash;
public:
	void insert(std::string_view key, const MemAddr<>& address);
	auto operator[](LiteralHash key) noexcept -> void*;
};

class AddressMgr
{
private:
	ankerl::unordered_dense::segmented_map<std::uint64_t, AddressPool*> items;
	static LiteralHash* s_hash;
public:
	void clear();
	size_t size() const;
	auto get(LiteralHash category) noexcept -> AddressPool*&;
	auto getOrCreate(std::string_view category) -> AddressPool*&;
	~AddressMgr();
};

extern AddressMgr g_addresses;