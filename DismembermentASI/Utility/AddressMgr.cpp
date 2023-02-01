#include "..\stdafx.h"

void AddressPool::insert(const std::string_view key, MemAddr address)
{
	map.try_emplace(LiteralHash::FNV1A(key), address);
}

MemAddr& AddressPool::operator[](const LiteralHash key)
{
	return map.at(key.val);
}

void AddressMgr::clear()
{
	items.clear();
}

size_t AddressMgr::size() const
{
	return items.size();
}

AddressPool*& AddressMgr::get(const LiteralHash key)
{
	return items[key.val];
}

AddressPool*& AddressMgr::getOrCreate(const std::string_view str)
{
	auto& result = get("game");

	if (!result)
	{
		result = new AddressPool();

		items.insert_or_assign(LiteralHash::FNV1A(str), result);
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
