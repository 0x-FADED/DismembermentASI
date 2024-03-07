#pragma once

#define NOMINMAX
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <xmmintrin.h>
#include <filesystem>
#include <cassert>

#include "include\ankerl\unordered_dense.h"
#include "flat_map\flat_map.hpp"

#include "Utility/MemAddr.h"
#include "Utility/pattern.h"
#include "Utility/AddressMgr.h"

#include "Utility/Logger.h"
#include "Utility/Hooking.h"

#pragma region RAGE Includes

#include "rage/datBase.h"
#include "rage/pgBase.h"
#include "rage/crSkeleton.h"
#include "rage/CBaseModelInfo.h"
#include "rage/CEntity.h"

#pragma endregion

void initialize();

void unload();
