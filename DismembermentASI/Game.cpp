#include "stdafx.h"
#include "Game.h"

AddressMgr g_addresses;

bool Game::InititalizeGame()
{
	auto addresses = g_addresses.getOrCreate("game");

	// SHV uses E8 ? ? ? ? 49 8D 76 ? 8B + 0x1 so we use a entirely different function where we don't need to deal with CEntity bullshit
#pragma region getScriptEntityIndex

	auto pattern = BytePattern("48 8B FA C6 44 24 ? ? E8");

	if (!pattern.bSuccess) {

		LOG("Failed to find getScriptGuidForEntityIndex pattern.");
		return false;
	}

	addresses->insert("getScriptGuidForEntityIndex", pattern.rip(9));

#pragma endregion

#pragma region getBoneIndexForId

	pattern = BytePattern("E8 ? ? ? ? 41 3B C5 74 ? 48");

	if (!pattern.bSuccess) {

		LOG("Failed to find getBoneIndexForId pattern.");
		return false;
	}

	addresses->insert("getBoneIndexForId", pattern.rip(1));

#pragma endregion

#pragma region getEntityFragCache

	pattern = BytePattern("0F BA 77 ? ? 44 8B D3");

	if ( !pattern.bSuccess ) {

		LOG("Failed to find getEntityFragCache pattern.");
		return false;
	}

	addresses->insert("getEntityFragCache", pattern.rip(-4));

#pragma endregion

#pragma region getLastSiblingBoneIndex

	pattern = BytePattern("33 D2 45 0F BF 48 ?");

	if (!pattern.bSuccess) {

		LOG("Failed to find getLastSiblingBoneIndex pattern.");
		return false;
	}

	addresses->insert("getLastSiblingBoneIndex", pattern.get(-0x15));

#pragma endregion

#pragma region fragCache__DrawSkeleton // we will redirect the calls all of them

	pattern = BytePattern("0F 18 ? 48 8B CA");

	if (!pattern.bSuccess) {

		LOG("Failed to find find hook pattern (fragCache::DrawSkeleton #1). Cannot continue.");
		return false;
	}

	addresses->insert("fragCache::DrawSkeleton_1", pattern.get(0x170));

	pattern = BytePattern("44 88 44 24 ? 45 8B C4");

	if (!pattern.bSuccess) {

		LOG("Failed to find hook pattern (fragCache::DrawSkeleton #2). Cannot continue.");
		return false;
	}

	addresses->insert("fragCache::DrawSkeleton_2", pattern.get(-0x9B));
	addresses->insert("fragCache::DrawSkeleton_3", pattern.get(11));
	addresses->insert("fragCache::DrawSkeleton_4", pattern.get(0x7D));

	pattern = BytePattern("41 0F 44 C5 48 8B D7");

	if (!pattern.bSuccess) {

		LOG("Failed to find hook pattern (fragCache::DrawSkeleton #3). Cannot continue.");
		return false;
	}

	addresses->insert("fragCache::DrawSkeleton_5", pattern.get(0x13));


#pragma endregion

	return true;
}
