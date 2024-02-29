// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		s_LOG->Write("mod Initialized.");
		initialize();
		break;

	case DLL_PROCESS_DETACH:
		unload();
		s_LOG->Write("mod Uninitialized.");
		break;
	}
	return TRUE;
}

