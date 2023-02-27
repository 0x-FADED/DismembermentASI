#include "stdafx.h"
#include "Game.h"

AddressMgr g_addresses;

bool Game::InititalizeGame()
{
	auto addresses = g_addresses.getOrCreate("GTA5");

	//	auto hmodoule = (PBYTE)GetModuleHandle(nullptr);

#pragma region rage::fwScriptGuid::GetBaseFromGuid

	auto pattern = BytePattern("48 8B FA C6 44 24 ? ? E8");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find rage::fwScriptGuid::GetBaseFromGuid pattern.");
		return false;
	}

	auto result = pattern.rip(9);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found rage::fwScriptGuid::GetBaseFromGuid address at {:#X}", result - hmodoule));

	addresses->insert("rage::fwScriptGuid::GetBaseFromGuid", result);

#pragma endregion

#pragma region CEntity::GetIndexForBoneId

	pattern = BytePattern("E8 ? ? ? ? 41 3B C5 74 ? 48");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find CEntity::GetIndexForBoneId pattern.");
		return false;
	}

	result = pattern.rip(1);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CEntity::GetIndexForBoneId address at {:#X}", result - hmodoule));

	addresses->insert("CEntity::GetIndexForBoneId", result);

#pragma endregion

#pragma region rage::fragCache::GetEntityFragCache

	pattern = BytePattern("0F BA 77 ? ? 44 8B D3");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find rage::fragCache::GetEntityFragCache pattern.");
		return false;
	}

	result = pattern.rip(-4);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found rage::fragCache::GetEntityFragCache address at {:#X}", result - hmodoule));

	addresses->insert("rage::fragCache::GetEntityFragCache", result);

#pragma endregion

#pragma region rage::fragCache::GetLastSiblingBoneIndex

	pattern = BytePattern("33 D2 45 0F BF 48 ?");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find rage::fragCache::GetLastSiblingBoneIndex pattern.");
		return false;
	}

	result = pattern.get(-0x15);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found rage::fragCache::GetLastSiblingBoneIndex address at {:#X}", result - hmodoule));

	addresses->insert("rage::fragCache::GetLastSiblingBoneIndex", result);

#pragma endregion

#pragma region rage::fragCache::DrawSkeleton // we will redirect the calls all of them

	pattern = BytePattern("0F 18 ? 48 8B CA");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find find hook pattern (rage::fragCache::DrawSkeleton #1). Cannot continue.");
		return false;
	}


	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found fragCache::DrawSkeleton #1 address at {:#X}", pattern.get(0x170) - hmodoule));

	addresses->insert("fragCache::DrawSkeleton_1", pattern.get(0x170));

	pattern = BytePattern("44 88 44 24 ? 45 8B C4");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find hook pattern (rage::fragCache::DrawSkeleton #2). Cannot continue.");
		return false;
	}

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found fragCache::DrawSkeleton #2 address at {:#X}", pattern.get(-0x9B) - hmodoule));
	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found fragCache::DrawSkeleton #2 address at {:#X}", pattern.get(11) - hmodoule));
	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found fragCache::DrawSkeleton #2 address at {:#X}", pattern.get(0x7D) - hmodoule));

	addresses->insert("fragCache::DrawSkeleton_2", pattern.get(-0x9B));
	addresses->insert("fragCache::DrawSkeleton_3", pattern.get(11));
	addresses->insert("fragCache::DrawSkeleton_4", pattern.get(0x7D));

	pattern = BytePattern("41 0F 44 C5 48 8B D7");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find hook pattern (rage::fragCache::DrawSkeleton #3). Cannot continue.");
		return false;
	}

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found fragCache::DrawSkeleton #3 address at {:#X}",pattern.get(0x13) - hmodoule));

	addresses->insert("fragCache::DrawSkeleton_5", pattern.get(0x13));


#pragma endregion

	return true;
}