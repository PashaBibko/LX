// dllmain.cpp : Defines the entry point for the DLL application.
#include <LX-Common.h>

namespace LX
{
    // All the different functions that can be called by the system //
    // Currently all empty but here for easier future development   //

    using DllFunc = bool(HMODULE);

    static bool ProcAttach(HMODULE hModule)
    {
        Log::Init(); // Initalises the log before the main process starts
        return true;
    }

    static bool ProcDetach(HMODULE hModule)
    {
        return true;
    }

    static bool ThreadAttach(HMODULE hModule) { return true; }
    static bool ThreadDetach(HMODULE hModule) { return true; }
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    // All the functions that might be called with their relevant state //
    static const std::unordered_map<DWORD, LX::DllFunc&> funcs =
    {
        { DLL_PROCESS_ATTACH, LX::ProcAttach    },
        { DLL_PROCESS_DETACH, LX::ProcDetach    },
        { DLL_THREAD_ATTACH , LX::ThreadAttach  },
        { DLL_THREAD_DETACH , LX::ThreadDetach  }
    };

    // Returns the output of the relavant function //
    return funcs.at(ul_reason_for_call)(hModule);
}

