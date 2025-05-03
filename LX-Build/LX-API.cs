using System;
using System.Runtime.InteropServices;

namespace LX_Build
{
    internal class LX_API
    {
        // Imports SetDllDirectory to change where Dlls are imported from //
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        // Imports the Frontend of the compiler //
        [DllImport("Generator.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int GenIR(string arg1, string arg2, string? arg3);

        // Sets the directory to import the DLLs from //
        public static void Init()
        {
            #if DEBUG
                SetDllDirectory("bin\\x64\\Debug");
            #else
                SetDllDirectory("bin\\x64\\Release");
            #endif
        }
    }
}
