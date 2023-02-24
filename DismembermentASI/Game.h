#pragma once

typedef rage::CEntity* CEntity;

namespace Game 
{

	bool InititalizeGame();

	inline CEntity rage_fwScriptGuid_GetBaseFromGuid(int32_t handle)
	{
		return ((CEntity(__fastcall*)(int32_t))
			(*g_addresses.get("game"))["rage::fwScriptGuid::GetBaseFromGuid"].addr)(handle);
	}

	inline int GetBoneIndexForId(CEntity entity, uint16_t boneId)
	{
		return ((int(__fastcall*)(CEntity, uint16_t))
			(*g_addresses.get("game"))["getBoneIndexForId"].addr)(entity, boneId);
	}

	inline rage::fragCache* GetEntityFragCache(CEntity entity) 
	{
		return ((rage::fragCache*(__fastcall*)(CEntity))
			(*g_addresses.get("game"))["getEntityFragCache"].addr)(entity);
	}

	inline int GetLastSiblingBoneIndex(rage::fragCache* fragCache, uint16_t boneId)
	{
		return ((int(__fastcall*)(rage::fragCache*, uint16_t))
			(*g_addresses.get("game"))["getLastSiblingBoneIndex"].addr)(fragCache, boneId);
	}
}
