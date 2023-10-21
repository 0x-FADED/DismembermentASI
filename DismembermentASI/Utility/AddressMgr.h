#pragma once

struct LiteralHash // optimizations using modern c++ features
{
	uint64_t val;

	LiteralHash() = delete;

	template<size_t Size>
	consteval LiteralHash(const char(&val)[Size]) : val(_FNV1A(val, Size - 1))
	{
	}

	static constexpr uint64_t FNV1A(const std::string_view str)
	{
		return _FNV1A(str.data(), str.length());
	}

	static constexpr uint64_t _FNV1A(const char* str, size_t length)
	{
		uint64_t hash = 0x100000001B3ull;

		for (size_t i = 0; i < length; i++)
		{
			hash ^= str[i];
			hash *= 0xCBF29CE484222325ull;
		}

		return hash;
	}
};

class AddressPool
{
private:
	ankerl::unordered_dense::map<std::uint64_t, MemAddr> map;
public:
	void insert(const std::string_view key, MemAddr address);
	MemAddr& operator[](const LiteralHash key);
};

class AddressMgr
{
private:
	ankerl::unordered_dense::map<std::uint64_t, AddressPool*> items;
public:
	void clear();
	size_t size() const;
	AddressPool*& get(const LiteralHash category);
	AddressPool*& getOrCreate(const std::string_view category);
	~AddressMgr();
};

extern AddressMgr g_addresses;