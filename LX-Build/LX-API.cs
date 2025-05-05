using System;
using System.Runtime.InteropServices;

namespace LX_Build
{
    internal partial class LX_API
    {
        // Imports SetDllDirectory to change where Dlls are imported from //
        [LibraryImport("kernel32.dll", EntryPoint = "SetDllDirectoryW", SetLastError = true, StringMarshalling = StringMarshalling.Utf16)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static partial bool SetDllDirectory(string lpPathName);

        // Imports the Frontend of the compiler //
        [LibraryImport ("Generator.dll", StringMarshalling = StringMarshalling.Custom,
            StringMarshallingCustomType = typeof(System.Runtime.InteropServices.Marshalling.AnsiStringMarshaller))]
        [UnmanagedCallConv(CallConvs = new Type[] { typeof(System.Runtime.CompilerServices.CallConvCdecl) })]
        public static partial int GenIR(string arg1, string arg2);

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
