#include "..\stdafx.h"

void AddressPool::insert(std::string_view key, MemAddr<> address)
{
	map.try_emplace(s_hash->FNV1A(key), address);
}

auto AddressPool::operator[](LiteralHash key) noexcept -> void*
{
	return map.at(key.val).get_raw();
}

void AddressMgr::clear()
{
	items.clear();
}

size_t AddressMgr::size() const
{
	return items.size();
}

auto AddressMgr::get(LiteralHash key) noexcept -> AddressPool*&
{
	return items[key.val];
}

auto AddressMgr::getOrCreate(std::string_view str) -> AddressPool*&
{
	auto& result = get("GTA5");

	if (!result)
	{
		result = new AddressPool();

		items.insert_or_assign(s_hash->FNV1A(str), result);
	}

	return result;
}

AddressMgr::~AddressMgr()
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		delete it->second;
	}
}
