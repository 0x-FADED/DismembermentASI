#include "stdafx.h"
#include "Game.h"
#include <mutex>

using namespace Game;

#define DLL_EXPORT extern "C" __declspec( dllexport )

typedef CPed* Cped;

using Ped = int32_t;

typedef std::uint32_t (*rage__fragCache__DrawSkeleton)(rage::fragCache*, uint32_t*, int, CBaseModelInfo*, bool, __int64, uint8_t, uint8_t, short, short, float);

static std::vector<CallHook<rage__fragCache__DrawSkeleton>*> g_drawFunctions;

struct DrawSkeletonInfo
{
	/**
	 * id of first bone in the skeleton that will be drawn.
	 */
	int startBoneId;

	/**
	 * id of last bone in hierarchy.
	 */
	int endBoneId;
};

std::map<Ped, DrawSkeletonInfo> g_pedList;

std::mutex g_mutex;

/**
 * Main function where the skeleton is drawn by the engine.
 */
std::uint32_t rage__fragCache__DrawSkeleton_Hook(rage::fragCache* fragCache, uint32_t* drawBuffer, int isFragment, CBaseModelInfo* modelInfo, bool bUnk, __int64 unkBoneIndex, uint8_t componentType, uint8_t subFragCache, short startBoneIndex, short lastSiblingIndex, float drawScale)
{
	
	std::unique_lock<std::mutex> lock(g_mutex);

	for (auto it = g_pedList.begin(); it != g_pedList.end();)
	{
		auto pedAddress = reinterpret_cast<Cped>(GetScriptGuidForEntityIndex(it->first));

		if (!pedAddress)
		{
			it = g_pedList.erase(it);
		}

		else
		{
			auto pedCache = GetEntityFragCache(pedAddress);

			if (pedCache && pedCache == fragCache)
			{
				if (it->second.startBoneId != -1)
				{
					startBoneIndex = fragCache->m_skeleton.m_skeletonData->getBoneIndexFormId(it->second.startBoneId);

					if (it->second.endBoneId != -1)
						lastSiblingIndex = GetBoneIndexForId(pedAddress, it->second.endBoneId);

					else
						lastSiblingIndex = GetLastSiblingBoneIndex(fragCache, startBoneIndex);
				}

				drawScale = 0.0f;

				break;
			}

			++it;
		}
	}

	return g_drawFunctions[0]->fn(fragCache, drawBuffer, isFragment, modelInfo, bUnk, unkBoneIndex, componentType, subFragCache, startBoneIndex, lastSiblingIndex, drawScale);
}

void initialize() 
{
	if (!InititalizeGame()) 
	{
		LOG("Failed to initialize game. Cannot continue.");
		return;
	}

	auto& loc = *g_addresses.get("game"); // using minhook and hooking the originial function would have been better

	g_drawFunctions.push_back(HookManager::SetCall<rage__fragCache__DrawSkeleton, NULL>(((PBYTE)loc["fragCache::DrawSkeleton_1"].addr), rage__fragCache__DrawSkeleton_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<rage__fragCache__DrawSkeleton, NULL>(((PBYTE)loc["fragCache::DrawSkeleton_2"].addr), rage__fragCache__DrawSkeleton_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<rage__fragCache__DrawSkeleton, NULL>(((PBYTE)loc["fragCache::DrawSkeleton_3"].addr), rage__fragCache__DrawSkeleton_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<rage__fragCache__DrawSkeleton, NULL>(((PBYTE)loc["fragCache::DrawSkeleton_4"].addr), rage__fragCache__DrawSkeleton_Hook));

	g_drawFunctions.push_back(HookManager::SetCall<rage__fragCache__DrawSkeleton, NULL>(((PBYTE)loc["fragCache::DrawSkeleton_5"].addr), rage__fragCache__DrawSkeleton_Hook));
}

DLL_EXPORT void AddBoneDraw(Ped handle, int start, int end)
{
	g_pedList[handle].startBoneId = start;
	g_pedList[handle].endBoneId = end;
}

DLL_EXPORT void RemoveBoneDraw(Ped handle)
{
	std::unique_lock<std::mutex> lock(g_mutex);

	auto pair = g_pedList.find( handle );

	if (pair != g_pedList.end())
	{
		g_pedList.erase( handle );
	}
}

void unload() 
{
	for (auto& function : g_drawFunctions)
	{
		delete function;
	}
}
