using System;
using System.Diagnostics;

namespace LX_Build
{
    class Program
    {
        static void CompileToObj(string inPath, string outPath)
        {
            // The command to compile the IR to .obj //
            string command = $"llc -filetype=obj -o \"{outPath}\" \"{inPath}\"";

            // Runs the command //
            CommandProcess process = new(command);

            if (process.ExitCode() == 0)
            {
                Console.WriteLine(inPath + " -> " + outPath);
                return;
            }

            Console.WriteLine(process.Error());
        }

        static void LinkToExe(string objectFile)
        {
            // The command to link the object file to an .exe //
            string command = $"lld-link /OUT:example/Main.exe {objectFile} /ENTRY:main";

            // Runs the command //
            CommandProcess process = new(command);

            if (process.ExitCode() == 0)
            {
                Console.WriteLine(objectFile + " -> " + "Main.exe");
                return;
            }

            Console.WriteLine(process.Error());
        }

        static void Main()
        {
            // Initalises the CPP interface, MUST ALWAYS BE CALLED FIRST //
            LX_API.Init();

            // Generates LLVM IR with the example files //
            _ = LX_API.GenIR("example/main.lx", "example/main.ll", "example/log");

            // Compilers the LLVM IR to an object file using the command line //
            CompileToObj("example/main.ll", "example/main.obj");

            // Links the object file to an .exe //
            LinkToExe("example/main.obj");
        }
    }
}
