#pragma once

#ifdef DismembermentASI_Export
#define Dismemberment_API extern "C" __declspec(dllexport)
#else
#define Dismemberment_API extern "C" __declspec(dllimport)
#endif

using Ped = int32_t;

Dismemberment_API void AddBoneDraw(Ped handle, int32_t start, int32_t end);
Dismemberment_API void RemoveBoneDraw(Ped handle);