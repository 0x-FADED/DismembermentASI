#pragma once

typedef rage::CEntity* CEntity;

namespace Game 
{

	bool InititalizeGame();

	inline CEntity rage_fwScriptGuid_GetBaseFromGuid(int32_t handle)
	{
		return ((CEntity(__fastcall*)(int32_t))
			(*g_addresses.get("GTA5"))["rage::fwScriptGuid::GetBaseFromGuid"].addr)(handle);
	}

	inline int CEntity__GetIndexForBoneId(CEntity entity, uint16_t boneId)
	{
		return ((int(__fastcall*)(CEntity, uint16_t))
			(*g_addresses.get("GTA5"))["CEntity::GetIndexForBoneId"].addr)(entity, boneId);
	}

	inline rage::fragCache* rage__fragCache__GetEntityFragCache(CEntity entity)
	{
		return ((rage::fragCache*(__fastcall*)(CEntity))
			(*g_addresses.get("GTA5"))["rage::fragCache::GetEntityFragCache"].addr)(entity);
	}

	inline int rage__fragCache__GetLastSiblingBoneIndex(rage::fragCache* fragCache, uint16_t boneId)
	{
		return ((int(__fastcall*)(rage::fragCache*, uint16_t))
			(*g_addresses.get("GTA5"))["rage::fragCache::GetLastSiblingBoneIndex"].addr)(fragCache, boneId);
	}
}
