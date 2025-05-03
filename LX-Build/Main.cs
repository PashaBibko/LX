using System;

namespace LX_Build
{
    class Program
    {
        static void Main(string[] args)
        {
            // Initalises the CPP interface //
            LX_API.Init();

            // Generates LLVM IR with the example files //
            _ = LX_API.GenIR("example/main.lx", "example/main.ll", "example/log");
        }
    }
}
