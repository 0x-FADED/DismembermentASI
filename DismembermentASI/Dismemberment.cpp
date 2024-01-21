#include "stdafx.h"
#include "Game.h"

using namespace Game;

#define DLL_EXPORT extern "C" __declspec( dllexport )

using Ped = int32_t;

typedef std::uint16_t(*CopyOffMatrixSet)(const rage::crSkeleton&, uint32_t&, int32_t, CBaseModelInfo*, bool, uint16_t*, uint8_t, uint8_t, short, short, float);

constinit std::vector<CallHook<CopyOffMatrixSet>*> g_drawFunctions;

struct DrawSkeletonInfo
{
	/**
	 * id of first bone in the skeleton that will be drawn.
	 */
	int32_t startBoneId;

	/**
	 * id of last bone in hierarchy.
	 */
	int32_t endBoneId;
};

std::map<Ped, DrawSkeletonInfo> g_pedList;

std::mutex g_mutex;

/**
 * Main function where the skeleton is drawn by the engine.
 */
auto CopyOffMatrixSet_Hook(const rage::crSkeleton& crSkel, uint32_t& drawBuffer, int32_t isFragment, CBaseModelInfo* modelInfo, bool drawListAddr, uint16_t* BoneMap, uint8_t componentType, uint8_t subFragCache, short startBoneIndex, short lastSiblingIndex, float drawScale) -> std::uint16_t
{

	std::unique_lock<std::mutex> lock(g_mutex);

	for (auto it = g_pedList.begin(); it != g_pedList.end();)
	{
		auto pedGUID = rage_fwScriptGuid_GetBaseFromGuid(it->first);

		if (!pedGUID)
		{
			it = g_pedList.erase(it);
		}

		else
		{
			auto dSkel = GetSkeletonForDraw((CPed)pedGUID);

			if (dSkel && dSkel == &crSkel)
			{
				if (it->second.startBoneId != -1)
				{
					startBoneIndex = crSkel.m_skeletonData->ConvertBoneIdToIndex(it->second.startBoneId);
					
					if (it->second.endBoneId != -1)
						lastSiblingIndex = CDynamicEntity__GetBoneIndexFromBoneTag((CDynamicEntity)pedGUID, it->second.endBoneId);

					else
						lastSiblingIndex = crSkel.GetTerminatingPartialBone(startBoneIndex); /** rage__crSkeleton__GetTerminatingPartialBone(crSkel, startBoneIndex); **/
					
				}

				drawScale = 0.0f;

				break;
			}

			++it;
		}
	}

	return g_drawFunctions[0]->fn(crSkel, drawBuffer, isFragment, modelInfo, drawListAddr, BoneMap, componentType, subFragCache, startBoneIndex, lastSiblingIndex, drawScale);
}

void initialize()
{
	if (!InititalizeGame())
	{
		LOG.Write(LogLevel::LOG_ERROR, "Failed to initialize game cannot continue.");
		return;
	}

	LOG.Write(LogLevel::LOG_INFO, "All patterns are valid proceeding to hook CopyOffMatrixSet calls.");

	auto& loc = *g_addresses.get("GTA5"); // using minhook and hooking the originial function would have been better

	//1st three are inside CAddCompositeSkeletonCommand::CAddCompositeSkeletonCommand
	//4th one is inside CAddSkeletonCommand::CAddSkeletonCommand
	//5th one is inside CPedBigPrototype::AddDataForEntity

	g_drawFunctions.push_back(HookManager::SetCall<CopyOffMatrixSet, NULL>(((PBYTE)loc["CopyOffMatrixSet_1"].addr), CopyOffMatrixSet_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<CopyOffMatrixSet, NULL>(((PBYTE)loc["CopyOffMatrixSet_2"].addr), CopyOffMatrixSet_Hook)); 

	g_drawFunctions.push_back(HookManager::SetCall<CopyOffMatrixSet, NULL>(((PBYTE)loc["CopyOffMatrixSet_3"].addr), CopyOffMatrixSet_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<CopyOffMatrixSet, NULL>(((PBYTE)loc["CopyOffMatrixSet_4"].addr), CopyOffMatrixSet_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<CopyOffMatrixSet, NULL>(((PBYTE)loc["CopyOffMatrixSet_5"].addr), CopyOffMatrixSet_Hook));

	LOG.Write(LogLevel::LOG_INFO, "Hooks successful!");
}

DLL_EXPORT void AddBoneDraw(Ped handle, int32_t start, int32_t end)
{
	g_pedList[handle].startBoneId = start;
	g_pedList[handle].endBoneId = end;
}

DLL_EXPORT void RemoveBoneDraw(Ped handle)
{
	std::unique_lock<std::mutex> lock(g_mutex);

	auto pair = g_pedList.find(handle);

	if (pair != g_pedList.end())
	{
		g_pedList.erase(handle);
	}
}

void unload()
{
	for (auto& function : g_drawFunctions)
	{
		delete function;
	}
}