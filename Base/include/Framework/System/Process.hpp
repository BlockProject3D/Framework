// Copyright (c) 2020, BlockProject 3D
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject 3D nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include "Framework/IO/IInputStream.hpp"
#include "Framework/IO/IOutputStream.hpp"
#include "Framework/String.hpp"
#include "Framework/System/Application.hpp"

namespace bpf
{
    namespace system
    {
#ifdef WINDOWS
        constexpr const char *PROCESS_DEFAULT_PATH = "%SystemRoot%/system32;%SystemRoot%;%SystemRoot%/System32/Wbem";
#else
        constexpr const char *PROCESS_DEFAULT_PATH = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin";
#endif

        /**
         * Checks if a process has crashed. Please note that this check may not be accurate for all applications
         * @param exitcode the exit code as returned by Process.GetExitCode()
         * @return true if process has crashed, false otherwise
         */
        inline constexpr bool ProcessCrashed(const fint exitcode)
        {
#ifdef WINDOWS
            return ((uint32)exitcode > 0x80000000);
#else
            return (exitcode > 128);
#endif
        }

        /**
         * Checks if a process is running
         * Please note that this check may not be accurate for all applications
         * @param exitcode the exit code as returned by Process.GetExitCode()
         * @return true if process is running, false otherwise
         */
        inline constexpr bool ProcessRunning(const fint exitcode)
        {
            return (exitcode == -1);
        }

        /**
         * Checks if a process has finished correctly. Please note that this check may not be accurate for all
         * applications, certain applications may trick the exit code in thinking it errored when it did not (eg:
         * Windows help.exe)
         * @param exitcode the exit code as returned by Process.GetExitCode()
         * @return true if process has finished correctly, false otherwise
         */
        inline constexpr bool ProcessFinished(const fint exitcode)
        {
            return (exitcode == 0);
        }

        class BPF_API Process
        {
        public:
            class BPF_API Builder
            {
            private:
                bool _redirectStdIn;
                bool _redirectStdOut;
                bool _redirectStdErr;
                io::File _workDir;
                collection::HashMap<String, String> _envp;
                String _appExe;
                String _appName;
                collection::Array<String> _argv;

#ifdef WINDOWS
                void CleanupHandles(void *fdStdOut[2], void *fdStdErr[2], void *fdStdIn[2]);
#else
                void CleanupHandles(int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2]);
                void ProcessWorker(int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2]);
                Process ProcessMaster(int pid, int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2]);
#endif

            public:
                /**
                 * Constructs a Process::Builder
                 */
                explicit Builder();

                /**
                 * Constructs a Process::Builder
                 * @param app instance of calling Application to copy defaults from
                 */
                explicit Builder(const Application &app);

                /**
                 * Sets the application to run as a separate process
                 * @param appName the application path or name string (this overload does automatically check the PATH
                 * using this Builder's environment)
                 * @throw io::IOException when the application could not be found
                 * @return Builder reference
                 */
                Builder &SetApplication(const String &name);

                /**
                 * Sets the process working directory
                 * @param dir the new directory
                 * @throw io::IOException when dir does not exist or is not a directory
                 * @return Builder reference
                 */
                Builder &SetWorkingDirectory(const io::File &dir);

                /**
                 * Sets the arguments
                 * @param args arguments to pass to the new process
                 * @return Builder reference
                 */
                inline Builder &SetArguments(const collection::Array<String> &args)
                {
                    _argv = args;
                    return (*this);
                }

                /**
                 * Sets the arguments
                 * @param args arguments to pass to the new process
                 * @return Builder reference
                 */
                inline Builder &SetArguments(collection::Array<String> &&args)
                {
                    _argv = std::move(args);
                    return (*this);
                }

                /**
                 * Sets the environment
                 * @param env the new environment
                 * @return Builder reference
                 */
                inline Builder &SetEnvironment(const collection::HashMap<String, String> &env)
                {
                    _envp = env;
                    return (*this);
                }

                /**
                 * Sets the environment
                 * @param env the new environment
                 * @return Builder reference
                 */
                inline Builder &SetEnvironment(collection::HashMap<String, String> &&env)
                {
                    _envp = std::move(env);
                    return (*this);
                }

