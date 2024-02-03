#include "stdafx.h"
#include "Game.h"

AddressMgr g_addresses;

bool Game::InititalizeGame()
{
	auto addresses = g_addresses.getOrCreate("GTA5");

	//auto hmodoule = (BYTE*)GetModuleHandle(nullptr);

#pragma region rage::fwScriptGuid::GetBaseFromGuid

	auto pattern = Pattern("48 8B FA C6 44 24 ? ? E8");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find rage::fwScriptGuid::GetBaseFromGuid pattern.");
		return false;
	}

	auto result = pattern.find().get(9).rip(4);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found rage::fwScriptGuid::GetBaseFromGuid address at {:#X}", result - hmodoule));

	addresses->insert("rage::fwScriptGuid::GetBaseFromGuid", result);

#pragma endregion

#pragma region CDynamicEntity::GetBoneIndexFromBoneTag

	pattern = Pattern("41 0F 28 D1 45 33 C9 8B D0");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find CDynamicEntity::GetBoneIndexFromBoneTag pattern.");
		return false;
	}

	result = pattern.find().get(-0xD).rip(4);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CDynamicEntity::GetBoneIndexFromBoneTag address at {:#X}", result - hmodoule));

	addresses->insert("CDynamicEntity::GetBoneIndexFromBoneTag", result);

#pragma endregion

#pragma region GetSkeletonForDrawIgnoringDrawlist

	pattern = Pattern("48 03 F3 45 8B CF");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find GetSkeletonForDrawIgnoringDrawlist pattern.");
		return false;
	}

	result = pattern.find().get(-0x8A).rip(4);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found GetSkeletonForDrawIgnoringDrawlist address at {:#X}", result - hmodoule));

	addresses->insert("GetSkeletonForDrawIgnoringDrawlist", result);

#pragma endregion

/**
#pragma region rage::crSkeleton::GetTerminatingPartialBone

	pattern = Pattern<BYTE*>("33 D2 45 0F BF 48 ?");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find rage::crSkeleton::GetTerminatingPartialBone pattern.");
		return false;
	}

	result = pattern.get(-0x15);

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found rage::crSkeleton::GetTerminatingPartialBone address at {:#X}", result - hmodoule));

	addresses->insert("rage::crSkeleton::GetTerminatingPartialBone", result);

#pragma endregion
**/

#pragma region CopyOffMatrixSet // we will redirect the calls all of them

	pattern = Pattern("0F 18 ? 48 8B CA");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find find hook pattern (CopyOffMatrixSet #1). Cannot continue.");
		return false;
	}


	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CopyOffMatrixSet #1 address at {:#X}", pattern.get(0x170) - hmodoule));

	addresses->insert("CopyOffMatrixSet_1", pattern.find().get(0x170));

	pattern = Pattern("44 88 44 24 ? 45 8B C4");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find hook pattern (CopyOffMatrixSet #2). Cannot continue.");
		return false;
	}

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CopyOffMatrixSet #2 address at {:#X}", pattern.get(-0x9B) - hmodoule));
	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CopyOffMatrixSet #2 address at {:#X}", pattern.get(11) - hmodoule));
	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CopyOffMatrixSet #2 address at {:#X}", pattern.get(0x7D) - hmodoule));

	addresses->insert("CopyOffMatrixSet_2", pattern.find().get(-0x9B));
	addresses->insert("CopyOffMatrixSet_3", pattern.find().get(11));
	addresses->insert("CopyOffMatrixSet_4", pattern.find().get(0x7D));

	pattern = Pattern("41 0F 44 C5 48 8B D7");

	if (!pattern.bSuccess) {

		LOG.Write(LogLevel::LOG_ERROR, "Failed to find hook pattern (CopyOffMatrixSet #3). Cannot continue.");
		return false;
	}

	//	LOG.Write(LogLevel::LOG_DEBUG, std::format("found CopyOffMatrixSet #3 address at {:#X}",pattern.get(0x13) - hmodoule));

	addresses->insert("CopyOffMatrixSet_5", pattern.find().get(0x13));


#pragma endregion

	return true;
}