// dllmain.cpp : Defines the entry point for the DLL application.
#include <LX-Common.h>

namespace LX
{
    using DllFunc = bool(HMODULE);

    static bool ProcAttach(HMODULE hModule)
    {
    }

    static bool ProcDetach(HMODULE hModule)
    {
    }

    static bool ThreadAttach(HMODULE hModule)
    {
    }

    static bool ThreadDetach(HMODULE hModule)
    {
    }
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return true;
}

