#pragma once

typedef rage::CEntity* CEntity;
typedef rage::CDynamicEntity* CDynamicEntity;

namespace Game 
{

	bool InititalizeGame();

	inline auto rage_fwScriptGuid_GetBaseFromGuid(int32_t GUID) -> rage::fwExtensibleBase*
	{
		return ((rage::fwExtensibleBase*(__fastcall*)(int32_t))
			(*g_addresses.get("GTA5"))["rage::fwScriptGuid::GetBaseFromGuid"].addr)(GUID);
	}

	inline auto CDynamicEntity__GetBoneIndexFromBoneTag(CDynamicEntity entity,const uint16_t boneId) -> const int32_t
	{
		return ((int32_t(__fastcall*)(CDynamicEntity,const uint16_t))
			(*g_addresses.get("GTA5"))["CDynamicEntity::GetBoneIndexFromBoneTag"].addr)(entity, boneId);
	}

	inline auto GetSkeletonForDrawIgnoringDrawlist(CEntity entity) -> const rage::crSkeleton*
	{
		return ((const rage::crSkeleton*(__fastcall*)(CEntity))
			(*g_addresses.get("GTA5"))["GetSkeletonForDrawIgnoringDrawlist"].addr)(entity);
	}
	/**
	inline auto rage__crSkeleton__GetTerminatingPartialBone(const rage::crSkeleton& crSkel, uint16_t boneId) -> const uint32_t
	{
		return ((uint32_t(__fastcall*)(const rage::crSkeleton&, uint16_t))
			(*g_addresses.get("GTA5"))["rage::crSkeleton::GetTerminatingPartialBone"].addr)(crSkel, boneId);
	}
	**/
}