                /**
                 * Enables standard input redirect
                 * @return Builder reference
                 */
                inline Builder &RedirectInput()
                {
                    _redirectStdIn = true;
                    return (*this);
                }

                /**
                 * Enables standard output redirect
                 * @return Builder reference
                 */
                inline Builder &RedirectOutput()
                {
                    _redirectStdOut = true;
                    return (*this);
                }

                /**
                 * Enables standard error redirect
                 * @return Builder reference
                 */
                inline Builder &RedirectError()
                {
                    _redirectStdErr = true;
                    return (*this);
                }

                /**
                 * Create a process from the information defined in this builder
                 * @throw OSException in case the process could not be created
                 * @return instance to represent the new process
                 */
                Process Build();
            };

        public:
            /**
             * Class to represent a pipe stream to be used for process link across platforms
             */
            class BPF_API PipeStream final : public io::IInputStream, public io::IOutputStream
            {
            private:
#ifdef WINDOWS
                void *_pipeHandles[2];
                PipeStream(void *pipefd[2]);
                ~PipeStream();
#else
                int _pipfd[2];
                PipeStream(int pipefd[2]);
                ~PipeStream();
#endif

                PipeStream(PipeStream &&other);
                PipeStream &operator=(PipeStream &&other);

            public:
                /**
                 * Do not allow copy construction
                 */
                PipeStream(const PipeStream &other) = delete;

                /**
                 * Do not allow copy assignment
                 */
                PipeStream &operator=(const PipeStream &other) = delete;

                /**
                 * Reads bytes from this stream, no buffering is performed
                 * @param buf buffer to receive the read bytes
                 * @param bufsize the size of the receiving buffer
                 * @throw IOException in case of system error
                 * @return number of bytes read
                 */
                fsize Read(void *buf, fsize bufsize);

                /**
                 * Writes bytes to this stream, no buffering is performed
                 * @param buf the buffer with the bytes to write
                 * @param bufsize the size of the buffer
                 * @throw IOException in case of system error
                 * @return number of bytes written
                 */
                fsize Write(const void *buf, fsize bufsize);

                /**
                 * Closes the pipe
                 */
                void Close();

                friend class Process;
            };

            fint _lastExitCode;
            bool _redirectStdIn;
            bool _redirectStdOut;
            bool _redirectStdErr;
            PipeStream _stdIn;
            PipeStream _stdOut;
            PipeStream _stdErr;

#ifdef WINDOWS
            void *_pHandle;
            void *_tHandle;
            Process(void *pinfo, void *fdStdIn[2], void *fdStdOut[2], void *fdStdErr[2]);
#else
            int _pid;
            Process(int pid, int fdStdIn[2], int fdStdOut[2], int fdStdErr[2]);
#endif

        public:
            /**
             * Move constructor
             */
            Process(Process &&other);

            /**
             * Force compiler to NEVER ever copy that class
             */
            Process(const Process &other) = delete;

            /**
             * Force compiler to NEVER ever copy that class
             */
            Process &operator=(const Process &other) = delete;

            /**
             * Move assignment operator
             */
            Process &operator=(Process &&other);

            /**
             * Destructor, the process gets automatically killed if this is reached
             */
            ~Process();

            /**
             * Wait for the process to terminate
             * @throw OSException in case the system can't wait for the process
             */
            void Wait();

            /**
             * Kill the target process
             * @param force true to force kill, false otherwise
             * @throw OSException in case the system could not send signal
             */
            void Kill(bool force);

            /**
             * Returns the exit code of the process
             * @throw OSException in case the system could not poll the target process
             * @return exit code as an integer, -1 if the process is still running
             */
            fint GetExitCode(); // WAITPID WNOHANG

            /**
             * Provides access to process standard input
             * @throw OSException if the target process does not allow standard input redirection
             * @return reference to IOutputStream
             */
            PipeStream &GetStandardInput();

            /**
             * Provides access to process standard output
             * @throw OSException if the target process does not allow standard output redirection
             * @return reference to IInputStream
             */
            PipeStream &GetStandardOutput();

            /**
             * Provides access to process standard error
             * @throw OSException if the target process does not allow standard error redirection
             * @return reference to IInputStream
             */
            PipeStream &GetStandardError();

            friend class Process::Builder;
        };
    }
}
