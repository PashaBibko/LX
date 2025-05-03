using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LX_Build
{
    internal class CommandProcess
    {
        private readonly string m_ErrorMessage;
        private readonly int m_ExitCode;

        public int ExitCode() => m_ExitCode;

        public string Error() => m_ErrorMessage;

        public CommandProcess(string command)
        {
            // Creates a process to run the command //
            ProcessStartInfo info = new()
            {
                FileName = "cmd.exe",
                Arguments = $"/c {command}",
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            // Starts the process //
            using Process process = Process.Start(info);

            // Reads the streams of the output //
            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();

            // Waits for the process to exit //
            process.WaitForExit();

            // Assigns output of the error for external access //
            m_ExitCode = process.ExitCode;
            m_ErrorMessage = error;
        }
    }
}
