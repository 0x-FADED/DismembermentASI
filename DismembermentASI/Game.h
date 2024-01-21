#pragma once

typedef rage::CPed* CPed;
typedef rage::CDynamicEntity* CDynamicEntity;

namespace Game 
{

	bool InititalizeGame();

	inline auto rage_fwScriptGuid_GetBaseFromGuid(int32_t GUID) -> rage::fwExtensibleBase*
	{
		return ((rage::fwExtensibleBase*(__fastcall*)(int32_t))
			(*g_addresses.get("GTA5"))["rage::fwScriptGuid::GetBaseFromGuid"].addr)(GUID);
	}

	inline auto CDynamicEntity__GetBoneIndexFromBoneTag(CDynamicEntity entity, uint16_t boneId) -> int32_t
	{
		return ((int32_t(__fastcall*)(CDynamicEntity, uint16_t))
			(*g_addresses.get("GTA5"))["CDynamicEntity::GetBoneIndexFromBoneTag"].addr)(entity, boneId);
	}

	inline auto GetSkeletonForDraw(CPed entity) -> rage::crSkeleton*
	{
		return ((rage::crSkeleton*(__fastcall*)(CPed))
			(*g_addresses.get("GTA5"))["GetSkeletonForDraw"].addr)(entity);
	}
	/**
	inline auto rage__crSkeleton__GetTerminatingPartialBone(const rage::crSkeleton& crSkel, uint16_t boneId) -> uint32_t
	{
		return ((uint32_t(__fastcall*)(const rage::crSkeleton&, uint16_t))
			(*g_addresses.get("GTA5"))["rage::crSkeleton::GetTerminatingPartialBone"].addr)(crSkel, boneId);
	}
	**/
}
