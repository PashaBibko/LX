using System;
using System.Diagnostics;

namespace LX_Build
{
    internal class CommandProcess
    {
        private string m_ErrorMessage = string.Empty;
        private int m_ExitCode = 0;

        private void HandleProcess(Process process)
        {
            // Reads the streams of the output //
            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();

            // Waits for the process to exit //
            process.WaitForExit();

            // Assigns output of the error for external access //
            m_ExitCode = process.ExitCode;
            m_ErrorMessage = error;
        }

        public CommandProcess(string command)
        {
            // Creates a process to run the command //
            ProcessStartInfo info = new()
            {
                FileName = command,
                Arguments = "",
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
                CreateNoWindow = true
            };

            // Starts the process and checks it is running //
            using Process? process = Process.Start(info)
                ?? throw new Exception($"Process failed to start: [{command}]");

            HandleProcess(process);
        }

        public CommandProcess(string command, string arguments)
        {
            // Creates a process to run the command //
            ProcessStartInfo info = new()
            {
                FileName = command,
                Arguments = arguments,
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            // Starts the process and checks it is running //
            using Process? process = Process.Start(info)
                ?? throw new Exception($"Process failed to start: [{command} {arguments}]");

            HandleProcess(process);
        }

        public int ExitCode() => m_ExitCode;

        public string Error() => m_ErrorMessage;
    }
}
