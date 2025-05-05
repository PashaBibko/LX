using System;
using System.ComponentModel;

namespace LX_Build
{
    class Program
    {
        static void CompileToObj(string inPath, string outPath)
        {
            // The arguments to compiler LLVM IR to object files //
            string arguments = $"-filetype=obj -o \"{outPath}\" \"{inPath}\"";

            // Runs the command //
            CommandProcess process = new("llc", arguments);

            if (process.ExitCode() == 0)
            {
                Console.WriteLine(inPath + " -> " + outPath);
                return;
            }

            Console.WriteLine(process.Error());
        }

        static void LinkToExe(string objectFile)
        {
            // The arguments to turn object files into an .exe //
            string arguments = $"/OUT:example/Main.exe {objectFile} /ENTRY:main";

            // Runs the command //
            CommandProcess process = new("lld-link", arguments);

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
            if (LX_API.GenIR("example/main.lx", "example/main.ll") != 0)
            {
                // Quits if the IR Generation fails //
                // The C++ script handles all of the error message outputting //
                return;
            }

            // Compilers the LLVM IR to an object file using the command line //
            CompileToObj("example/main.ll", "example/main.obj");

            // Links the object file to an .exe //
            LinkToExe("example/main.obj");

            // Runs the outputted .exe //
            string command = "example/Main.exe";
            CommandProcess exe = new(command);

            // Outputs that the program ended with {x} exit code //
            Console.WriteLine("\nProcess {Main.exe} finished with exit code: " + exe.ExitCode());
        }
    }
}
