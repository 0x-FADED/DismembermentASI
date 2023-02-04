// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		LOG.Write(LogLevel::LOG_INFO, "mod Initialized.");
		initialize();
		break;

	case DLL_PROCESS_DETACH:
		unload();
		LOG.Write(LogLevel::LOG_INFO, "mod Uninitialized.");
		break;
	}
	return TRUE;
}

