#pragma once

typedef rage::CEntity* CEntity;

namespace Game 
{

	bool InititalizeGame();

	inline auto rage_fwScriptGuid_GetBaseFromGuid(int32_t handle) -> CEntity
	{
		return ((CEntity(__fastcall*)(int32_t))
			(*g_addresses.get("GTA5"))["rage::fwScriptGuid::GetBaseFromGuid"].addr)(handle);
	}

	inline auto CEntity__GetIndexForBoneId(CEntity entity, uint16_t boneId) -> int
	{
		return ((int(__fastcall*)(CEntity, uint16_t))
			(*g_addresses.get("GTA5"))["CEntity::GetIndexForBoneId"].addr)(entity, boneId);
	}

	inline auto rage__fragCache__GetEntityFragCache(CEntity entity) -> rage::fragCache*
	{
		return ((rage::fragCache*(__fastcall*)(CEntity))
			(*g_addresses.get("GTA5"))["rage::fragCache::GetEntityFragCache"].addr)(entity);
	}

	inline auto rage__fragCache__GetLastSiblingBoneIndex(rage::fragCache* fragCache, uint16_t boneId) -> int
	{
		return ((int(__fastcall*)(rage::fragCache*, uint16_t))
			(*g_addresses.get("GTA5"))["rage::fragCache::GetLastSiblingBoneIndex"].addr)(fragCache, boneId);
	}
}
