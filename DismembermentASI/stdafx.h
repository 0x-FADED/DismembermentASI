#pragma once

#include <windows.h>
#include <inttypes.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <xmmintrin.h>

#include "Utility/MemAddr.h"
#include "Utility/AddressMgr.h"

#include "Utility/Utility.h"
#include "Utility/Logger.h"
#include "Utility/Hooking.h"
#include "Utility/pattern.h"

#pragma region RAGE Includes

#include "rage/datBase.h"
#include "rage/pgBase.h"
#include "rage/crSkeleton.h"
#include "rage/fragCache.h"
#include "rage/CBaseModelInfo.h"
#include "rage/CEntity.h"
#include "rage/CPed.h"

#pragma endregion

void initialize();

void unload();
